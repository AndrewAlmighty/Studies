import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Item
{
    id: setupMainItem
    anchors.fill: parent
    anchors.margins: 5

    Label
    {
        id: runModeLabel
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 10

        text: qsTr("Run mode:")
    }

    RadioButton
    {
        id: serverOption
        anchors.left: runModeLabel.right
        anchors.leftMargin: 50
        anchors.verticalCenter: runModeLabel.verticalCenter
        text: qsTr("Server")
        checked: true
    }

    RadioButton
    {
        id: clientOption
        anchors.left: serverOption.right
        anchors.leftMargin: 50
        anchors.verticalCenter: runModeLabel.verticalCenter
        text: qsTr("Client")
    }

    Label
    {
        id: searchOptionLabel
        anchors.top: runModeLabel.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 10
        visible: false
        text: qsTr("Search for server:")
    }

    RadioButton
    {
        id: ipOption
        anchors.verticalCenter: searchOptionLabel.verticalCenter
        x: serverOption.x
        visible: false
        text: qsTr("Direct IP")
        checked: true
    }

    RadioButton
    {
        id: lookOption
        anchors.verticalCenter: searchOptionLabel.verticalCenter
        x: clientOption.x
        visible: false
        text: qsTr("Look for servers")
    }

    Loader
    {
        id: clientOptLoader
        visible: false
        anchors.top: searchOptionLabel.bottom
        anchors.topMargin: 30
        width: parent.width
        sourceComponent: directIP_component
    }

    Component
    {
        id: directIP_component

        DirectIP
        {
            id: serverIPItem
        }
    }

    Component
    {
        id: listIP_component

        DevicesList
        {
            id:devicesList
        }
    }

    Button
    {
        id: runButton
        text: "Run"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked:
        {
           GuiManager.beginJob()
            if(GuiManager.running)
                windowLoader.source = "RunWindow.qml"
        }
    }

    Connections
    {
        target: serverOption
        onClicked:
        {
            searchOptionLabel.visible = false
            ipOption.visible = false
            lookOption.visible = false
            clientOptLoader.visible = false
            GuiManager.setMode("Server")
            GuiManager.setServerIP("127.0.0.1")
        }
    }

    Connections
    {
        target: clientOption
        onClicked:
        {
            searchOptionLabel.visible = true
            ipOption.visible = true
            lookOption.visible = true
            clientOptLoader.visible = true
            GuiManager.setMode("Client")
        }
    }

    Connections
    {
        target: lookOption
        onClicked:
        {
            clientOptLoader.anchors.bottom = runButton.top
            clientOptLoader.anchors.bottomMargin = 10
            clientOptLoader.sourceComponent = listIP_component
        }
    }

    Connections
    {
        target: ipOption
        onClicked:
        {
            clientOptLoader.anchors.bottom = undefined
            clientOptLoader.height = 20
            clientOptLoader.sourceComponent = directIP_component
        }
    }

    ButtonGroup
    {
        buttons: [serverOption, clientOption]
    }

    ButtonGroup
    {
        buttons: [ipOption, lookOption]
    }
}
