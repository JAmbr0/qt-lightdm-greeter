#include "settings.h"

#include <QDebug>
#include <QDir>


#define BACKGROUND_IMAGE_KEY "greeter-background-image"
#define LOGFILE_PATH_KEY "logfile-path"

const QString Cache::GREETER_DATA_DIR_PATH = "/var/lib/lightdm/qt-lightdm-greeter";

void Cache::prepare()
{
    QDir dir(GREETER_DATA_DIR_PATH);
    if (!dir.exists()) {
        if (!dir.mkpath(GREETER_DATA_DIR_PATH)) {
            qWarning() << "Unable to create directory" << GREETER_DATA_DIR_PATH;
        }
    }
}
