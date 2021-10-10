import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    property string pImageData: ""

    id: root
    anchors.fill: parent
    border.width: 2
    border.color: "lightgrey"

    Image {
        anchors.fill: parent
        anchors.margins: 2
        source: pImageData
    }

    Item {
        id: background
        width: 400
        height: 100
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20

        Rectangle {
            color: "green"
            anchors.fill: parent
            opacity: 0.9
            border.width: 2
            border.color: "lightgrey"
            radius: 20
        }

        Button {
            anchors.centerIn: background
            text: "Close"
            width: 100

            onPressed: root.opacity = 0
        }
    }


    Behavior on opacity {
        NumberAnimation { duration: 250 }
    }
}
