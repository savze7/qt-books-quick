import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Kelompok7.Perpus

Frame {
    property string pengadaanKode: ""
    property string pengadaanSumber: ""
    required property PengadaanBukuModel pengadaanBukuModel

    signal editClicked()
    signal deleteClicked()

    Layout.minimumWidth: 300
    Layout.maximumWidth: 300
    Layout.fillHeight: true
    Layout.rightMargin: 16
    Layout.topMargin: 16
    Layout.bottomMargin: 16
    padding: 16

    background: Rectangle {
        border.color: "#dedede"
        border.width: 1
        radius: 16
    }

    GridLayout {
        anchors.fill: parent
        columnSpacing: 16
        visible: pengadaanKode != ""
        columns: 2

        Label {
            text: "Kode"
        }

        Label {
            text: pengadaanKode
            Layout.fillWidth: true
        }

        Label {
            text: "Sumber"
        }

        Label {
            text: pengadaanSumber
            Layout.fillWidth: true
        }

        Label {
            text: pengadaanBukuModel.count > 0 ?  "Buku" : "Buku kosong"
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignHCenter

        }

        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.columnSpan: 2
            model: pengadaanBukuModel
            spacing: 8

            delegate: Pane {
                width: ListView.view.width
                padding: 16

                background: Rectangle {
                    border.color: "#dedede"
                    border.width: 1
                    radius: 16
                }

                contentItem: ColumnLayout {
                    Label {
                        text: model.judulBuku
                    }
                    Label {
                        text: model.jumlah
                    }
                }
            }
        }

        Row {
            Layout.columnSpan: 2
            spacing: 8

            Button {
                text: "Edit"
                onClicked: editClicked()
            }

            Button {
                text: "Hapus"
                onClicked: deleteClicked()
            }
        }

    }

    Label {
        visible: pengadaanDetailFrame.detailSumber == ""
        text: "Tidak ada item yang terpilih"
    }
}