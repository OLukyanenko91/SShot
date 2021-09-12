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

    Rectangle {
        id: buttonBackground
        width: 90
        height: 70
        color: "lightgreen"
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 10
        opacity: 0.9
        radius: 10
    }

    Button {
        anchors.centerIn: buttonBackground
        text: "Close"

        onPressed: root.opacity = 0
    }

    Behavior on opacity {
        NumberAnimation { duration: 250 }
    }
}
