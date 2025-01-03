#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class LoginForm;
class Panel;

namespace Ui{
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(int screen, QWidget *parent = nullptr);
    ~MainWindow();

    void setFocus(Qt::FocusReason reason);
    bool showLoginForm();
    LoginForm* loginForm() { return m_LoginForm; }

private:
    void setBackground();
    int m_Screen;
    LoginForm* m_LoginForm;
    Panel* m_Panel;
};

#endif // MAINWINDOW_H
