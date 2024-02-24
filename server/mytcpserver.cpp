#include "mytcpserver.h"
#include<QDebug>
//单例模式只有一个服务器
MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr handle)
{
    qDebug()<<"新客户端建立连接";
    //为连接的客户端创建一个socket
    MyTcpSocket* ptcpsocket = new MyTcpSocket;
    //将新的socket赋值
    ptcpsocket->setSocketDescriptor(handle);
    //将新的socket加入socket列表
    m_tcpSocketList.append(ptcpsocket);
    //测试连接的客户端
//    foreach(MyTcpSocket *ptcpsocket,m_tcpSocketList){
//        qDebug()<<ptcpsocket;
//    }
}

MyTcpServer::MyTcpServer()
{

}
