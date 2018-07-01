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
        model: GuiManager.model
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
                Text { text: model.modelData.ID; width: parent.width / devicesList.argsNumber }
                Text { text: model.modelData.IP; width: parent.width / devicesList.argsNumber }
                Text { text: model.modelData.MAC; width: parent.width / devicesList.argsNumber }
                Text { text: model.modelData.Mode; width: parent.width / devicesList.argsNumber }
            }

            onClicked:
            {
                devicesList.currentIndex = index
            }
        }
    }
}
