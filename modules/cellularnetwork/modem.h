//
// SPDX-FileCopyrightText: 2021 Devin Lin <espidev@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "cellularnetworksettings.h"
#include "modemdetails.h"
#include "sim.h"

#include <QList>
#include <QString>

#include <NetworkManagerQt/CdmaSetting>
#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/GsmSetting>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/ModemDevice>
#include <NetworkManagerQt/Settings>

#include <ModemManagerQt/GenericTypes>
#include <ModemManagerQt/Manager>
#include <ModemManagerQt/ModemDevice>

#include <modem3gpp.h>

class ProfileSettings;
class Sim;
class AvailableNetwork;
class ModemDetails;
class MobileProviders;

// only supports GSM/UMTS/LTE
class Modem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ModemDetails *details READ modemDetails NOTIFY modemDetailsChanged)
    Q_PROPERTY(QString uni READ uni NOTIFY uniChanged)
    Q_PROPERTY(QString displayId READ displayId NOTIFY displayIdChanged)

    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool isRoaming READ isRoaming WRITE setIsRoaming NOTIFY isRoamingChanged)
    Q_PROPERTY(bool hasSim READ hasSim NOTIFY hasSimChanged)
    Q_PROPERTY(QList<ProfileSettings *> profiles READ profileList NOTIFY profileListChanged)
    Q_PROPERTY(QString activeConnectionUni READ activeConnectionUni NOTIFY activeConnectionUniChanged)

public:
    Modem(QObject *parent = nullptr)
        : QObject{parent}
    {
    }
    Modem(QObject *parent, ModemManager::ModemDevice::Ptr mmDevice, NetworkManager::ModemDevice::Ptr nmDevice, ModemManager::Modem::Ptr m_mmInterface);

    ModemDetails *modemDetails();
    QString displayId(); // splits uni and obtains the number suffix
    QString uni();
    QString activeConnectionUni();

    Q_INVOKABLE void reset();

    bool enabled();
    void setEnabled(bool enabled);

    bool isRoaming();
    void setIsRoaming(bool roaming);
    bool hasSim();

    // connection profiles
    QList<ProfileSettings *> &profileList();
    void refreshProfiles();
    Q_INVOKABLE void activateProfile(const QString &connectionUni);
    Q_INVOKABLE void addProfile(QString name, QString apn, QString username, QString password, QString networkType);
    Q_INVOKABLE void removeProfile(const QString &connectionUni);
    Q_INVOKABLE void updateProfile(QString connectionUni, QString name, QString apn, QString username, QString password, QString networkType);
    Q_INVOKABLE void addDetectedProfileSettings(); // detect modem connection settings (ex. apn) and add a new connection

    QList<Sim *> sims();

    ModemManager::ModemDevice::Ptr mmModemDevice();
    NetworkManager::ModemDevice::Ptr nmModemDevice();
    ModemManager::Modem::Ptr mmModemInterface();

Q_SIGNALS:
    void modemDetailsChanged();
    void uniChanged();
    void displayIdChanged();
    void activeConnectionUniChanged();

    void enabledChanged();
    void isRoamingChanged();
    void simsChanged();
    void hasSimChanged();
    void profileListChanged();

    void couldNotAutodetectSettings();

private:
    QString nmDeviceStateStr(NetworkManager::Device::State state);

    ModemDetails *m_details;

    ModemManager::ModemDevice::Ptr m_mmDevice;
    NetworkManager::ModemDevice::Ptr m_nmDevice;
    ModemManager::Modem::Ptr m_mmInterface = nullptr;
    ModemManager::Modem3gpp::Ptr m_mm3gppDevice = nullptr; // this may be a nullptr if no sim is inserted

    QList<ProfileSettings *> m_profileList;

    friend class ModemDetails;
};

class ProfileSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString apn READ apn WRITE setApn NOTIFY apnChanged)
    Q_PROPERTY(QString user READ user WRITE setUser NOTIFY userChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString networkType READ networkType WRITE setNetworkType NOTIFY networkTypeChanged)
    Q_PROPERTY(QString connectionUni READ connectionUni NOTIFY connectionUniChanged)

public:
    ProfileSettings(QObject *parent = nullptr)
        : QObject{parent}
    {
    }
    ProfileSettings(QObject *parent,
                    QString name,
                    QString apn,
                    QString user,
                    QString password,
                    NetworkManager::GsmSetting::NetworkType networkType,
                    QString connectionUni);
    ProfileSettings(QObject *parent, NetworkManager::Setting::Ptr settings, NetworkManager::Connection::Ptr connection);

    QString name();
    QString apn();
    void setApn(QString apn);
    QString user();
    void setUser(QString user);
    QString password();
    void setPassword(QString password);
    QString networkType();
    void setNetworkType(QString ipType);
    QString connectionUni();

    // utilities
    static QString networkTypeStr(NetworkManager::GsmSetting::NetworkType networkType);
    static NetworkManager::GsmSetting::NetworkType networkTypeFlag(const QString &networkType);

Q_SIGNALS:
    void nameChanged();
    void apnChanged();
    void userChanged();
    void passwordChanged();
    void networkTypeChanged();
    void connectionUniChanged();

private:
    QString m_name;
    QString m_apn;
    QString m_user;
    QString m_password;
    QString m_networkType;
    QString m_connectionUni;
};
