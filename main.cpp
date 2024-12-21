#include "mainwindow.h"
#include "settings.h"

#include <iostream>

#include <QApplication>
#include <QScreen>
#include <QtGlobal>


// Direct Qt log to stderr
void messageHandler(QtMsgType type, const QMessageLogContext&, const QString& msg)
{
    std::cerr << type << ": " << msg.toLatin1().data() << "\n";
}

int main(int argc, char *argv[])
{
    // Capture Qt messages and prepare cache before application starts
    qInstallMessageHandler(messageHandler);
    Cache::prepare();

    QApplication application(argc, argv);

    // Track active window
    MainWindow *focusWindow = nullptr;

    // Create MainWindow for each screen
    const QList<QScreen *> screens = QGuiApplication::screens();
    for (int i = 0; i < screens.size(); ++i)
    {
        MainWindow *window = new MainWindow(i);
        window->show();

        // Track the window as active if it needs to display the login form
        if (window->showLoginForm())
            focusWindow = window;
    }

    // Set only one window as active
    if (focusWindow)
    {
        focusWindow->setFocus(Qt::OtherFocusReason);
        focusWindow->activateWindow();
    }

    return application.exec();
}
