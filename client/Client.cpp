#include "Client.h"
#include "ui_Client.h"
#include "protocol.h"
#include "index.h"
#include <QString>
#include <QFile>
#include <QHostAddress>
#include <QMessageBox>



Client &Client::getInstance()
{
    static Client instance;
    return instance;
}

QTcpSocket &Client::getTcpsocket()
{
    return m_tcpsocket;
}

Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
    loadConfig();
    connect(&m_tcpsocket,&QTcpSocket::connected,this,&Client::showConnect);
    connect(&m_tcpsocket,&QTcpSocket::readyRead,this,&Client::recvMsg);
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

void Client::recvMsg()
{
    qDebug()<<"接收消息长度"<<m_tcpsocket.bytesAvailable();
    uint uiPDULen = 0;
    //读取协议长度
    m_tcpsocket.read((char*)&uiPDULen,sizeof(uint));
    uint uiMSGLen = uiPDULen-sizeof(PDU);
    PDU *pdu = mkPDU(uiMSGLen);
    //继续读取剩余数据
    m_tcpsocket.read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));
    qDebug()<<"recvMsg 数据类型"<<pdu->uiMsgType<<endl
           <<"参数1"<<pdu->caData<<"参数2"<<pdu->caData+32<<endl
          <<"数据"<<pdu->caMsg;
    switch(pdu->uiMsgType){
    case ENUM_MSG_TYPE_REGIST_RESPEND:{
        bool ret;
        memcpy(&ret,pdu->caData,sizeof(bool));
        if(ret){
            QMessageBox::information(this,"注册","注册成功");
        }else{
            QMessageBox::information(this,"注册","注册失败，使用非法的名称或密码");
        }
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_RESPEND:{
        bool ret;
        memcpy(&ret,pdu->caData,sizeof(bool));
        if(ret){
            QMessageBox::information(this,"登录","登录成功");
            Index::getinstance().show();
            this->hide();
        }else{
            QMessageBox::information(this,"登录","登录失败，使用错误的名称或密码");
        }
        break;
    }
    case ENUM_MSG_TYPE_FINDUSER_RESPEND:{
        char name[32] = {'\0'};
        int ret;
        memcpy(name,pdu->caData,32);
        memcpy(&ret,pdu->caData+32,sizeof(int));
        if(ret == 1){
            QMessageBox::information(this,"搜索",QString("%1用户存在并且在线").arg(name));
        }else if(ret == 0){
            QMessageBox::information(this,"搜索",QString("%1用户存在但是不在线").arg(name));
        }else if(ret == -1){
            QMessageBox::critical(this,"搜索","没有查到此用户");
        }
        break;
    }
    case ENUM_MSG_TYPE_ONLINEUSER_RESPEND:{
        uint usersize = pdu->uiMsgLen/32;
        QStringList userlist;
        for(int i = 0 ; i < usersize;i++){
            char name[32] = {'\0'};
            memcpy(name,pdu->caData+i*32,32);
            userlist.append(QString(name));
        }
        Index::getinstance().getFriend()->getonlineuser()->showOnlineUser(userlist);
        break;
    }
    default:
        break;
    }
    free(pdu);
    pdu = NULL;
}



//void Client::on_send_PB_clicked()
//{

//    QString strMsg =  ui->input_LE->text();
//    if(strMsg.isEmpty()){
//        QMessageBox::warning(this,"发送消息","消息框不能为空");
//        return;
//    }
//    qDebug()<<"on_send_PB_clicked MSG"<<strMsg;
//    PDU* pdu = mkPDU(strMsg.size());
//    pdu->uiMsgType = ENUM_MSG_TYPE_REGIST_REQUEST;
//    memcpy(pdu->caData,"登录",sizeof("登录"));
//    memcpy(pdu->caMsg,strMsg.toStdString().c_str(),strMsg.size());
//    m_tcpsocket.write((char*)pdu,pdu->uiPDUlen);
//    free(pdu);
//    pdu=nullptr;
//}

void Client::on_regist_PB_clicked()
{
    //用户注册按钮客户端代码实现
    qDebug()<<"用户界面注册操作";
    QString strName = ui->username_LE->text();
    QString strPwd = ui->password_LE->text();
    if(strName.isEmpty()||strPwd.isEmpty()||strName.size()>32||strPwd.size()>32){
        QMessageBox::critical(this,"注册","用户名或密码为空或过长");
        return;
    }
    //构建pdu，不需要message大小
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_REGIST_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),strName.size());
    memcpy(pdu->caData+32,strPwd.toStdString().c_str(),strPwd.size());
    qDebug()<<"注册 strName: "<<strName<<"strPwd: "<<strPwd;
    m_tcpsocket.write((char*)pdu,pdu->uiPDUlen);
    free(pdu);
    pdu = NULL;
}

void Client::on_login_PB_clicked()
{
    //用户登录按钮客户端代码实现
    qDebug()<<"用户界面注册操作";
    QString strName = ui->username_LE->text();
    QString strPwd = ui->password_LE->text();
    if(strName.isEmpty()||strPwd.isEmpty()||strName.size()>32||strPwd.size()>32){
        QMessageBox::critical(this,"登录","用户名或密码为空或过长");
        return;
    }
    //构建pdu，不需要message大小
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),strName.size());
    memcpy(pdu->caData+32,strPwd.toStdString().c_str(),strPwd.size());
    qDebug()<<"登录 strName: "<<strName<<"strPwd: "<<strPwd;
    m_tcpsocket.write((char*)pdu,pdu->uiPDUlen);
    free(pdu);
    pdu = NULL;
}
