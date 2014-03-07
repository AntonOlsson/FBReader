/*
 * Copyright (C) 2004-2011 Geometer Plus <contact@geometerplus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: root
    property variant handler
    width: parent.width
    visible: handler.visible
    height: childrenRect.height
    

//    Label {
//        id: label
//        text: handler.name
//        x: Theme.paddingLarge
//    }

    TextField {
        id: textField
        width: parent.width
//        anchors {
//            left: label.right
//            right: root.right
//            leftMargin: Theme.paddingMedium
//            rightMargin: Theme.paddingLarge
//            verticalCenter: label.verticalCenter
//        }
        label: handler.name
        placeholderText: label
        enabled: handler.enabled
        text: handler.text
        echoMode: handler.password ? TextInput.PasswordEchoOnEdit : TextInput.Normal
        onTextChanged: root.handler.text = text
    }

//    Connections {
//        target: root.handler
//        onTextChanged: textField.text = root.handler.text
//    }
}
