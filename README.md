# Qt-Android-Face-Landmarks
A simple template of QT QML Camera Android application for demonstrating OpenCV and DLib Functionality.
Face is detected with OpenCV lbp Cascade CLassifier.
Landmarks are detected with Dlib Shape detector.

Frames are processed in a thread which is implemented with QThread. The way QThread was employed is not the best for that purpose and used just for demonstrating.

OpenCV with contrib prebuilt for Android
https://pullrequest.opencv.org/buildbot/export/opencv_releases/master-contrib_pack-contrib-android/  

Dlib prebuilt for Android
https://github.com/Luca96/dlib-for-android
