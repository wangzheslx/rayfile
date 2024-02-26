#include "protocol.h"

PDU *mkPDU(uint uiMSGLen)
{
    uint uiPDULen = sizeof(PDU)+uiMSGLen;
    PDU *pdu = (PDU*)malloc(uiPDULen);
    if(pdu==nullptr){
        exit(1);
    }
    memset(pdu,0,uiPDULen);
    pdu->uiPDUlen = uiPDULen;
    pdu->uiMsgLen = uiMSGLen;
    return pdu;
}
