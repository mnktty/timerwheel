#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "def_types.h"

enum eMessageType {
    MSG_REQUEST,
    MSG_RESPONSE,
    MSG_TIMER,
    MSG_SHUTDOWN                /* sent to stop the task */
};


/**
 * For communication */
class Message {
    u32 token_;                 /**< Transaction token (shared by Context) */
    eMessageType msgType_;

public:

    Message(const u32 token, eMessageType msgType): token_(token), msgType_(msgType) {}

    const u32 token() const {
        return token_;
    }

    eMessageType msgType() const {
        return msgType_;
    }
  
};



#endif /* _MESSAGE_H_ */
