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
        font.pixelSize: 30
        text: GuiManager.time
    }

    Label
    {
        id: whichModeLabel
        anchors.top: timeLabel.bottom
        anchors.left: parent.left
        anchors.topMargin: 40
        text: "Running as:"
    }

    Label
    {
        id: modeLabel
        anchors.left: whichModeLabel.right
        anchors.leftMargin: 10
        anchors.verticalCenter: whichModeLabel.verticalCenter
        text: GuiManager.mode
    }

    Label
    {
        id: myIPLabel
        anchors.left: parent.left
        anchors.top: whichModeLabel.bottom
        anchors.topMargin: 10
        text: "Device IP:"
    }

    Label
    {
        id: myIP
        anchors.left: myIPLabel.right
        anchors.verticalCenter: myIPLabel.verticalCenter
        anchors.leftMargin: 10
        text: GuiManager.ip
    }

    Label
    {
        id: myMACLabel
        anchors.left: parent.left
        anchors.top: myIPLabel.bottom
        anchors.topMargin: 10
        text: "Device MAC:"
    }

    Label
    {
        id: myMAC
        anchors.left: myMACLabel.right
        anchors.verticalCenter: myMACLabel.verticalCenter
        anchors.leftMargin: 10
        text: GuiManager.MAC
    }

    Label
    {
        id: programIDLabel
        anchors.left: parent.left
        anchors.top: myMACLabel.bottom
        anchors.topMargin: 10
        text: "ID:"
    }

    Label
    {
        id: programID
        anchors.left: programIDLabel.right
        anchors.verticalCenter: programIDLabel.verticalCenter
        anchors.leftMargin: 10
        text: GuiManager.ID
    }

    Label
    {
        id: setTimeLabel
        anchors.right: setTimeTextField.left
        anchors.rightMargin: 10
        anchors.verticalCenter: modeLabel.verticalCenter
        text: "Set time to:"
    }

    TextField
    {
        id: setTimeTextField
        anchors.right: parent.right
        anchors.verticalCenter: modeLabel.verticalCenter
        placeholderText: "hh:mm:ss"
    }

    Button
    {
        id: setTimeButton
        anchors.top: setTimeTextField.bottom
        anchors.right: parent.right
        anchors.margins: 15
        text: "Set time"

        onClicked:
        {
            GuiManager.changeClock(setTimeTextField.getText(0, setTimeTextField.length))
            setTimeTextField.text = "hh:mm:ss"
        }
    }

    Button
    {
        id: abortButton
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 10
        text: "Abort"
        onClicked:
        {
            GuiManager.finishJob()
            windowLoader.source = "SetupWindow.qml"
        }
    }
}
