#include "e60kbdhandler.h"

#include <QDebug>
#include <qplatformdefs.h>

#include <errno.h>

E60KbdHandler::E60KbdHandler(const QString &device)
{
    qDebug() << "Loaded E60 keyboard plugin!";
    setObjectName("E60 Keypad Handler");
    kbdFd = QT_OPEN(device.toLocal8Bit().constData(), O_RDONLY, 0);
    if (kbdFd >= 0) {
        qDebug() << "Opened" << device << "as keyboard input";
        m_notify = new QSocketNotifier(kbdFd, QSocketNotifier::Read, this);
        connect(m_notify, SIGNAL(activated(int)), this, SLOT(readKbdData()));
    } else {
        qWarning() << "Cannot open " << device.toLocal8Bit().constData()
                   << " for keyboard input " << strerror(errno);
        return;
    }
}

E60KbdHandler::~E60KbdHandler()
{
    if (kbdFd >= 0)
        ::close(kbdFd);
}

struct E60Input {
    unsigned int   dummy1;
    unsigned int   dummy2;
    unsigned short dummy3;
    unsigned short code;
    unsigned short value;
    unsigned short dummy4;
};

void E60KbdHandler::readKbdData()
{
    E60Input event;
    int n = read(kbdFd, &event, sizeof(E60Input));
    if (n != 16) {
        qWarning() << "keypressed: n=" << n;
        return;
    }
    qDebug() << "code=" << event.code
             << "value=" << event.value
             << ((event.value != 0) ? "(Down)" : "(Up)");

    int key_code = 0;
    switch (event.code) {
    case 0x0001:
        key_code = Qt::Key_VolumeUp;
        break;
    case 0x0002:
        key_code = Qt::Key_Up;
        break;
    case 0x0003:
        key_code = Qt::Key_Back;
        break;
    case 0x0009:
        key_code = Qt::Key_VolumeDown;
        break;
    case 0x000a:
        key_code = Qt::Key_Enter;
        break;
    case 0x000b:
        key_code = Qt::Key_MediaNext;
        break;
    case 0x0011:
        key_code = Qt::Key_Bluetooth;
        break;
    case 0x0012:
        key_code = Qt::Key_Left;
        break;
    case 0x0013:
        key_code = Qt::Key_MediaPrevious;
        break;
    case 0x0019:
        key_code = Qt::Key_Menu;
        break;
    case 0x001a:
        key_code = Qt::Key_Right;
        break;
    case 0x0021:
        key_code = Qt::Key_Home;
        break;
    case 0x0022:
        key_code = Qt::Key_Down;
        break;
    case 0x0096:
        key_code = Qt::Key_PowerOff;
        break;
    }
    processKeyEvent(0xffff, key_code, Qt::NoModifier, event.value!=0, false);
}
