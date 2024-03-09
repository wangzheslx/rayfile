#ifndef FRIEND_H
#define FRIEND_H

#include <QListWidget>
#include <QWidget>
#include"onlineuser.h"
namespace Ui {
class Friend;
}

class Friend : public QWidget
{
    Q_OBJECT

public:
    explicit Friend(QWidget *parent = nullptr);
    OnlineUser*getonlineuser();
    void getFriendRequest();
    void showfriend(QStringList list);
    ~Friend();

private slots:
    void on_findUser_PB_clicked();

    void on_onlineUser_PB_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_flushFriend_PB_clicked();

private:
    Ui::Friend *ui;
    OnlineUser* m_onlineuser;
};

#endif // FRIEND_H
