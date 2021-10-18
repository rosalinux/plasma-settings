/*

    SPDX-FileCopyrightText: 2019 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QAbstractListModel>
#include <QVector>

#include <KPackage/Package>
#include <KPluginMetaData>
#include <KQuickAddons/ConfigModule>

struct Data {
    KPluginMetaData plugin;
    QPointer<KQuickAddons::ConfigModule> kcm;
    bool operator<(const Data &other) const
    {
        return plugin.name() < other.plugin.name();
    }
};

class ModulesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ModelRoles { NameRole = Qt::DisplayRole, IconNameRole = Qt::DecorationRole, DescriptionRole = Qt::UserRole + 1, KcmRole };

public:
    ModulesModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QHash<int, QByteArray> roleNames() const override;

private:
    KQuickAddons::ConfigModule *instantiateKcm(const QString &name) const;

    QVector<Data> m_plugins;
};
