import QtQuick 2.9

Rectangle {
    property string pImageData: ""

    width: grid.cellWidth - 5
    height: grid.cellHeight - 5
    border.color: "lightgrey"
    border.width: 1
    radius: 5

    Image {
        anchors.fill: parent
        source: pImageData
    }
}
