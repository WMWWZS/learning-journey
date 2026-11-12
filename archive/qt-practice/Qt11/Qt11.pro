QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# opencv路径配置
OPENCV_ROOT = E:/opencv/build/opencv
# Opencv头文件
INCLUDEPATH += $${OPENCV_ROOT}/include
#OpenCv库文件
LIBS += -L$${OPENCV_ROOT}/x64/mingw/lib

# 必选核心库（基础功能+视频读写，依赖 FFmpeg）
LIBS += -lopencv_core4100       # 核心模块（内存管理、数据结构）
LIBS += -lopencv_imgproc4100    # 图像处理（滤波、缩放等）
LIBS += -lopencv_videoio4100    # 视频 I/O（调用 FFmpeg 读写视频）
LIBS += -lopencv_highgui4100    # 简单 GUI 显示（如 imshow）
LIBS += -lopencv_imgcodecs4100  # 图像编解码（读写图片）
# 可选扩展库（根据项目需求添加）
LIBS += -lopencv_features2d4100 # 特征检测（如 SIFT/SURF）
LIBS += -lopencv_calib3d4100    # 相机标定、3D 重建
LIBS += -lopencv_objdetect4100  # 目标检测（如 Haar 级联）

DESTDIR = $$PWD/bin