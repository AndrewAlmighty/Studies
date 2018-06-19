import QtQuick 2.0
import QtQuick.Controls 2.2

Item
{
    id: runWindow
    anchors.fill: parent
    anchors.margins: 5

    Label
    {
        id: timeLabel
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20
        text: "00:00:00"
    }

    Label
    {
        id: whichModeLabel
        anchors.top: timeLabel.bottom
        anchors.left: parent.left
        anchors.topMargin: 20
        text: "Running as:"
    }

    Label
    {
        id: modeLabel
        anchors.left: whichModeLabel.right
        anchors.leftMargin: 10
        anchors.verticalCenter: whichModeLabel.verticalCenter
        text: "Server"
    }

    Button
    {
        id: abortButton
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 10
        text: "Abort"
    }
}
