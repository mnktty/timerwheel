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
 * thread of execution). For simulation sake, we make a call to async_call().
 *
 * Messages are enqueued to the task. To stop the task, a special message type
 * called MSG_SHUTDOWN is enqueued. */
class Task {

    volatile bool active_;

    /**
     * atomic queue of messages */
    std::queue<Message*> queue_;
    pthread_mutex_t mutex_;
    pthread_cond_t  condition_;

    /**
     * thread handle */
    pthread_t thread_;
    
    /**
     * own set of timers */
    TimerWheel timers_;
    
    /**
     * All active contexts.  */
    TransactionContextTable contexts_;
    
    /**
     * Dispatch message processing */
    void process(Message* msg);

    void processRequest(Message* msg);
    void processResponse(Message* msg);
    void processTimer(Message* msg);

public:

    /**
     * Create task, LWP not yet spawned */
    Task();

    /**
     * Cleanup */
    ~Task();

    void start();

    /**
     * Call this to ensure task LWP has completed. @warning Always ensure that a
     * MSG_SHUTDOWN is sent *before* calling this. Or the caller would wait
     * indefinitely. */
    void join();

    void enqueue(Message* const msg);
    friend void* consume(void* arg);

};

#endif /* _TASK_H_ */
