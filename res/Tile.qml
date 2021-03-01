import QtQuick 2.9

Rectangle {
    property string pImageData: ""
    property string pImageEquality: ""

    width: grid.cellWidth - 5
    height: grid.cellHeight - 5
    border.color: "lightgrey"
    border.width: 1
    radius: 5

    Image {
        anchors.fill: parent
        source: pImageData
    }

    Rectangle {
        anchors.fill: parent
        color: "lightgrey"
        opacity: 0.5
    }

    Text {
        text: qsTr(pImageEquality)
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }
}
