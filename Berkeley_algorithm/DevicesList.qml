import QtQuick 2.9
import QtQuick.Controls 2.2

Rectangle
{
    anchors.fill: parent
    color: "transparent"
    border.color: "black"

    ListView
    {
        id: devicesList

        property int argsNumber: 4

        anchors.fill: parent
        model: jakismodel
        highlight: Rectangle { color: "lightsteelblue"}
        highlightFollowsCurrentItem: true
        headerPositioning: ListView.OverlayHeader
        clip: true

        header: Row
        {
            id: header
            width: parent.width
            z: 2

            Repeater
            {
                id: headerRep
                model:[qsTr("ID"), qsTr("IP address"), qsTr("MAC Address"), qsTr("Mode")]

                Rectangle
                {
                    width: header.width / headerRep.count
                    height: 30
                    border.color: "black"
                    border.width: 2

                    Text
                    {
                        anchors.centerIn: parent
                        text: modelData
                    }
                }
            }
        }

        delegate: MouseArea
        {
            width: devicesList.width
            height: 20
            Row
            {
                anchors.fill: parent
                Text { text: ID; width: parent.width / devicesList.argsNumber }
                Text { text: IP; width: parent.width / devicesList.argsNumber }
                Text { text: MAC; width: parent.width / devicesList.argsNumber }
                Text { text: Mode; width: parent.width / devicesList.argsNumber }
            }

            onClicked:
            {
                devicesList.currentIndex = index
            }
        }
    }

    ListModel
    {
        id:pustymodel
    }

    ListModel
    {
        id: jakismodel
        ListElement
        {
            ID: 1
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"

        }

        ListElement
        {
            ID: 2
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }

        ListElement
        {
            ID: 3
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }

        ListElement
        {
            ID: 4
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }

        ListElement
        {
            ID: 5
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }

        ListElement
        {
            ID: 6
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }

        ListElement
        {
            ID: 7
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }

        ListElement
        {
            ID: 8
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }

        ListElement
        {
            ID: 9
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }

        ListElement
        {
            ID: 10
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }

        ListElement
        {
            ID: 10
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }

        ListElement
        {
            ID: 10
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }

        ListElement
        {
            ID: 10
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }

        ListElement
        {
            ID: 10
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }

        ListElement
        {
            ID: 10
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }

        ListElement
        {
            ID: 10
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }

        ListElement
        {
            ID: 10
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }

        ListElement
        {
            ID: 10
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }

        ListElement
        {
            ID: 10
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }

        ListElement
        {
            ID: 10
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }

        ListElement
        {
            ID: 10
            IP: "12.43.65.87"
            MAC: "78:32:1b:03:3a:71"
            Mode: "Server"
        }
    }
}
