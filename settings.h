#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>


class Cache : public QSettings
{
public:
    static const QString GREETER_DATA_DIR_PATH;
    static void prepare();

    Cache() : QSettings(GREETER_DATA_DIR_PATH + "/state", QSettings::NativeFormat) {}
    QString getLastUser() { return value("last-user").toString(); }
    void setLastUSer(QString userId) { setValue("last-user", userId); }
    QString getLastSession(QString userId) { return value(userId + "/last-session").toString(); }
    void setLastSession(QString userId, QString session) { setValue(userId + "/last-session", session); }
};


#define CONFIG_FILE "/etc/lightdm/qt-lightdm-greeter.conf"
#define BACKGROUND_IMAGE_KEY "greeter-background-image"


class Settings : public QSettings
{
public:
    Settings() : QSettings(QString("/etc/lightdm/qt-lightdm-greeter.conf"), QSettings::NativeFormat) {}
    QString backgroundImagePath() { return value("greeter-background-image").toString(); }
};

#endif // SETIINGS_H
