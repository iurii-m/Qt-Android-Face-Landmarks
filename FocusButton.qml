import QtQuick 2.0
import QtMultimedia 5.0

Item {
    id: button
    property Camera camera

    opacity: 0.0

    property string text

    signal clicked


    Rectangle {
        id:buttonRect
        color: "white"
        width: button.width
        height: button.height
    }

    MouseArea {
        id: mouseRegion
        anchors.fill: buttonImage
        onClicked: { button.clicked(); }
              }

    onClicked: {
         if (camera.lockStatus == Camera.Unlocked)
             camera.searchAndLock();
         else
             camera.unlock();
             camera.searchAndLock();
               }

}
