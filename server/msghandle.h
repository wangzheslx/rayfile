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
    PDU* AddFriend(PDU*pdu);
    PDU* AddFriendagree (PDU*pdu);
    PDU* GetFriend(PDU*pdu);
    PDU* DelFriend(PDU* pdu);

};

#endif // MSGHANDLE_H
