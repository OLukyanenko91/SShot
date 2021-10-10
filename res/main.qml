import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: root
    width: 810
    height: 600
    minimumWidth: 500
    minimumHeight: 400
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

            model: viewModel

            delegate: Tile {
                pTileIndex: index
                pImageData: imageData
                pImageEquality: imageEquality
            }
            remove: Transition {
                ParallelAnimation {
                    NumberAnimation { property: "opacity"; to: 0; duration: 500 }
                    NumberAnimation { property: "scale"; to: 0; duration: 500 }
                }
            }
            add: Transition {
                ParallelAnimation {
                    NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 500 }
                    NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 500 }
                }
            }
            move: Transition {
                NumberAnimation { properties: "x,y"; duration: 500 }
            }
            displaced: Transition {
                NumberAnimation { properties: "x,y"; duration: 500 }
            }
            populate: Transition {
                NumberAnimation { properties: "x,y"; duration: 500 }
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
                opacity: 0.9
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

        Popup {
            property string popUpMessage: ""

            function setMessage(message) {
                console.log("Main::Popup::setMessage::" + message);
                popUpMessage = message;
            }

            id: popup
            width: 150
            height: 150
            modal: true
            closePolicy: Popup.NoAutoClose
            anchors.centerIn: parent

            background: Rectangle {
                border.color: "lightgreen"
                border.width: 4
                radius: 20
            }

            contentItem: ColumnLayout {
                BusyIndicator {
                    id: indicator
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.margins: 10

                    contentItem: Item {
                        id: indicatorElement
                        x: parent.width / 2 - 32
                        y: parent.height / 2 - 32
                        width: 64
                        height: 64
                        opacity: indicator.running ? 1 : 0

                        Behavior on opacity {
                            OpacityAnimator {
                                duration: 250
                            }
                        }

                        RotationAnimator {
                            target: indicatorElement
                            running: indicator.visible && indicator.running
                            from: 0
                            to: 360
                            loops: Animation.Infinite
                            duration: 2000
                        }

                        Repeater {
                            id: repeater
                            model: 10

                            Rectangle {
                                x: indicatorElement.width / 2 - width / 2
                                y: indicatorElement.height / 2 - height / 2
                                implicitWidth: 10
                                implicitHeight: 10
                                radius: 5
                                color: "#21be2b"
                                transform: [
                                    Translate {
                                        y: -Math.min(indicatorElement.width, indicatorElement.height) * 0.5 + 5
                                    },
                                    Rotation {
                                        angle: index / repeater.count * 360
                                        origin.x: 5
                                        origin.y: 5
                                    }
                                ]
                            }
                        }
                    }
                }

                Text {
                    text: popup.popUpMessage
                    Layout.alignment: Qt.AlignCenter
                }
            }
        }

        Timer {
            id: timer
            running: false
            interval: 2000
            repeat: true

            onTriggered: {
                console.debug("Main::Timer::onTriggered");
                backendModel.makeScreenshot();
            }
        }
    }

    FullscreenTile {
        id: fullscreenTile
        opacity: 0
    }

    Connections{
        target: backendModel

        function onAddUIScreenshot(screenshot) {
            console.debug("Main::backendModel::onAddUIScreenshot)")

            viewModel.insert(0, {index: "0",
                                        imageEquality: screenshot.equality,
                                        imageData: screenshot.imageStr})
        }

        function onLoaded(screenshots) {
            console.debug("Main::backendModel::onReady()")

            for (var index in screenshots) {
                viewModel.append({index: index,
                                         imageEquality: screenshots[index].equality,
                                         imageData: screenshots[index].imageStr})
            }

            popup.close()
        }

        function onSaved() {
            Qt.quit();
        }
    }

    ListModel{
        function updateTileIndexes() {
            console.debug("Main::screenshotsModel::updateTileIndexes()")

            var currIndex = -1;
            while(++currIndex <= count - 1)
                viewModel.setProperty(currIndex, "index", currIndex.toString())
        }

        id: viewModel
    }

    Component.onCompleted: {
        console.debug("Main::ApplicationWindow::onCompleted()");

        popup.open();
        popup.setMessage("Loading...");
        backendModel.load();
    }

    onClosing: {
        console.debug("Main::ApplicationWindow::onClosing()");

        close.accepted = false;
        popup.open();
        popup.setMessage("Saving...");
        backendModel.save();
    }
}
