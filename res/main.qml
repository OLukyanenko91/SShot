import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import Components 1.0

ApplicationWindow {
    id: root
    width: 610
    minimumWidth: 500
    height: 480
    visible: true
    title: qsTr("SShot")

    Item {
        anchors.fill: parent

        GridView {
            id: grid
            anchors.fill: parent
            anchors.margins: 5
            cellWidth: 200
            cellHeight: 100

            model: Model {
                id: model
            }

            delegate: Tile {
                pImageData: imageData
                pImageEquality: imageEquality
            }
        }

        Timer {
            id: timer
            running: false
            interval: 2000
            repeat: true

            onTriggered: {
                console.debug("Timer triggered");
                model.increase()
            }
        }

        Item {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            width: 400
            height: 100

            Rectangle {
                anchors.fill: parent
                color: "green"
                radius: 20
                border.width: 2
                border.color: "lightgrey"
                opacity: 1
            }

            Button {
                text: "START"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 80
                width: 100

                onClicked: {
                    console.debug("Timer start");
                    timer.start();
                }
            }

            Button {
                text: "STOP"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 80
                width: 100

                onClicked: {
                    console.debug("Timer stop");
                    timer.stop();
                }
            }
        }
    }

    onClosing: {
        console.debug("Window closing")
        model.save();
    }
}
