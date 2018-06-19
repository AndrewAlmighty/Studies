import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Berkeley_algorithm")

    Loader
    {
        id: windowLoader
        anchors.fill: parent
        source: "RunWindow.qml"
    }
}
