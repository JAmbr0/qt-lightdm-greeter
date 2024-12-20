#include "mainwindow.h"
#include "settings.h"

#include <QtWidgets/QApplication>

#include <QScreen>
#include <QtGlobal>
#include <QDebug>
#include <QSettings>

#include <iostream>

QFile logfile;
QTextStream textstream;

void messageHandler(QtMsgType type, const QMessageLogContext&, const QString& msg)
{
    std::cerr << type << ": " << msg.toLatin1().data() << "\n";
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(messageHandler);
    Cache::prepare();

    QApplication application(argc, argv);

    MainWindow *focusWindow = nullptr;
    const QList<QScreen *> screens = QGuiApplication::screens();
    for (int i = 0; i < screens.size(); ++i)
    {
        MainWindow *window = new MainWindow(i);
        window->show();
        if (window->showLoginForm())
            focusWindow = window;
    }

    // Set only primary screen's widget as active if multiple screens
    if (focusWindow)
    {
        focusWindow->setFocus(Qt::OtherFocusReason);
        focusWindow->activateWindow();
    }

    return application.exec();
}
