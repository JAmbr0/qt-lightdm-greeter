#include "loginform.h"
#include "ui_loginform.h"
#include "panel.h"
#include "settings.h"

#include <QLightDM/UsersModel>

#include <QDebug>
#include <QCompleter>
#include <QStringList>
#include <QMessageBox>


LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm),
    m_Greeter(),
    m_Panel(nullptr)
{
    if (!m_Greeter.connectSync()) {
        close();
    }

    ui->setupUi(this);

    connect(ui->loginButton, &QPushButton::clicked, this, &LoginForm::respond);

    initialize();
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::setFocus(Qt::FocusReason reason)
{
    if (ui->userInput->text().isEmpty()) {
        ui->userInput->setFocus(reason);
    }
    else {
        ui->passwordInput->setFocus(reason);
    }
}

void LoginForm::initialize()
{
    ui->hostnameLabel->setText(m_Greeter.hostname());

    connect(ui->userInput, &QLineEdit::editingFinished, this, &LoginForm::userChanged);
    connect(&m_Greeter, &QLightDM::Greeter::showPrompt, this, &LoginForm::onPrompt);
    connect(&m_Greeter, &QLightDM::Greeter::authenticationComplete, this, &LoginForm::authenticationComplete);

    ui->passwordInput->setEnabled(false);
    ui->passwordInput->clear();

    if (!m_Greeter.hideUsersHint()) {
        QStringList knownUsers;
        QLightDM::UsersModel usersModel;
        for (int i = 0; i < usersModel.rowCount(QModelIndex()); i++) {
            knownUsers << usersModel.data(usersModel.index(i, 0), QLightDM::UsersModel::NameRole).toString();
        }
        ui->userInput->setCompleter(new QCompleter(knownUsers));
        ui->userInput->completer()->setCompletionMode(QCompleter::InlineCompletion);
    }

    QString user = Cache().getLastUser();
    if (user.isEmpty()) {
        user = m_Greeter.selectUserHint();
    }
    ui->userInput->setText(user);
    userChanged();
}

void LoginForm::userChanged()
{
    if (m_Greeter.inAuthentication()) {
        m_Greeter.cancelAuthentication();
    }
    if (!ui->userInput->text().isEmpty()) {
        m_Greeter.authenticate(ui->userInput->text());
        ui->passwordInput->setFocus();
    }
    else {
        ui->userInput->setFocus();
    }
}

void LoginForm::respond()
{
    m_Greeter.respond(ui->passwordInput->text().trimmed());
    ui->passwordInput->clear();
    ui->passwordInput->setEnabled(false);
}

void LoginForm::onPrompt(QString prompt, QLightDM::Greeter::PromptType)
{
    ui->passwordInput->setEnabled(true);
    ui->passwordInput->setFocus();
}

void LoginForm::authenticationComplete()
{
    if (m_Greeter.isAuthenticated()) {
        // Fetch current session from panel
        QString currentSession = m_Panel ? m_Panel->currentSession() : m_Greeter.defaultSessionHint();
        Cache().setLastSession(ui->userInput->text(), currentSession);
        Cache().sync();
        m_Greeter.startSessionSync(currentSession);
    }
    else {
        ui->passwordInput->clear();
        userChanged();
    }
}

void LoginForm::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        respond();
    }
    else {
        QWidget::keyPressEvent(event);
    }
}
