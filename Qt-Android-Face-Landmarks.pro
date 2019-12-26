QT += qml quick multimedia widgets

CONFIG += c++11

SOURCES += main.cpp \
    Filter.cpp \
    threadprocessor.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Filter.h \
    threadprocessor.h

android {
# opencv sdk folder. I use here opencv 3.4.3 compiled with NDK r17c
    OPENCV_PATH =C:/OpenCV/OpenCV-android-sdk-3.4.3

ANDROID_EXTRA_LIBS += $$OPENCV_PATH/sdk/native/libs/armeabi-v7a/libopencv_java3.so
INCLUDEPATH += "$$OPENCV_PATH/sdk/native/jni/include"


# 3rd party libs !!! very important  to keep the correct order(according compilind order in makefiles)
LIBS += \
   -L"$$OPENCV_PATH/sdk/native/3rdparty/libs/armeabi-v7a"\
   -ltbb\
   -lcpufeatures\
   -llibprotobuf\
   -llibjpeg-turbo\
   -llibwebp\
   -llibpng\
   -llibtiff\
   -llibjasper\
   -lIlmImf\
   -ltegra_hal

LIBS += \
    -L"$$OPENCV_PATH/sdk/native/libs/armeabi-v7a"\
    -lopencv_java3

# opencv libs
LIBS += \
    -L"$$OPENCV_PATH/sdk/native/staticlibs/armeabi-v7a"\
    -lopencv_dnn\
    -lopencv_ml\
    -lopencv_objdetect\
    -lopencv_photo\
    -lopencv_stitching\
    -lopencv_video\
    -lopencv_calib3d\
    -lopencv_features2d\
    -lopencv_highgui\
    -lopencv_flann\
    -lopencv_videoio\
    -lopencv_imgcodecs\
    -lopencv_imgproc\
    -lopencv_core\
    -lopencv_face


DLIB_PATH =C:/DLib_Android
INCLUDEPATH += "$$DLIB_PATH/include"
ANDROID_EXTRA_LIBS += $$DLIB_PATH/prebuilt/armeabi-v7a/libdlib.so
}


DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    lbpcascade_frontalface_improved.xml \
    face_landmarks_68.dat

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}

