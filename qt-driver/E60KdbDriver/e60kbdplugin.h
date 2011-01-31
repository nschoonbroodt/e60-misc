#ifndef E60KBDPLUGIN_H
#define E60KBDPLUGIN_H

#include <QKbdDriverPlugin>

class E60KbdPlugin : public QKbdDriverPlugin {
    Q_OBJECT
public:
    E60KbdPlugin( QObject *parent  = 0 );
    ~E60KbdPlugin();
    QWSKeyboardHandler* create(const QString& driver, const QString& device);
    QStringList keys() const;
};

#endif // E60KBDPLUGIN_H
