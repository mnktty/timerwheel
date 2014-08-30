#include "AsyncCall.h"
#include "Message.h"
#include "Task.h"
#include <cstdlib>
#include <cassert>
#include <unistd.h>

void async_call(Message* const request, Task* const caller) {
    // FIXME: not fully implemented
    assert(request && caller);

    useconds_t busy = (random() % 10) * 100*1000;
    usleep(busy);
    
    if (MSG_REQUEST == request->msgType()) {
        Message* response = new Message(request->token(), MSG_RESPONSE);
        caller->enqueue(response);
    }
    
    delete request;
}

// eof
