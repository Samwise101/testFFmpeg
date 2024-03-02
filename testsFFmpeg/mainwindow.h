#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <vector>
// FFmpeg
extern "C" {
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
#include "libavcodec/avcodec.h"
#include <libavutil/opt.h>
#include <libavdevice/avdevice.h>
#include <libavutil/opt.h>
#include <libavutil/time.h>
#include <libavformat/avio.h>
#include <libavutil/avstring.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadVideo(const char *path);
    void mergeVideoStreams(int stopAtOriginalVideoFrame, int outputVideoFp, const char* outPath);

    std::vector<AVStream> videoStreamIndex1;
    AVFormatContext *formatCtx = nullptr;
    AVCodec* codec = nullptr;
    AVCodecContext* codec_ctx = nullptr;
    AVCodecContext* codec_ctx2 = nullptr;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
