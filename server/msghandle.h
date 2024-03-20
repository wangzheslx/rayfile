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
    void ResendChat(PDU* pdu);
    PDU* MkDir(PDU* pdu);
    PDU* flush_file(PDU* pdu);
    PDU* del_dir(PDU* pdu);
    PDU* del_file(PDU* pdu);
    PDU* rename_file(PDU* pdu);
    PDU* move_file(PDU* pdu);




};

#endif // MSGHANDLE_H
