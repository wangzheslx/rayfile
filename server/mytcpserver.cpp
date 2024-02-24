#include "mytcpserver.h"
#include<QDebug>
MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr handle)
{
    qDebug()<<"新客户端建立连接";
    //MyTcpServer* ptcpSocket = new MyTcpSocket;
}

MyTcpServer::MyTcpServer()
{

}
