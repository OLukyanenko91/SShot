import QtQuick 2.9

Rectangle {
    property string pImageData: ""
    property string pImageEquality: ""

    width: grid.cellWidth - 5
    height: grid.cellHeight - 5
    border.color: "lightgreen"
    border.width: 1
    radius: 5

    Image {
        anchors.fill: parent
        anchors.margins: 1
        source: pImageData
    }

    Rectangle {
        id: highlight
        anchors.fill: parent
        color: "lightgrey"
        opacity: 0

        Behavior on opacity {
            NumberAnimation { duration: 250 }
        }
    }

    Rectangle {
        id: rect
        width: 50
        height: 25
        radius: 10
        color: "lightgreen"
        opacity: 0.7
        anchors.centerIn: parent

        Text {
            id: equality
            text: qsTr(pImageEquality)
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onEntered: highlight.opacity = 0.5
        onExited: highlight.opacity = 0
        onClicked: {
            fullscreenTile.pImageData = pImageData;
            fullscreenTile.opacity = 1;
        }
    }
}
