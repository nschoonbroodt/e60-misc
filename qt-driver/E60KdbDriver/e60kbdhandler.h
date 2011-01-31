#ifndef E60KBDHANDLER_H
#define E60KBDHANDLER_H

#include <QWSKeyboardHandler>
#include <QSocketNotifier>

class E60KbdHandler :public QObject, public QWSKeyboardHandler
{
    Q_OBJECT
public:
    E60KbdHandler(const QString &device = QString("/dev/input/event0"));
    ~E60KbdHandler();
private:
    QSocketNotifier *m_notify;
    int  kbdFd;
private Q_SLOTS:
    void readKbdData();
};

#endif // E60KBDHANDLER_H
