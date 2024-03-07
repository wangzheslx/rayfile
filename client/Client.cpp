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
    m_pmh = new ResHandler;
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

PDU *Client::readPDU()
{
    qDebug()<<"接收消息长度"<<m_tcpsocket.bytesAvailable();
    uint uiPDULen = 0;
    //读取协议长度
    m_tcpsocket.read((char*)&uiPDULen,sizeof(uint));
    uint uiMSGLen = uiPDULen-sizeof(PDU);
    PDU *pdu = mkPDU(uiMSGLen);
    //继续读取剩余数据
    m_tcpsocket.read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));

    return pdu;
}

void Client::handlePDU(PDU *pdu)
{
    switch(pdu->uiMsgType){
    case ENUM_MSG_TYPE_REGIST_RESPEND:{
        m_pmh->regist(pdu);
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_RESPEND:{
        m_pmh->login(pdu);
        break;
    }
    case ENUM_MSG_TYPE_FINDUSER_RESPEND:{
        m_pmh->finduser(pdu);
        break;
    }
    case ENUM_MSG_TYPE_ONLINEUSER_RESPEND:{
        m_pmh->onlineuser(pdu);
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_RESPEND:{
        m_pmh->addfriendreturn(pdu);
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:{
        m_pmh->requestaddfriend(pdu);
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPEND:{
        m_pmh->respondaddfriend();
        break;
    }
    default:
        break;
    }
    free(pdu);
    pdu = NULL;
}

void Client::sendPDU(PDU *pdu)
{
    qDebug()<<"recvMsg 数据类型"<<pdu->uiMsgType<<endl
           <<"参数1"<<pdu->caData<<"参数2"<<pdu->caData+32<<endl
          <<"数据"<<pdu->caMsg;
    m_tcpsocket.write((char*)pdu,pdu->uiPDUlen);
    free(pdu);
    pdu = NULL;
}

void Client::showConnect()
{
    qDebug()<<"连接服务器成功";
}

void Client::recvMsg()
{
    PDU* pdu = readPDU();
    handlePDU(pdu);
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
    sendPDU(pdu);
}

void Client::on_login_PB_clicked()
{
    //用户登录按钮客户端代码实现
    qDebug()<<"用户界面登录操作";
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
    m_strLogName= strName;
    sendPDU(pdu);
}
