#include "Task.h"
#include "Message.h"
#include <TransactionContext.h>
#include "AsyncCall.h"
#include "log.h"
#include <cassert>

namespace {
    const u32 UPPER_WATERMARK = 100; // Let the queue not grow beyond this size !
}

// the thread function
void* consume(void* arg) {
    Task* task = static_cast<Task*>(arg);
    task->active_ = true;
    assert(task && task->active_);
    
    while(task->active_) {
        ::pthread_mutex_lock(&task->mutex_);
        while (task->queue_.empty()) {
            LOG_TRACE("cs");
            ::pthread_cond_wait(&task->condition_, &task->mutex_);
            LOG_TRACE("cw");
        }

        Message* msg = task->queue_.front();
        task->queue_.pop();
        LOG_TRACE("-");
        
        ::pthread_cond_signal(&task->condition_);
        ::pthread_mutex_unlock(&task->mutex_);
    
        task->process(msg);
    }
}

Task::Task() : active_(false) {
    int status;

    status = ::pthread_mutex_init(&mutex_, NULL);
    assert(0 == status);

    status = ::pthread_cond_init(&condition_, NULL);
    assert(0 == status);
}

Task::~Task() {

    // clear all active contexts
    for (TransactionContextTable::iterator it = contexts_.begin(); 
         it != contexts_.end(); ++it) {
        delete it->second;
        it->second = 0;
    }

    // clear any pending messages (to be processed)
    while(!queue_.empty()) {
        Message* msg = queue_.front();
        delete msg;
        msg = 0;
        queue_.pop();
    }

    int status;
    status = ::pthread_cond_destroy(&condition_);
    assert(0 == status);

    status = ::pthread_mutex_destroy(&mutex_);
    assert(0 == status);
}

void Task::start() {
    int status = ::pthread_create(&thread_, NULL, consume, static_cast<void*>(this));
    assert(0 == status);
}

void Task::join() {
    int status = ::pthread_join(thread_, NULL);
    assert(0 == status);
}

void Task::enqueue(Message* msg) {
    ::pthread_mutex_lock(&mutex_);
    while (queue_.size() > UPPER_WATERMARK) {
        LOG_TRACE("ps");
        ::pthread_cond_wait(&condition_, &mutex_);
        LOG_TRACE("pw");
    }

    queue_.push(msg);
    LOG_TRACE("+");

    ::pthread_cond_signal(&condition_);
    ::pthread_mutex_unlock(&mutex_);
}

void Task::process(Message* msg) {

    switch(msg->msgType()) {
    case MSG_REQUEST: processRequest(msg);
        break;
        
    case MSG_RESPONSE: processResponse(msg);
        break;

    case MSG_TIMER: processTimer(msg);
        break;

    case MSG_SHUTDOWN: active_ = false;
        break;

    default:                                 
        break;

    }
    // finally consume it
    delete msg;
    msg = 0;
}

void Task::processRequest(Message* msg) {
    TransactionContext* ctx = new TransactionContext(msg->token(), 100);
    std::pair<TransactionContextTable::iterator, bool> res = contexts_.insert(std::make_pair(msg->token(), ctx));
    assert(res.second);
    timers_.store_context(ctx);

    // NOTE: async semantics can be simulated with a message sequence for
    // testing. Insert the real async call here
    
    // async_call(msg, this);
}

void Task::processResponse(Message* msg) {
    TransactionContextTable::iterator it = contexts_.find(msg->token());
    assert (it != contexts_.end());
    TransactionContext* ctx = it->second;
    assert(ctx);

    switch(ctx->status()) {
    case TX_ACTIVE:
        LOG_INFO("async_call() succeeded: " << *ctx << '\n');
        break;
        
    case TX_EXPIRED:
        LOG_INFO("async_call() timed out: " << *ctx << '\n');
        break;

    default:
    case TX_INVALID:
        break;
    }

    // remove context from all places - timers_ and contexts_
    timers_.remove_context(ctx);
    delete ctx;
    ctx = 0;
    contexts_.erase(it);
}

void Task::processTimer(Message* msg) {
    timers_.expire_timers();
}

// eof
