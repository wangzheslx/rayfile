#include "Client.h"
#include "ui_Client.h"

#include <QFile>
#include <QHostAddress>
#include <QMessageBox>



Client &Client::getInstance()
{
    static Client instance;
    return instance;
}

Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
    loadConfig();
    connect(&m_tcpsocket,&QTcpSocket::connected,this,&Client::showConnect);
    m_tcpsocket.connectToHost(QHostAddress(m_strIP),m_usPort);
}

Client::~Client()
{
    delete ui;
}

void Client::loadConfig()
{
    QFile file(":/client.config");
    if(file.open(QIODevice::ReadOnly)){
        QByteArray badata = file.readAll();
        QString strData = QString(badata);
        QStringList strList =  strData.split("\r\n");
        m_strIP = strList.at(0);
        m_usPort = strList.at(1).toUShort();
        qDebug()<<"IP:"<<m_strIP<< " PORT:"<<m_usPort;
        file.close();
    }else{
        QMessageBox::critical(this,"打开配置","打开配置失败");
    }
//    m_strIP = "127.0.0.1";
//    m_usPort = 5000;
}

void Client::showConnect()
{
    qDebug()<<"连接服务器成功";
}



