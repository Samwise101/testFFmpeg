#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadVideo("C:/Users/u/Documents/Qt/GS010001.360");
    mergeVideoStreams(40, 5, "C:/Users/u/Documents/Qt/video.avi");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadVideo(const char *path)
{

    int ret = avformat_open_input(&formatCtx, path, nullptr, nullptr);
    if(ret < 0){
        std::cerr << "Failed to load the video" << std::endl;
        return;
    }

    ret = avformat_find_stream_info(formatCtx, nullptr);
    if(ret < 0){
        std::cerr << "Failed to find stream info" << std::endl;
        return;
    }

    for ( int i = 0; i < formatCtx->nb_streams; i++) {
        if(formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStreamIndex1.push_back(*formatCtx->streams[i]);
        }
    }

    AVCodecParameters* codecpar = formatCtx->streams[videoStreamIndex1.at(0).index]->codecpar;
    const AVCodec* codec = avcodec_find_decoder(codecpar->codec_id);
    codec_ctx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codec_ctx, codecpar);

    if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
        std::cerr << "Could not open codec" << std::endl;
        return;
    }

    AVCodecParameters* codecpar2 = formatCtx->streams[videoStreamIndex1.at(1).index]->codecpar;
    const AVCodec* codec2 = avcodec_find_decoder(codecpar2->codec_id);
    codec_ctx2 = avcodec_alloc_context3(codec2);
    avcodec_parameters_to_context(codec_ctx2, codecpar2);

    if (avcodec_open2(codec_ctx2, codec2, nullptr) < 0) {
        std::cerr << "Could not open codec" << std::endl;
        return;
    }
}

void MainWindow::mergeVideoStreams(int stopAtOriginalVideoFrame, int outputVideoFps, const char* outPath)
{
    AVFrame* frame = av_frame_alloc();
    AVPacket packet;
    AVPacket packet2;
    AVFrame* rgbFrame = av_frame_alloc();
    cv::Mat frame1;
    cv::Mat frame2;
    cv::Mat frameRotated;
    // cv::Mat frameConcat;
    bool firstFrame = false;
    bool secondFrame = false;

    int i = 0;
    int ret;
    std::vector<cv::Mat> imageFrames{};

    av_init_packet(&packet);

    struct SwsContext* swsContext = sws_getContext(
        codec_ctx->width, codec_ctx->height, codec_ctx->pix_fmt,
        codec_ctx->width, codec_ctx->height, AV_PIX_FMT_BGR24,
        SWS_BILINEAR, nullptr, nullptr, nullptr);

    while(av_read_frame(formatCtx, &packet) >= 0){
        if(packet.stream_index == videoStreamIndex1.at(0).index && !firstFrame){
            ret = avcodec_send_packet(codec_ctx, &packet);
            if(ret < 0){
                std::cerr << "Error sending packet for decoding" << std::endl;
                break;
            }
            if(ret >= 0){
                ret = avcodec_receive_frame(codec_ctx, frame);
                if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
                    break;
                }
                else if(ret < 0){
                    std::cerr << "Error during decoding" << std::endl;
                    break;
                }

                qDebug() << "Stream 1: "
                         << "Frame " << codec_ctx->frame_number
                         << " (type=" << av_get_picture_type_char(frame->pict_type)
                         << ", size=" << frame->pkt_size
                         << " bytes) pts " << frame->pts
                         << " key_frame " << frame->key_frame;
                av_image_alloc(rgbFrame->data, rgbFrame->linesize, codec_ctx->width, codec_ctx->height, AV_PIX_FMT_BGR24, 1);
                sws_scale(swsContext, frame->data, frame->linesize, 0, codec_ctx->height, rgbFrame->data, rgbFrame->linesize);
                cv::Mat image1(codec_ctx->height, codec_ctx->width, CV_8UC3, rgbFrame->data[0]);
                image1.copyTo(frame1);
                cv::imwrite("frame1.jpg", frame1);

                firstFrame = true;
                i++;
                std::cout  << "i = " << i << std::endl;
            }
        }
        if(packet.stream_index == videoStreamIndex1.at(1).index && !secondFrame){
            ret = avcodec_send_packet(codec_ctx2, &packet);
            if(ret < 0){
                std::cerr << "Error sending packet for decoding" << std::endl;
                break;
            }
            if(ret >= 0){
                ret = avcodec_receive_frame(codec_ctx2, frame);
                if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
                    break;
                }
                else if(ret < 0){
                    std::cerr << "Error during decoding" << std::endl;
                    break;
                }

                qDebug() << "Stream 2: "
                         << "Frame " << codec_ctx2->frame_number
                         << " (type=" << av_get_picture_type_char(frame->pict_type)
                         << ", size=" << frame->pkt_size
                         << " bytes) pts " << frame->pts
                         << " key_frame " << frame->key_frame;
                av_image_alloc(rgbFrame->data, rgbFrame->linesize, codec_ctx2->width, codec_ctx2->height, AV_PIX_FMT_BGR24, 1);
                sws_scale(swsContext, frame->data, frame->linesize, 0, codec_ctx2->height, rgbFrame->data, rgbFrame->linesize);
                cv::Mat image1(codec_ctx2->height, codec_ctx2->width, CV_8UC3, rgbFrame->data[0]);
                image1.copyTo(frame2);
                cv::imwrite("frame2.jpg", frame2);

                secondFrame = true;
                i++;
                std::cout  << "i = " << i << std::endl;
            }
        }

        if(!frame1.empty() && !frame2.empty() && (codec_ctx2->frame_number == codec_ctx->frame_number) && firstFrame && secondFrame){
            cv::rotate(frame2,frameRotated, cv::ROTATE_90_CLOCKWISE);

            int rows1 = frame1.rows;
            int rows2 = frameRotated.rows;

            int cols1 = frame1.cols;
            int cols2 = frameRotated.cols;

            cv::Mat3b frameConcat(rows1 + rows2, cols1, cv::Vec3b(0,0,0));

            // Copy images in correct position
            frame1.copyTo(frameConcat(cv::Rect(0, 0, cols1, rows1)));
            frameRotated.copyTo(frameConcat(cv::Rect(cols2, rows1, cols2, rows2)));

            cv::imwrite("frameConcat.jpg", frameConcat);
            cv::imshow("frame2",frameConcat);
            cv::waitKey(20);
            imageFrames.push_back(frameConcat);
            firstFrame = false;
            secondFrame = false;
            std::cout << "IAM HERE!" << std::endl;

            if(i >= stopAtOriginalVideoFrame*2){
                av_packet_unref(&packet);
                break;
            }
        }

        av_packet_unref(&packet);
    }
    av_frame_free(&frame);
    avcodec_free_context(&codec_ctx);
    avcodec_free_context(&codec_ctx2);
    avformat_close_input(&formatCtx);

    cv::destroyAllWindows();

    cv::Size frame_size(imageFrames[0].cols, imageFrames[0].rows);
    cv::VideoWriter out_capture(outPath, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), outputVideoFps, frame_size, true);

    if (out_capture.isOpened() == false)
    {
        std::cout << "Cannot save the video to a file" << std::endl;
        return;
    }

    for(cv::Mat image : imageFrames){
        out_capture.write(image);
    }

    std::cout << "Successfully created video from image frames" << std::endl;
}

