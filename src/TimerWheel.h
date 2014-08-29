#ifndef _TIMERWHEEL_H_
#define _TIMERWHEEL_H_

#include "def_types.h"
#include <map>
#include <iosfwd>

const int MAX_TIMER_BUCKETS = 7;

class TransactionContext;
typedef std::map<u32, TransactionContext*> TransactionContextSet;


/**
 * Defines a timer wheel, where timers to expire are put in buckets based on the
 * time to expire from now. */
class TimerWheel {
    /**
     * set of transactions sorted per expiry bucket */
    TransactionContextSet contexts_[MAX_TIMER_BUCKETS];
    /**
     * timers in head will be expired on expire_timers() */
    u32 head_;
    
public:
    TimerWheel();

    /**
     * (intelligently) Add context to the correct bucket, based on
     * expiry. Assume user does not give expiry greater than MAX_TIMER_BUCKETS*
     * base_frequency
     *
     * @param[in] ctx the transaction context
     * @param[in] time (in milliseconds) to expire from now
     * */
    void store_context(TransactionContext* const ctx);
    
    /**
     * Remove a specific context */
    void remove_context(TransactionContext* ctx);

    /**
     * Called (by task) when timer message is received */
    void expire_timers();

    /**
     * Placeholder  */
    void clear();

    friend std::ostream& operator<<(std::ostream& os, const TimerWheel& wheel);
};

#endif /* _TIMERWHEEL_H_ */
