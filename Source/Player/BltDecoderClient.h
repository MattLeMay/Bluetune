/*****************************************************************
|
|   BlueTune - Async Layer
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

#ifndef _BLT_DECODER_CLIENT_H_
#define _BLT_DECODER_CLIENT_H_

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "Neptune.h"
#include "Atomix.h"
#include "BltDecoder.h"
#include "BltTime.h"
#include "BltDecoderServer.h"

/*----------------------------------------------------------------------
|   BLT_DecoderClient_MessageHandler
+---------------------------------------------------------------------*/
class BLT_DecoderClient_MessageHandler
{
public:
    // methods
    virtual ~BLT_DecoderClient_MessageHandler() {}

    virtual void OnAckNotification(BLT_DecoderServer_Message::CommandId /*id*/) {}
    virtual void OnNackNotification(BLT_DecoderServer_Message::CommandId /*id*/,
                                    BLT_Result                     /*result*/) {}
    virtual void OnPongNotification(const void* /*cookie*/) {}
    virtual void OnDecoderStateNotification(BLT_DecoderServer::State /*state*/) {}
    virtual void OnStreamTimeCodeNotification(BLT_TimeCode /*timecode*/) {}
    virtual void OnStreamPositionNotification(BLT_StreamPosition& /*position*/) {}
    virtual void OnStreamInfoNotification(BLT_Mask        /*update_mask*/, 
                                          BLT_StreamInfo& /*info*/) {}
    virtual void OnStreamPropertyNotification(const ATX_Property& /* property */) {}
};

/*----------------------------------------------------------------------
|   BLT_DecoderClient_Message
+---------------------------------------------------------------------*/
class BLT_DecoderClient_Message : public NPT_Message
{
public:
    // functions
    static NPT_Message::Type MessageType;
    NPT_Message::Type GetType() {
        return MessageType;
    }

    // methods
    virtual NPT_Result Deliver(BLT_DecoderClient_MessageHandler* handler) = 0;
    virtual NPT_Result Dispatch(NPT_MessageHandler* handler) {
        BLT_DecoderClient_MessageHandler* specific =
            dynamic_cast<BLT_DecoderClient_MessageHandler*>(handler);
        if (specific) {
            return Deliver(specific);
        } else {
            return DefaultDeliver(handler);
        }
    }
};

/*----------------------------------------------------------------------
|   BLT_DecoderClient_AckNotificationMessage
+---------------------------------------------------------------------*/
class BLT_DecoderClient_AckNotificationMessage :
    public BLT_DecoderClient_Message
{
public:
    // methods
    BLT_DecoderClient_AckNotificationMessage(
        BLT_DecoderServer_Message::CommandId id) :
        m_Id(id) {}
    NPT_Result Deliver(BLT_DecoderClient_MessageHandler* handler) {
        handler->OnAckNotification(m_Id);
        return NPT_SUCCESS;
    }

private:
    // members
    BLT_DecoderServer_Message::CommandId m_Id;
};

/*----------------------------------------------------------------------
|   BLT_DecoderClient_NackNotificationMessage
+---------------------------------------------------------------------*/
class BLT_DecoderClient_NackNotificationMessage :
    public BLT_DecoderClient_Message
{
public:
    // methods
    BLT_DecoderClient_NackNotificationMessage(
        BLT_DecoderServer_Message::CommandId id, BLT_Result result) :
        m_Id(id), m_Result(result) {}
    NPT_Result Deliver(BLT_DecoderClient_MessageHandler* handler) {
        handler->OnNackNotification(m_Id, m_Result);
        return NPT_SUCCESS;
    }

private:
    // members
    BLT_DecoderServer_Message::CommandId m_Id;
    BLT_Result                           m_Result;
};

/*----------------------------------------------------------------------
|   BLT_DecoderClient_PongNotificationMessage
+---------------------------------------------------------------------*/
class BLT_DecoderClient_PongNotificationMessage :
    public BLT_DecoderClient_Message
{
public:
    // methods
    BLT_DecoderClient_PongNotificationMessage(const void* cookie) :
        m_Cookie(cookie) {}
    NPT_Result Deliver(BLT_DecoderClient_MessageHandler* handler) {
        handler->OnPongNotification(m_Cookie);
        return NPT_SUCCESS;
    }

private:
    // members
    const void* m_Cookie;
};

/*----------------------------------------------------------------------
|   BLT_DecoderClient_DecoderStateNotificationMessage
+---------------------------------------------------------------------*/
class BLT_DecoderClient_DecoderStateNotificationMessage :
    public BLT_DecoderClient_Message
{
public:
    // methods
    BLT_DecoderClient_DecoderStateNotificationMessage(
        BLT_DecoderServer::State state) :
        m_State(state) {}
    NPT_Result Deliver(BLT_DecoderClient_MessageHandler* handler) {
        handler->OnDecoderStateNotification(m_State);
        return NPT_SUCCESS;
    }

private:
    // members
    BLT_DecoderServer::State m_State;
};

/*----------------------------------------------------------------------
|   BLT_DecoderClient_StreamTimeCodeNotificationMessage
+---------------------------------------------------------------------*/
class BLT_DecoderClient_StreamTimeCodeNotificationMessage :
    public BLT_DecoderClient_Message
{
public:
    // methods
    BLT_DecoderClient_StreamTimeCodeNotificationMessage(BLT_TimeCode time):
        m_TimeCode(time) {}
    NPT_Result Deliver(BLT_DecoderClient_MessageHandler* handler) {
        handler->OnStreamTimeCodeNotification(m_TimeCode);
        return NPT_SUCCESS;
    }

private:
    // members
    BLT_TimeCode m_TimeCode;
};

/*----------------------------------------------------------------------
|   BLT_DecoderClient_StreamPositionNotificationMessage
+---------------------------------------------------------------------*/
class BLT_DecoderClient_StreamPositionNotificationMessage :
    public BLT_DecoderClient_Message
{
public:
    // methods
    BLT_DecoderClient_StreamPositionNotificationMessage(
        BLT_StreamPosition& position): m_Position(position) {}
    NPT_Result Deliver(BLT_DecoderClient_MessageHandler* handler) {
        handler->OnStreamPositionNotification(m_Position);
        return NPT_SUCCESS;
    }

private:
    // members
    BLT_StreamPosition m_Position;
};

/*----------------------------------------------------------------------
|   BLT_DecoderClient_StreamInfoNotificationMessage
+---------------------------------------------------------------------*/
class BLT_DecoderClient_StreamInfoNotificationMessage :
    public BLT_DecoderClient_Message
{
public:
    // methods
    BLT_DecoderClient_StreamInfoNotificationMessage(BLT_Mask       update_mask,
                                                    BLT_StreamInfo& info):
        m_UpdateMask(update_mask), m_StreamInfo(info) {}
    NPT_Result Deliver(BLT_DecoderClient_MessageHandler* handler) {
        handler->OnStreamInfoNotification(m_UpdateMask, m_StreamInfo);
        return NPT_SUCCESS;
    }

private:
    // members
    BLT_Mask       m_UpdateMask;
    BLT_StreamInfo m_StreamInfo;
};

/*----------------------------------------------------------------------
|   BLT_DecoderClient_StreamPropertyNotificationMessage
+---------------------------------------------------------------------*/
class BLT_DecoderClient_StreamPropertyNotificationMessage :
    public BLT_DecoderClient_Message
{
public:
    // methods
    BLT_DecoderClient_StreamPropertyNotificationMessage(const ATX_Property& property) :
      m_PropertyWarpper(property) {}
    NPT_Result Deliver(BLT_DecoderClient_MessageHandler* handler) {
        handler->OnStreamPropertyNotification(m_PropertyWarpper.m_Property);
        return NPT_SUCCESS;
    }

private:
    // members
    BLT_DecoderServer_PropertyWrapper m_PropertyWarpper;
};

/*----------------------------------------------------------------------
|   BLT_DecoderClient
+---------------------------------------------------------------------*/
class BLT_DecoderClient : public NPT_MessageReceiver,
                          public NPT_MessageHandler,
                          public BLT_DecoderClient_MessageHandler
{
public:
    // methods
             BLT_DecoderClient(NPT_MessageQueue*   queue   = NULL,
                               NPT_MessageHandler* handler = NULL);
    virtual ~BLT_DecoderClient();

protected:
    // members
    NPT_MessageQueue* m_MessageQueue;
    bool              m_MessageQueueIsLocal;
};

#endif /* _BLT_DECODER_CLIENT_H_ */
