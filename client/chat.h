#ifndef CHAT_H
#define CHAT_H

#include "protocol.h"

#include <QWidget>
#include <QString>
namespace Ui {
class chat;
}

class chat : public QWidget
{
    Q_OBJECT

public:
    explicit chat(QWidget *parent = nullptr);
    QString m_strChatName;
    void show_TEXT(QString sender,QString opposite,PDU* pdu);
    ~chat();

private slots:
    void on_pushButton_clicked();

private:
    Ui::chat *ui;
};

#endif // CHAT_H
