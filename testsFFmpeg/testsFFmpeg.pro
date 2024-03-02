QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17



INCLUDEPATH += C:/FFMpeg/installed/x64-windows/include/

LIBS += -LC:/FFMpeg/installed/x64-windows/lib -lavformat
LIBS += -LC:/FFMpeg/installed/x64-windows/lib -lavcodec
LIBS += -LC:/FFMpeg/installed/x64-windows/lib -lavutil
LIBS += -LC:/FFMpeg/installed/x64-windows/lib -lswscale

LIBS += -LC:/FFMpeg/installed/x64-windows/bin
LIBS += -LC:/FFMpeg/installed/x64-windows/lib

# OpenCV
win32 {
    INCLUDEPATH += C:/opencv_vc16/include/

    LIBS +=-LC:/opencv_vc16/bin
    LIBS +=-LC:/opencv_vc16/lib

    CONFIG(debug, debug|release) {
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_core440d
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_highgui440d
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_imgcodecs440d
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_imgproc440d
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_features2d440d
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_calib3d440d
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_videoio440d
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_ml440d
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_dnn440d
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_flann440d
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_objdetect440d
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_photo440d
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_shape440d
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_video440d
    }
    else {
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_core440
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_highgui440
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_imgcodecs440
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_imgproc440
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_features2d440
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_calib3d440
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_videoio440
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_ml440
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_dnn440
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_flann440
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_objdetect440
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_photo440
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_shape440
        LIBS +=-LC:/opencv_vc16/lib/ -lopencv_video440
    }
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
