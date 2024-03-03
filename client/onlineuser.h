#ifndef ONLINEUSER_H
#define ONLINEUSER_H

#include <QWidget>

namespace Ui {
class OnlineUser;
}

class OnlineUser : public QWidget
{
    Q_OBJECT

public:
    explicit OnlineUser(QWidget *parent = nullptr);
    ~OnlineUser();
    void showOnlineUser(QStringList mlist);

private:
    Ui::OnlineUser *ui;
};

#endif // ONLINEUSER_H
