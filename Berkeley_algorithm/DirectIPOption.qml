import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Item
{
    anchors.top: parent.top
    width: parent.width

    RowLayout
    {
        anchors.fill: parent

        Label
        {
            text: qsTr("Server IP: ")
        }

        TextField
        {

        }
    }
}
