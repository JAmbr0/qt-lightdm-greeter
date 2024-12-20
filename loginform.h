#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QLightDM/Greeter>

#include <QWidget>
#include <QProcess>
#include <QDialog>
#include <QKeyEvent>
#include <QGraphicsOpacityEffect>
#include <QMap>


class Panel;

namespace Ui
{
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

friend class DecoratedUsersModel;

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();
    virtual void setFocus(Qt::FocusReason reason);

    void setPanel(Panel *panel) { m_Panel = panel; }

public slots:
    void userChanged();
    void respond();
    void onPrompt(QString prompt, QLightDM::Greeter::PromptType promptType);
    void authenticationComplete();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private:
    void initialize();

    Ui::LoginForm *ui;
    QLightDM::Greeter m_Greeter;

    Panel *m_Panel;
};

#endif // LOGINFORM_H
