# Qt-Android-Face-Landmarks
A simple template of QT QML Camera Android application for demonstrating OpenCV and DLib Functionality.
Face is detected with OpenCV lbp Cascade CLassifyer.
Landmarks are detected with Dlib Shape detector.

Frames ae processed in thread which is implemented with QThread. QThread is not the best choise for that goal and used just for demonstrating.

OpenCV with contrib prebuilt for Android
https://pullrequest.opencv.org/buildbot/export/opencv_releases/master-contrib_pack-contrib-android/  

Dlib prebuilt for Android
https://github.com/Luca96/dlib-for-android