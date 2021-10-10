import QtQuick 2.9
import QtGraphicalEffects 1.0

Rectangle {
    property string pImageData: ""
    property string pImageEquality: ""
    property int pTileIndex: -1

    width: grid.cellWidth - 5
    height: grid.cellHeight - 5
    border.color: "lightgreen"
    border.width: 1
    radius: 5

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onEntered: highlight.show()
        onExited: highlight.hide()
        onClicked: {
            fullscreenTile.pImageData = pImageData;
            fullscreenTile.opacity = 1;
        }
    }

    Image {
        anchors.fill: parent
        anchors.margins: 1
        source: pImageData
    }

    Item {
        id: highlight

        function hide() {
            highlightRect.opacity = 0;
            closeImage.opacity = 0;
        }

        function show() {
            highlightRect.opacity = 0.5;
            closeImage.opacity = 1;
        }

        anchors.fill: parent

        Rectangle {
            id: highlightRect
            anchors.fill: parent
            color: "lightgrey"
            opacity: 0

            Behavior on opacity {
                NumberAnimation { duration: 250 }
            }
        }

        Image {
            id: closeImage
            height: 25
            width: 25
            smooth: true
            mipmap: true
            anchors.right: parent.right
            source: "images/close.png"
            opacity: 0

            Behavior on opacity {
                NumberAnimation { duration: 250 }
            }
        }

        ColorOverlay {
            id: closeImageOverlay
            anchors.fill: closeImage
            source: closeImage
            color: "#80ff0000"
            opacity: 0

            Behavior on opacity {
                NumberAnimation { duration: 250 }
            }
        }

        MouseArea {
            anchors.fill: closeImage
            hoverEnabled: true

            onClicked: {
                viewModel.remove(pTileIndex)
                viewModel.updateTileIndexes()
                backendModel.removeScreenshot(pTileIndex)
            }
            onEntered: {
                highlight.show()
                closeImageOverlay.opacity = 1
            }
            onExited: {
                highlight.hide()
                closeImageOverlay.opacity = 0
            }
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

}
