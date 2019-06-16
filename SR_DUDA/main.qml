import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

Window
{
    visible: true
    width: 640
    height: 480
    title: qsTr("Ricart-Agrawala")

    Dialog
    {
        id: dialog_WrongIP
        title: "Error"
        Text { text: "You provided wrong IP in textfield!" }
    }

    Dialog
    {
        id: dialog_Connect_to
        title: "Connection settings"
        property bool ourIp: false

        function checkIP(ip)
        {
            var dots = 0;
            var digits = 0;
            var providePort = false;

            for (var i = 0; i < ip.length; i++)
            {
                if (!isNaN(ip[i]))
                    digits++;

                else if (ip[i] === '.' && !providePort)
                {
                    dots++;

                    if (digits === 0 || digits > 3 || dots > 3)
                        return false;

                    digits = 0;
                }

                else if (ip[i] === ':' && !providePort)
                    providePort = true;

                else
                    return false;
            }

            if (dots !== 3 || (digits > 3 && !providePort))
                return false;

            else if (!providePort)
                return false;

            return true;
        }

        GridLayout
        {
            id: grid
            columns: 2

            Label
            {
                text: "Address:"
            }

            TextField
            {
                id: ipField
                placeholderText: "123.45.67.89:1111"
            }
        }

        onAccepted:
        {
            if (ipField.text.length != 0 && !checkIP(ipField.text))
            {
                ipField.text = ""
                dialog_WrongIP.open()
            }

            else
            {
                if (!ourIp)
                    GuiController.connectTo(ipField.text)

                else
                    GuiController.setOurIp(ipField.text)
            }
        }
    }

    Dialog
    {
        id: networkStatusDialog

        Label
        {
            id: textLabel
        }

        onVisibleChanged:
        {
            if (GuiController.networkGood)
                textLabel.text = "Ready to work."

            else
                textLabel.text = "Not ready to work."
        }
    }

    MenuBar
    {
        Menu
        {
            title: "Connections"

            MenuItem
            {
                text: "Connect to ..."
                onClicked:
                {
                    dialog_Connect_to.ourIp = false
                    dialog_Connect_to.open()
                }
            }

            MenuItem
            {
                text: "Set ip and port"
                onClicked:
                {
                    dialog_Connect_to.ourIp = true;
                    dialog_Connect_to.open()
                }
            }

            MenuItem
            {
                text: "Network status"
                onClicked: networkStatusDialog.open()
            }
        }
    }

    Item
    {
        anchors.fill: parent

        Label
        {
            id: statusLabel
            text: GuiController.status
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 70
        }

        Button
        {
            id: enterBtn
            text: "Enter the critical section"
            anchors.top: statusLabel.bottom
            anchors.topMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            enabled: GuiController.btnEnable

            property bool inCritilacSection: GuiController.inCritical

            onInCritilacSectionChanged:
            {
                if (!inCritilacSection)
                    text = "Enter the critical section"

                else
                    text = "Leave the critical section"
            }

            onClicked: GuiController.btnClicked()
        }

        Rectangle
        {
            anchors.top: enterBtn.bottom
            anchors.topMargin: 20
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            color: "transparent"
            border.color: "black"

            ListView
            {
                id: devicesList

                property int argsNumber: 3

                anchors.fill: parent
                model: GuiController.model
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
                        model:[qsTr("IP address"), qsTr("Port"), qsTr("Timestamp")]

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
                        Text { text: model.modelData.IP; width: parent.width / devicesList.argsNumber }
                        Text { text: model.modelData.port; width: parent.width / devicesList.argsNumber }
                        Text { text: model.modelData.timestamp; width: parent.width / devicesList.argsNumber }
                    }

                    onClicked:
                    {
                        devicesList.currentIndex = index
                    }
                }
            }
        }
    }
}
