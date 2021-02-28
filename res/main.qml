import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import Components 1.0

Window {
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

            delegate: Tile {}
        }

        Timer {
            id: timer
            running: false
            interval: 1000
            repeat: true

            onTriggered: model.makeScreenshot()
        }

        Item {
            anchors.fill: parent
            anchors.leftMargin: root.width / 10
            anchors.rightMargin: root.width / 10
            anchors.bottomMargin: 10
            anchors.topMargin: root.height - 100

            Rectangle {
                anchors.fill: parent
                color: "gray"
                radius: 20
                opacity: 0.8
            }

            Button {
                text: "START"
                anchors.bottom: root.bottom
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 100

                onClicked: timer.start()
            }

            Button {
                text: "STOP"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 100

                onClicked: timer.stop()
            }
        }
    }
}
