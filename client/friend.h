#ifndef FRIEND_H
#define FRIEND_H

#include <QListWidget>
#include <QWidget>
#include"onlineuser.h"
#include "chat.h"
namespace Ui {
class Friend;
}

class Friend : public QWidget
{
    Q_OBJECT

public:
    explicit Friend(QWidget *parent = nullptr);
    OnlineUser*getonlineuser();
    chat* getchat();
    void getFriendRequest();
    void showfriend(QStringList list);
    void flushFriend();
    ~Friend();

private slots:
    void on_findUser_PB_clicked();

    void on_onlineUser_PB_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_flushFriend_PB_clicked();

    void on_delFriend_PB_clicked();

    void on_chat_PB_clicked();

private:
    Ui::Friend *ui;
    OnlineUser* m_onlineuser;
    chat * m_chat;
};

#endif // FRIEND_H
