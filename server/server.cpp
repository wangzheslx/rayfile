#include "mytcpserver.h"
#include "server.h"
#include "ui_server.h"

#include <QFile>
#include <QMessageBox>
#include <QDebug>
Server &Server::getinstance()
{
    static Server instance;
    return instance;
}

void Server::loadConfig()
{
    QFile file(":/server.config");
    if(file.open(QIODevice::ReadOnly)){
        QByteArray batata = file.readAll();
        QString strdata = QString(batata);
        //qDebug()<<strdata;
        QStringList strList =  strdata.split("\r\n");
        m_strIP = strList.at(0);
        m_usPort = strList.at(1).toUShort();
        m_strRootPath = strList.at(2);
        qDebug()<<"IP:"<<m_strIP<< " PORT:"<<m_usPort<<" ROOT:"<<m_strRootPath;
        file.close();

    }else{
        QMessageBox::critical(this,"文件配置","文件打开错误");
    }
}

QString Server::getrootpath()
{
    return m_strRootPath;
}


Server::Server(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);
    loadConfig();
    MyTcpServer::getInstance().listen(QHostAddress(m_strIP),m_usPort);
}

Server::~Server()
{
    delete ui;
}

