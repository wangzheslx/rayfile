#ifndef MSGHANDLE_H
#define MSGHANDLE_H

#include "protocol.h"

#include <QString>



class MsgHandle
{
public:
    MsgHandle();
    PDU* Regist(PDU* pdu);
    PDU* Login(PDU* pdu,QString & strusername);
    PDU* FindUser(PDU* pdu);
    PDU* OnlineUser();
};

#endif // MSGHANDLE_H
