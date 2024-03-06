#ifndef WIDGET_H
#define WIDGET_H

#include "protocol.h"
#include "reshandler.h"
#include <QTcpSocket>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QWidget
{
    Q_OBJECT

public:
    static Client& getInstance();
    QTcpSocket& getTcpsocket();
    ~Client();
    void loadConfig();
    PDU* readPDU();
    void handlePDU(PDU* pdu);
    ResHandler* m_pmh;

public slots:
    void showConnect();
private slots:
    //void on_send_PB_clicked();
    void recvMsg();
    void on_regist_PB_clicked();

    void on_login_PB_clicked();

private:
    Ui::Client *ui;
    QString m_strIP;
    quint16 m_usPort;
    QTcpSocket m_tcpsocket;
    Client(QWidget *parent = nullptr);
    Client(const Client& instance) = delete;
    Client& operator=(const Client&) = delete;
};
#endif // WIDGET_H
