import QtQuick 2.5
import QtQuick.Controls 2.1
import QtMultimedia 5.5
import FilterLib 1.0
import QtQuick.Window 2.12
import QtPositioning 5.2
import QtSensors 5.0

ApplicationWindow {
    id: mainscreen
    visible: true
    color: "black"
    //visibility: "FullScreen"

    property string text

    signal qmlSignal(string msg)

    Camera {
        id: camera
        focus.focusMode: FocusMacro

        //Component.onCompleted: camera.stop()
        // To Do - automatically choose resolutions
        viewfinder.resolution: Qt.size( 1920,1080 );

    }


    VideoOutput {
        id: videoOutput
        //orientation:
        opacity: 1.0
        anchors.fill: parent   
        fillMode: VideoOutput.PreserveAspectCrop //cropp image to fit the screen
        autoOrientation: true
        source: camera
        filters: [ videoFilter ]
    }

    Filter {
        id: videoFilter
        orientation: videoOutput.orientation
        faceid:parseInt(facenumber.text)
        onFaceidChanged: console.log("faceid changed:", faceid)
        onClicked: console.log("onClicked!")
    }

    Timer {
        interval: 1000; running: true; repeat: true
        onTriggered: {if (camera.lockStatus == Camera.Unlocked)
                      {
                        camera.searchAndLock();
                      }
                      else{
                        camera.unlock();
                        camera.searchAndLock();
                      }

                      }
    }


    //some debug information, once in a timer
    Timer {
        id:debugtimer
        interval: 10000; running: true; repeat: true
        onTriggered: {
                        console.log("camera orientation", camera.orientation)
                        console.log("videooutput orientation",videoOutput.orientation)
                      //Printing all the resolutions
                        console.log("number of viewfinder resolutions ",camera.supportedViewfinderResolutions().length);
                        for (var i = 0; i < camera.supportedViewfinderResolutions().length; i++) {
                             console.log("resolution № ",i,camera.supportedViewfinderResolutions()[i].width.toString()+"x"+camera.supportedViewfinderResolutions()[i].height.toString());
                        }



        }
    }

     //focus on tapping the screen
     FocusButton {
         id:focusing
        enabled: false
         width : parent.width
         height: parent.height*0.7
         camera:  camera
         visible: camera.cameraStatus == Camera.ActiveStatus && camera.focus.isFocusModeSupported(Camera.FocusAuto)
         Component.onCompleted:  focusing.enabled = true
     }



   onClosing: {

   }


}
