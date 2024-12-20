#include "panel.h"
#include "ui_panel.h"
#include "settings.h"

#include <QTimer>
#include <QDateTime>


Panel::Panel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Panel),
    power(this)
{
    ui->setupUi(this);

    // Setup sessions model in the panel
    ui->sessionCombo->setModel(&sessionsModel);
    ui->sessionCombo->setCurrentIndex(0);

    // Setup power actions
    addLeaveEntry(power.canShutdown(), "system-shutdown", tr("Shutdown"), "shutdown");
    addLeaveEntry(power.canRestart(), "system-reboot", tr("Restart"), "restart");
    addLeaveEntry(power.canHibernate(), "system-suspend-hibernate", tr("Hibernate"), "hibernate");
    addLeaveEntry(power.canSuspend(), "system-suspend", tr("Suspend"), "suspend");

    ui->leaveComboBox->setDisabled(ui->leaveComboBox->count() <= 1);

    connect(ui->leaveComboBox, SIGNAL(activated(int)), this, SLOT(onLeaveComboBoxActivated(int)));

    // Create timer to update datetime label
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        QDateTime currentTime = QDateTime::currentDateTime();
        QString formattedTime = currentTime.toString("dddd, dd-MM-yyyy hh:mm");
        ui->currentDateTime->setText(formattedTime);
    });

    // Start timer with a 1-second interval and update at startup
    timer->start(1000);
    ui->currentDateTime->setText(QDateTime::currentDateTime().toString("ddd, dd-MM-yyyy hh:mm"));
}

Panel::~Panel()
{
    delete ui;
}

QString Panel::currentSession()
{
    QModelIndex index = sessionsModel.index(ui->sessionCombo->currentIndex(), 0, QModelIndex());
    return sessionsModel.data(index, QLightDM::SessionsModel::KeyRole).toString();
}

void Panel::addLeaveEntry(bool canDo, const QString &iconName, const QString &text, const QString &actionName)
{
    if (canDo) {
        ui->leaveComboBox->addItem(QIcon::fromTheme(iconName), text, actionName);
    }
}

void Panel::onLeaveComboBoxActivated(int index) {
    QString actionName = ui->leaveComboBox->itemData(index).toString();

    if (actionName == "shutdown") emit requestShutdown();
    else if (actionName == "restart") emit requestRestart();
    else if (actionName == "hibernate") emit requestHibernate();
    else if (actionName == "suspend") emit requestSuspend();
}
