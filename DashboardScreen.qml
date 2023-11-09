import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl

Item {
    Loader {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        anchors.leftMargin: sidebarWidth - sidebarRadius

        source: currentView
    }

    Frame {
        width: sidebarWidth
        height: parent.height
        background: Rectangle {
            width: parent.width + radius
            x: -radius
            radius: sidebarRadius
            border.color: "#dedede"
        }
        padding: 0

        Column {
            anchors.fill: parent
            Repeater {

                model: ListModel {
                    ListElement {
                        name: "Buku"
                        view: "BukuScreen.qml"
                    }
                    ListElement {
                        name: "Kategori"
                        view: "KategoriScreen.qml"
                    }
                    ListElement{
                        name : "Penerbit"
                        view : "PenerbitScreen.qml"
                    }
                    ListElement{
                        name : "Member"
                        view : "MemberScreen.qml"
                    }
                    ListElement {
                        name: "Pengadaan Buku"
                        view: "PengadaanScreen.qml"
                    }
                    ListElement {
                        name: "Peminjaman Buku"
                        view: "PeminjamanList.qml"
                    }
                    ListElement {
                        name: "Pengaturan"
                        view: "SettingsScreen.qml"
                    }
                }

                delegate: ItemDelegate {
                    id: sidebarDelegate
                    width: parent.width
                    onClicked: currentView = view
                    highlighted: currentView === view

                    contentItem: Label {
                        text: name
                        font.bold: true
                        verticalAlignment: Qt.AlignVCenter
                        color: sidebarDelegate.highlighted ?
                                   sidebarDelegate.Material.primary :
                                   sidebarDelegate.Material.foreground
                    }


                    background: Rectangle {
                        implicitHeight: sidebarDelegate.Material.delegateHeight
                        x: -sidebarRadius
                        width: parent.width + sidebarRadius

                        color: sidebarDelegate.highlighted ?
                                   Qt.alpha(Material.shade(sidebarDelegate.Material.primary, Material.Shade500), 0.12)
                                 : "transparent"
                        radius: 32

                        Ripple {
                            width: parent.width
                            height: parent.height
                            clip: true
                            clipRadius: 32
                            pressed: sidebarDelegate.pressed
                            anchor: sidebarDelegate
                            active: enabled &&
                                    (sidebarDelegate.down || sidebarDelegate.visualFocus || sidebarDelegate.hovered)
                            color: sidebarDelegate.Material.rippleColor
                        }

                    }
                }
            }
        }
    }
}