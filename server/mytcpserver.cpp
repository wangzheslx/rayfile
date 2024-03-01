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
    connect(ptcpsocket,&MyTcpSocket::offline,this,&MyTcpServer::deleteSocket);
    //测试连接的客户端
//    foreach(MyTcpSocket *ptcpsocket,m_tcpSocketList){
//        qDebug()<<ptcpsocket;
    //    }
}

void MyTcpServer::deleteSocket(MyTcpSocket* mytcpsocket)
{
    //tcpserver删除socket列表操作,槽函数
    qDebug()<<"tcpserver删除socket列表操作";
    m_tcpSocketList.removeOne(mytcpsocket);
    //不能立即删除socket，因为mytcpsocket里面的程序还在运行
    mytcpsocket->deleteLater();//延迟删除
    mytcpsocket = NULL;
    qDebug()<<"当前剩余客户端数量"<<m_tcpSocketList.size();
    qDebug()<<"剩余客户端名称：";
    foreach(MyTcpSocket* m_socket,m_tcpSocketList){
        qDebug()<<m_socket->m_strLogName;
    }
}

MyTcpServer::MyTcpServer()
{

}
