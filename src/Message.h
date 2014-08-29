#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "def_types.h"

enum eMessageType {
    MSG_REQUEST,
    MSG_RESPONSE,
    MSG_TIMER,
    MSG_FINAL
};


/**
 * For communication */
class Message {
    u32 token_;                 /**< Transaction token (shared by Context) */
    eMessageType msgType_;

public:

    Message(const u32 token, eMessageType msgType): token_(token), msgType_(msgType) {}

    bool isRequest() const {
        return MSG_REQUEST == msgType_;
    }
    
    bool isResponse() const {
        return MSG_RESPONSE == msgType_;
    }

    bool isTimer() const {
        return MSG_TIMER == msgType_;
    }

    const u32 token() const {
        return token_;
    }
  
};



#endif /* _MESSAGE_H_ */
