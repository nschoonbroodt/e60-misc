#include "e60kbdplugin.h"

#include <QDebug>

#include "e60kbdhandler.h"

QStringList E60KbdPlugin::keys() const
{
    return QStringList() << "e60kbddriver";
}

QWSKeyboardHandler* E60KbdPlugin::create(const QString& driver,
                                         const QString& device)
{
    if (driver.toLower() == "e60kbddriver") {
        qDebug() << "Before call E60KbdHandler()";
        return new E60KbdHandler(device);
    }
    return 0;
}

Q_EXPORT_PLUGIN2(E60KbdDriver, E60KbdPlugin)
