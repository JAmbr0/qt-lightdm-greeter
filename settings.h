#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QDebug>


#define CONFIG_FILE "/etc/lightdm/qt-lightdm-greeter.conf"
#define BACKGROUND_IMAGE_KEY "greeter-background-image"
#define LOGFILE_PATH_KEY "logfile-path"


class Cache : public QSettings
{
public:
    static const QString GREETER_DATA_DIR_PATH;
    static void prepare();
    static void logMessage(const QString &message);

    Cache() : QSettings(GREETER_DATA_DIR_PATH + "/state", QSettings::NativeFormat) {}
    QString getLastUser() { return value("last-user").toString(); }
    void setLastUSer(QString userId) { setValue("last-user", userId); }
    QString getLastSession(QString userId) { return value(userId + "/last-session").toString(); }
    void setLastSession(QString userId, QString session) { setValue(userId + "/last-session", session); }
};

class Settings : public QSettings
{
public:
    Settings() : QSettings(QString("/etc/lightdm/qt-lightdm-greeter.conf"), QSettings::NativeFormat) {}
    QString backgroundImagePath() { return value("greeter-background-image").toString(); }
    QString logFilePath() { return value(LOGFILE_PATH_KEY, "/tmp/qt-light-dm-greeter.log").toString(); }
};

#endif // SETIINGS_H
