#ifndef _TRANSACTIONCONTEXT_H_
#define _TRANSACTIONCONTEXT_H_

#include "def_types.h"
#include <iosfwd>

/**
 * Possible transaction states */ 
enum eTransactionStatus {
    TX_ACTIVE,
    TX_EXPIRED,
    TX_INVALID
};

/**
 * Defines a transaction context, made to store information until async_call()
 * returns. Contains
 *
 * - a unique token associated context and all messages in that transaction
 * - expiry time (in milliseconds) from the point of creation
 * - transaction status
 * - bucket allotted by timer wheel @see TimerWheel
 *
 * The context may be cleaned up either on response message or timeout. 
 * */
class TransactionContext {

    u32 token_;                 
    u32 expiry_;
    eTransactionStatus status_;
    u32 timerBucket_;

public:
    TransactionContext(const u32 token, const u32 expiry): 
        token_(token), expiry_(expiry) , status_(TX_ACTIVE) {}

    const u32 token() const {
        return token_;
    }

    const u32 expiry() const {
        return expiry_;
    }

    eTransactionStatus status() const {
        return status_;
    }

    u32 timerBucket() const {
        return timerBucket_;
    }

    void expire() {
        status_ = TX_EXPIRED;
    };

    void timerBucket(const u32 bucket) {
        timerBucket_ = bucket;
    }

    friend std::ostream& operator<<(std::ostream& os, const TransactionContext ctx);

};

#endif /* _TRANSACTIONCONTEXT_H_ */
