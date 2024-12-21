#include "settings.h"

#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QFile>


#define BACKGROUND_IMAGE_KEY "greeter-background-image"
#define LOGFILE_PATH_KEY "logfile-path"

const QString Cache::GREETER_DATA_DIR_PATH = "/var/lib/lightdm/qt-lightdm-greeter";

void Cache::prepare()
{
    QDir dir(GREETER_DATA_DIR_PATH);
    if (!dir.exists()) {
        if (!dir.mkpath(GREETER_DATA_DIR_PATH)) {
            logMessage("Unable to create directory: " + GREETER_DATA_DIR_PATH);
        }
    }
    logMessage("Directory prepared successfully: " + GREETER_DATA_DIR_PATH);
}

void Cache::logMessage(const QString &message)
{
    Settings settings;
    QString logFilePath = settings.logFilePath();

    QFile logFile(logFilePath);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&logFile);
        stream << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << " - " << message << Qt::endl;
        logFile.close();
    }
    else {
        qWarning() << "Unable to open log file:" << logFilePath;
    }
}
