#include "LinuxTimer.h"
#include "TransactionContext.h"
#include "TimerWheel.h"

#include "Task.h"
#include "Message.h"

#include <signal.h>
#include <cstdlib>
#include <cassert>
#include <iostream>

/** Check if correct buckets are selected */
void testTimerWheel() {
    typedef std::vector<TransactionContext*> TxCache;
    // purely to cache the pointers so that they can be freed later
    TxCache cache; 

    TimerWheel wheel;
    for (int i = 0; i < 100; ++i)
    {
        u32 expiry = random()%2000;
        TransactionContext* ctx = new TransactionContext(i, expiry);
        cache.push_back(ctx);
        wheel.store_context(ctx);
    }
    std::cout << wheel << '\n';
    wheel.expire_timers();
    
    TransactionContext d(101, 500);
    wheel.store_context(&d);
    std::cout << wheel << '\n';

    for (TxCache::iterator it = cache.begin(); it != cache.end(); ++it) {
        delete *it;
    }
}

/**
 * Check if context expiry with timerwheel works as expected.
 */
void testContextHandling() {
    Task task;
    task.start();

    u32 token = 1;
    Message* m;

    m = new Message(token, MSG_REQUEST);
    task.enqueue(m);
    m = new Message(token, MSG_RESPONSE);
    task.enqueue(m);

    ++token;

    // second message has no matching response
    m = new Message(token, MSG_REQUEST);
    task.enqueue(m);

    ++token;
    m = new Message(token, MSG_REQUEST);
    task.enqueue(m);
    m = new Message(token, MSG_RESPONSE);
    task.enqueue(m);

    // context for the second message will expire before it gets a response
    m = new Message(0, MSG_TIMER);
    task.enqueue(m);
    m = new Message(0, MSG_TIMER);
    task.enqueue(m);
    m = new Message(0, MSG_TIMER);
    task.enqueue(m);

    m = new Message(0, MSG_SHUTDOWN);
    task.enqueue(m);

    task.join();
}


int main(int argc, char *argv[])
{
    testTimerWheel();
    testContextHandling();
    return 0;
}

// eof
