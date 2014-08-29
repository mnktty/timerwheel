#ifndef _TASK_H_
#define _TASK_H_

#include "TimerWheel.h"
#include <queue>
#include <map>
#include <pthread.h>


class Message;
class TransactionContext;
typedef std::map<u32, TransactionContext*> TransactionContextTable;

/**
 * An active object that blocks for tasks and runs to completion (one queue, one
 * thread of execution). For simulation sake, we make a call to async_call()  */
class Task {

    volatile bool active_;

    /* protected queue of messages */
    std::queue<Message*> queue_;
    pthread_mutex_t mutex_;
    pthread_cond_t  condition_;
    pthread_t thread_;
    
    /* own set of timers */
    TimerWheel timers_;
    
    /* active contexts */
    TransactionContextTable contexts_;
    
    /* dispatch message processing */
    void process(Message* msg);
    
    void processRequest(Message* msg);
    void processResponse(Message* msg);
    void processTimer(Message* msg);

public:

    /* Task are created active, for the time being */
    Task();
    ~Task();

    void stop() { active_ = false; }
    void join();

    void enqueue(Message* const msg);
    friend void* thread_func(void* arg);

};

#endif /* _TASK_H_ */
