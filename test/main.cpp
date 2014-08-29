#include "LinuxTimer.h"
#include "TransactionContext.h"
#include "TimerWheel.h"

#include "Task.h"
#include "Message.h"

#include <signal.h>
#include <cstdlib>
#include <iostream>

void testTimerWheel() {
    // Check if correct buckets are selected

    TimerWheel wheel;
    for (int i = 0; i < 100; ++i)
    {
        u32 expiry = random()%2000;
        TransactionContext* ctx = new TransactionContext(i, expiry);
        wheel.store_context(ctx);
    }
    std::cout << wheel << '\n';
    wheel.expire_timers();
    
    TransactionContext d(101, 500);
    wheel.store_context(&d);
    std::cout << wheel << '\n';
}

void testContextHandling() {
    Task task;
    u32 token = 1;
    Message* m;

    m = new Message(token, MSG_REQUEST);
    task.enqueue(m);
    m = new Message(token, MSG_RESPONSE);
    task.enqueue(m);

    ++token;

    m = new Message(token, MSG_REQUEST);
    task.enqueue(m);

    ++token;
    m = new Message(token, MSG_REQUEST);
    task.enqueue(m);
    m = new Message(token, MSG_RESPONSE);
    task.enqueue(m);

    m = new Message(0, MSG_TIMER);
    task.enqueue(m);
    m = new Message(0, MSG_TIMER);
    task.enqueue(m);
    m = new Message(0, MSG_TIMER);
    task.enqueue(m);
    
    task.join();
}


int main(int argc, char *argv[])
{
    // testTimerWheel();
    testContextHandling();
    return 0;
}

// eof
