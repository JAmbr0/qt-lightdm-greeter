#include "mainwindow.h"
#include "loginform.h"
#include "panel.h"
#include "settings.h"

#include <QRect>
#include <QGuiApplication>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>
#include <QPalette>
#include <QSettings>
#include <QImage>
#include <QBrush>
#include <QString>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>


MainWindow::MainWindow(int screen, QWidget *parent) :
    QWidget(parent),
    m_Screen(screen),
    m_LoginForm(nullptr),
    m_Panel(nullptr)
{
    setObjectName(QString("MainWindow_%1").arg(screen));
    const QList<QScreen *> screens = QGuiApplication::screens();
    if (screen < 0 || screen >= screens.size()) {
        qWarning() << "Invalid screen index:" << screen;
        return;
    }

    QRect screenRect = screens.at(screen)->geometry();
    setGeometry(screenRect);
    setBackground();

    // Display login dialog only in the main screen
    if (showLoginForm())
    {
        // Create and set up login form and panel
        m_Panel = new Panel(this);
        m_LoginForm = new LoginForm(this);

        // Set up main vertical layout for entire window
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0,0,0,0);
        mainLayout->setSpacing(0);

        // Add panel to main layout
        mainLayout->addWidget(m_Panel, 0, Qt::AlignTop);

        // Prepare the logo
        QLabel *logo = new QLabel(this);
        QPixmap logoPixmap("/home/josh/Projects/qt-lightdm-greeter/resources/logo.png");
        logo->setPixmap(logoPixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        logo->setAlignment(Qt::AlignCenter);

        // Add logo to center of screen
        mainLayout->addWidget(logo, 0, Qt::AlignCenter);

        // Set up horizontal layout to center the login form
        QHBoxLayout *centerLayout = new QHBoxLayout();
        centerLayout->addStretch(1);
        centerLayout->addWidget(m_LoginForm, 0, Qt::AlignCenter);
        centerLayout->addStretch(1);

        // Add center layout to main layout
        mainLayout->addLayout(centerLayout, 0);

        // Set the main layout as the layout for the main window
        setLayout(mainLayout);

        // Show the login form
        m_LoginForm->show();


        // Connect the panel signals to perform power actions
        connect(m_Panel, &Panel::requestShutdown, [this]() {
            QLightDM::PowerInterface power;
            power.shutdown();
        });
        connect(m_Panel, &Panel::requestRestart, [this]() {
            QLightDM::PowerInterface power;
            power.restart();
        });
        connect(m_Panel, &Panel::requestHibernate, [this]() {
            QLightDM::PowerInterface power;
            power.hibernate();
        });
        connect(m_Panel, &Panel::requestSuspend, [this]() {
            QLightDM::PowerInterface power;
            power.suspend();
        });
    }
}

MainWindow::~MainWindow() {}

bool MainWindow::showLoginForm()
{
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    const QList<QScreen *> screens = QGuiApplication::screens();

    if (m_Screen >= 0 && m_Screen < screens.size()) {
        return screens.at(m_Screen) == primaryScreen;
    }

    qWarning() << "Invalid screen index:" << m_Screen;
    return false;
}

void MainWindow::setFocus(Qt::FocusReason reason)
{
    if (m_LoginForm) {
        m_LoginForm->setFocus(reason);
    }
    else {
        QWidget::setFocus(reason);
    }
}

void MainWindow::setBackground()
{
    QImage backgroundImage;
    QSettings greeterSettings(CONFIG_FILE, QSettings::IniFormat);

    if (greeterSettings.contains(BACKGROUND_IMAGE_KEY)) {
        QString pathToBackgroundImage = greeterSettings.value(BACKGROUND_IMAGE_KEY).toString();
        backgroundImage = QImage(pathToBackgroundImage);

        if (backgroundImage.isNull()) {
            qWarning() << "Not able to read" << pathToBackgroundImage << "as image";
        }
    }

    QPalette palette;
    const QList<QScreen *> screens = QGuiApplication::screens();
    if (m_Screen < 0 || m_Screen >= screens.size()) {
        qWarning() << "Invalid screen index:" << m_Screen;
        return;
    }

    QRect rect = screens.at(m_Screen)->geometry();

    if (backgroundImage.isNull()) {
        palette.setColor(QPalette::Window, Qt::white);
    }
    else {
        QBrush brush(backgroundImage.scaled(rect.width(), rect.height()));
        palette.setBrush(this->backgroundRole(), brush);
    }
    this->setPalette(palette);
}
