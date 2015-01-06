import QtQuick 1.1

// rectangle for description texts
Rectangle {
    property alias text: textBox.text
    property alias continueVisible: continueIcon.visible
    property int innerWidth;
    signal clicked

    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 100
    width: textBox.width + 30
    height: textBox.height + 8
    border.color: "black"
    border.width: 3
    smooth: true
    //opacity: 0.9
    z: 10000
    color: "#FF707070"

    Image {
        id: continueIcon
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        source: "qrc:/MO/gui/next"

        SequentialAnimation on opacity {
            loops: Animation.Infinite

            PauseAnimation { duration: 500 }
            PropertyAnimation { easing.type: Easing.InOutSine; duration: 400; to: 0.0 }
            PropertyAnimation { easing.type: Easing.OutInSine; duration: 400; to: 1.0 }
        }
    }

    Text {
        id: textBox
        text: ""
        font.pointSize: 12
        font.bold: false
        width: innerWidth
        font.family: "Courier"
        wrapMode: Text.WordWrap
        anchors.centerIn: parent
    }

    MouseArea {
        id: clickArea
        anchors.fill: parent
        onClicked: parent.clicked(mouse)
    }
}
