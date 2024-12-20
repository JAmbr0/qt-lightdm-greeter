#ifndef PANEL_H
#define PANEL_H

#include <QLightDM/SessionsModel>
#include <QLightDM/Power>

#include <QWidget>


namespace Ui {
class Panel;
}

class Panel : public QWidget
{
    Q_OBJECT

public:
    explicit Panel(QWidget *parent = nullptr);
    ~Panel();

    QString currentSession();

public slots:
    void onLeaveComboBoxActivated(int index);

signals:
    void requestShutdown();
    void requestRestart();
    void requestHibernate();
    void requestSuspend();

private:
    void addLeaveEntry(bool canDo, const QString &iconName, const QString &text, const QString &actionName);

    Ui::Panel *ui;
    QLightDM::SessionsModel sessionsModel;
    QLightDM::PowerInterface power;
};

#endif // PANEL_H
