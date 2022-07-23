/*
 * Copyright (C) 2020 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2

ToolBar {
    Layout.minimumWidth: 280
    Layout.minimumHeight: 370

    id: main
    background: Rectangle {
        color: "transparent"
    }

    function parentNewImg() {}

    Connections {
      target: multiview_rendering
      function onNewImage() {
        parentNewImg();
      }
    }

    RowLayout {
        spacing: 20
        anchors.fill: parent

        Button {
            text: qsTr("2x2 Multiview")

            onClicked: {
                var component = Qt.createComponent("Multiview.qml")
                if (component.status != Component.Ready) {
                    if (component.status == Component.Error) {
                        console.debug("Error: " + component.errorString());
                        return;
                    }
                }
                var multiview = component.createObject(main);
                multiview.show();
            }
        }

        Button {
            text: qsTr("1-3 Multiview")

            onClicked: {
                var component = Qt.createComponent("MultiviewTest.qml")
                if (component.status != Component.Ready) {
                    if (component.status == Component.Error) {
                        console.debug("Error: " + component.errorString());
                        return;
                    }
                }
                var multiviewTest = component.createObject(main);
                multiviewTest.show();
            }
        }

        Image {
            id: testingFront
            width: 80
            height: 60
            fillMode: Image.PreserveAspectFit
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