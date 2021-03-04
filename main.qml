import QtQuick 2.12
import QtQuick.Window 2.12

Window {//root element
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Puzzle")


    GameBoard {
        id: _gameBoard
        anchors.fill: parent
        anchors.margins: 5
    }
}
