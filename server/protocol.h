#ifndef PROTOCOL_H
#define PROTOCOL_H

#include<stdlib.h>
#include<unistd.h>
#include<string.h>

typedef unsigned int uint;
//登录状态状态机
enum ENUM_MSG_TYPE{
    ENUM_MSG_TYPE_MIN = 0,
    ENUM_MSG_TYPE_REGIST_REQUEST,
    ENUM_MSG_TYPE_REGIST_RESPEND,
    ENUM_MSG_TYPE_MAX = 0x00ffffff,
};
//协议数据结构体
struct PDU{
    uint uiPDUlen;//总长度
    uint uiMsgLen;//数据长度
    uint uiMsgType;//协议消息类型
    char caData[64];//参数
    char caMsg[];//数据
};

PDU* mkPDU(uint uiMSGLen);

#endif // PROTOCOL_H
