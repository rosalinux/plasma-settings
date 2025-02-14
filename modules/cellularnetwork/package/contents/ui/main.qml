//
// SPDX-FileCopyrightText: 2018 Martin Kacej <m.kacej@atlas.sk>
// SPDX-FileCopyrightText: 2020 Dimitris Kardarakos <dimkard@posteo.net>
// SPDX-FileCopyrightText: 2021-2022 Devin Lin <espidev@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.12 as Controls

import org.kde.plasma.networkmanagement 0.2 as PlasmaNM
import org.kde.kirigami 2.19 as Kirigami
import org.kde.kcm 1.3 as KCM

import cellularnetworkkcm 1.0

import "mobileform" as MobileForm

KCM.SimpleKCM {
    id: root
    objectName: "mobileDataMain"

    leftPadding: 0
    rightPadding: 0
    topPadding: Kirigami.Units.gridUnit
    bottomPadding: Kirigami.Units.gridUnit
    
    Kirigami.PlaceholderMessage {
        id: noModem
        anchors.centerIn: parent
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Kirigami.Units.largeSpacing
        
        visible: !enabledConnections.wwanHwEnabled || !availableDevices.modemDeviceAvailable || !kcm.modemFound
        icon.name: "auth-sim-missing"
        text: i18n("Modem not available")
    }
    
    ColumnLayout {
        spacing: 0
        width: root.width
        visible: !noModem.visible
        
        PlasmaNM.Handler {
            id: nmHandler
        }

        PlasmaNM.AvailableDevices {
            id: availableDevices
        }

        PlasmaNM.EnabledConnections {
            id: enabledConnections

            onWwanEnabledChanged: {
                mobileDataCheckbox.checked = mobileDataCheckbox.enabled && enabled
            }

            onWwanHwEnabledChanged: {
                mobileDataCheckbox.enabled = enabled && availableDevices.modemDeviceAvailable
            }
        }
        
        MessagesList {
            Layout.fillWidth: true
            Layout.margins: Kirigami.Units.largeSpacing
            model: kcm.messages
        }
        
        MobileForm.FormCard {
            Layout.fillWidth: true
            
            contentItem: ColumnLayout {
                spacing: 0
                
                MobileForm.FormSwitchDelegate {
                    id: mobileDataSwitch
                    text: i18n("Mobile data")
                    description: i18n("Whether mobile data is enabled.")
                    
                    enabled: kcm.selectedModem
                    
                    checked: kcm.selectedModem && kcm.selectedModem.enabled
                    onCheckedChanged: {
                        if (kcm.selectedModem.enabled != checked) {
                            kcm.selectedModem.enabled = !checked
                        }
                    }
                }
                
                Kirigami.Separator {
                    Layout.leftMargin: Kirigami.Units.largeSpacing
                    Layout.rightMargin: Kirigami.Units.largeSpacing
                    Layout.fillWidth: true
                    opacity: (!mobileDataSwitch.controlHovered && !dataUsageButton.controlHovered) ? 0.5 : 0
                }
                
                MobileForm.FormButtonDelegate {
                    id: dataUsageButton
                    text: i18n("Data Usage")
                    description: i18n("View data usage.")
                    iconName: "office-chart-bar"
                    
                    enabled: false
                }
            }
        }
        
        MobileForm.FormCard {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing
            
            contentItem: ColumnLayout {
                spacing: 0
                
                MobileForm.FormCardHeader {
                    title: kcm.sims.count == 1 ? i18n("SIM") : i18n("SIMs")
                }
                
                Repeater {
                    id: repeater
                    model: kcm.sims
                    
                    delegate: ColumnLayout {
                        Layout.fillWidth: true
                        
                        Kirigami.Separator {
                            visible: model.index !== 0
                            Layout.leftMargin: Kirigami.Units.largeSpacing
                            Layout.rightMargin: Kirigami.Units.largeSpacing
                            Layout.fillWidth: true
                            opacity: (!(model.index && repeater.itemAt(model.index - 1).controlHovered) && !simDelegate.controlHovered) ? 0.5 : 0
                        }
                        
                        MobileForm.FormButtonDelegate {
                            id: simDelegate
                            text: i18n("SIM %1", modelData.displayId)
                            description: i18n("View SIM %1 details.", modelData.displayId)
                            iconName: "auth-sim-symbolic"
                            onClicked: kcm.push("Sim.qml", { "sim": modelData })
                        }
                    }
                }
            }
        }
    }
}
