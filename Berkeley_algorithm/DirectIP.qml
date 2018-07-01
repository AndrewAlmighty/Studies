import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Item
{
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.leftMargin: 10

    RowLayout
    {
        Label
        {
            text: qsTr("Server IP: ")
        }

        TextField
        {
            id: serverIPField
            onEditingFinished:
            {
                GuiManager.setServerIP(serverIPField.getText(0, serverIPField.length))
            }
        }
    }
}
