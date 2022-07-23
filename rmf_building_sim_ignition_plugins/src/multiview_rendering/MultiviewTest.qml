import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2


Window {
    // title: qsTr("Multi-view display")
    id: testview
    x: 100
    y: 100
    readonly property int defaultWidth: 1600
    readonly property int defaultHeight: 800
    width: defaultWidth
    height: defaultHeight
    minimumWidth: 600
    minimumHeight: 300
    visible: true

    Shortcut {
    sequence: "Ctrl+F"
    onActivated: showFullScreen()
    }

    Shortcut {
        sequence: "Esc"
        onActivated: show()
    }

    Connections {
      target: parent
      function parentNewImg() {
        testingFront.reload();
      }
    }

    GridLayout {
        anchors.fill: parent
        columns: 3
        rows: 2
        columnSpacing: 0
        rowSpacing: 0

        Image {
            id: front
            source: "images/front.png"
            fillMode: Image.PreserveAspectFit
            width: testview.width / 3
            height: testview.height / 2
            Layout.column: 1
            Layout.row: 1
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Image {
            id: left
            source: "images/left.png"
            fillMode: Image.PreserveAspectFit
            width: testview.width / 3
            height: testview.height / 2
            // anchors.right: front.left
            Layout.column: 0
            Layout.row: 1
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Image {
            id: right
            source: "images/right.png"
            fillMode: Image.PreserveAspectFit
            width: testview.width / 3
            height: testview.height / 2
            // anchors.left: front.right
            Layout.column: 2
            Layout.row: 1
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Image {
            id: top
            source: "images/top.png"
            width: testview.width / 3
            height: testview.height / 2
            fillMode: Image.PreserveAspectFit
            // anchors.bottom: front.top
            Layout.column: 1
            Layout.row: 0
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Image {
            id: testingFront
            width: testview.width / 3
            height: testview.height / 2
            fillMode: Image.PreserveAspectFit
            // anchors.bottom: front.top
            Layout.column: 2
            Layout.row: 0
            Layout.fillWidth: true
            Layout.fillHeight: true
            // source: "images/top.png"
            function reload() {
              if (source = "images/top.png") {
                source = "images/front.png"
              } else {
                source = "iamges/top.png"
              }
            //   source = "image://test_test_123_multiview/" + Math.random().toString(36).substr(2, 5);
            }
        }
    }
}