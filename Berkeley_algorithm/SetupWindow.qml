import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Item
{
    id: setupMainItem
    anchors.fill: parent
    anchors.margins: 5

    RowLayout
    {
        id: modeLayout
        anchors.top: parent.top
        anchors.topMargin: 20
        width: parent.width
        height: 30

        Label
        {
            id: runModeLabel
            text: qsTr("Run mode:")
        }

        RadioButton
        {
            id: serverOption
            text: qsTr("Server")
            checked: true
        }

        RadioButton
        {
            id: clientOption
            text: qsTr("Client")
        }
    }

    RowLayout
    {
        id: searchOptions
        visible: false
        anchors.top: modeLayout.bottom
        anchors.topMargin: 20
        width: parent.width

        Label
        {
            id: searchOptionLabel
            text: qsTr("Search for server:")
        }

        RadioButton
        {
            id: ipOption
            text: qsTr("Direct IP")
            checked: true
        }

        RadioButton
        {
            id: lookOption
            text: qsTr("Look for servers")
        }
    }

    Loader
    {
        id: clientOptLoader
        visible: false
        anchors.top: searchOptions.bottom
        anchors.topMargin: 30
        width: parent.width
        sourceComponent: directIP_component
    }

    Component
    {
        id: directIP_component
        DirectIPOption
        {
            id: serverIPItem
        }
    }

    Component
    {
        id: listIP_component
        IpList
        {
            id:ipList
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
            searchOptions.visible = false
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
            searchOptions.visible = true
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
}
