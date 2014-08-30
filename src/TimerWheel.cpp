#include "TimerWheel.h"
#include "LinuxTimer.h"
#include "TransactionContext.h"
#include "log.h"
#include <ostream>
#include <cassert>

TimerWheel::TimerWheel() : head_(0) {
    // empty
}

void TimerWheel::store_context(TransactionContext* const ctx) {
    assert(ctx);

    // Calculate bucket. Fine grained adjustments are not done to find the
    // bucket. For example, an exact 500ms expiry falls in bucket 1 rather than
    // zero even if add_context() at head = 0. Similarly other values are
    // rounded off. A more sophisticated system might store timestamp of next
    // expiry, take current timestamp and then decide which bucket to put.
    u32 bucket = head_ + ctx->expiry()/SYSTIMER_GRANULARITY_IN_MILLISECONDS;
    const u32 deviation = ctx->expiry()%SYSTIMER_GRANULARITY_IN_MILLISECONDS;
    bucket += (deviation > SYSTIMER_GRANULARITY_IN_MILLISECONDS/2) ? 1 : 0;
    bucket %= MAX_TIMER_BUCKETS;

    assert(bucket < MAX_TIMER_BUCKETS);

    // insert in correct bucket and also update this info in context
    std::pair<TransactionContextSet::iterator, bool> res = 
        contexts_[bucket].insert(std::make_pair(ctx->token(), ctx));
    assert(res.second);
    ctx->timerBucket(bucket);
}

void TimerWheel::remove_context(TransactionContext* ctx) {
    const u32 bucket = ctx->timerBucket();
    contexts_[bucket].erase(ctx->token());
}

void TimerWheel::expire_timers() {
    for (TransactionContextSet::iterator it = contexts_[head_].begin(); 
         it != contexts_[head_].end(); 
         ++it) {
        it->second->expire();
        LOG_INFO("async_call() timed out: " << *it->second << '\n');
    }
    contexts_[head_].clear();
    head_ = ++head_ % MAX_TIMER_BUCKETS;
}

std::ostream& operator<<(std::ostream& os, const TimerWheel& wheel) {
    os << "wheel(head: "  << wheel.head_ << '\n';
    
    for (int i = 0; i < MAX_TIMER_BUCKETS; ++i) {
        os << "bucket[" << i << "]: ";
        for (TransactionContextSet::const_iterator it = wheel.contexts_[i].begin(); 
             it != wheel.contexts_[i].end(); 
             ++it) {
            os << *(it->second) << ", ";
        }
        os << '\n';
    }
    os << ')';
    return os;        
}

// eof
