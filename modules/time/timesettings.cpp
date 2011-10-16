/***************************************************************************
 *                                                                         *
 *   Copyright 2011 Sebastian Kügler <sebas@kde.org>                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "timesettings.h"

#include <kdebug.h>
#include <KIcon>
#include <KLocale>

#include <QTimer>
#include <QVariant>

#include <kdemacros.h>
#include "settingsmodule_macros.h"
#include <QtDeclarative/qdeclarative.h>
#include <QtCore/QDate>

//SETTINGSMODULE_PLUGIN_EXPORT("TimeSettings");

K_PLUGIN_FACTORY(TimeSettingsFactory, registerPlugin<TimeSettings>();)
K_EXPORT_PLUGIN(TimeSettingsFactory("active_settings_time"))

class TimeSettingsPrivate {
public:
    QString name;
    QString description;
    QString module;
    QString iconName;
    QIcon icon;
    QTimer *timer;
};

TimeSettings::TimeSettings(QObject *parent, const QVariantList &list)
    : SettingsModule(parent, list)
{
    qmlRegisterType<TimeSettings>();
    qmlRegisterType<TimeSettings>("org.kde.active.settings", 0, 1, "TimeSettings");
}

TimeSettings::TimeSettings()
{
    d = new TimeSettingsPrivate;
    d->name = i18n("Date and Time");
    d->module = QString();

    // Just for making sure that data gets through
    d->timer = new QTimer(this);
    d->timer->setInterval(1000);
    connect(d->timer, SIGNAL(timeout()), SLOT(timeout()));
    d->timer->start();
    //kDebug() << " @@@@@@@@@@@@@@@@ Loaded Module Successfully: EMPTY CTOR" << d->name << d->module;
}


TimeSettings::~TimeSettings()
{
    delete d;
}

QString TimeSettings::name()
{
    return d->name;
}

void TimeSettings::timeout()
{
    d->description = KGlobal::locale()->formatTime(QTime::currentTime(), true);
    //kDebug() << "timeout" << d->description;
    emit descriptionChanged();
}


QString TimeSettings::description()
{
    return d->description;
}

QString TimeSettings::module()
{
    return d->module;
}

QString TimeSettings::iconName()
{
    return d->iconName;
}

QIcon TimeSettings::icon()
{
    return d->icon;
}

void TimeSettings::setName(const QString &name)
{
    d->name = name;
}

void TimeSettings::setDescription(const QString &description)
{
    d->description = description;
}

void TimeSettings::setIconName(const QString &iconName)
{
    d->iconName = iconName;
}

void TimeSettings::setModule(const QString &module)
{
    d->module = module;
}

void TimeSettings::setIcon(const QIcon &icon)
{
    d->icon = icon;
}

#include "timesettings.moc"
