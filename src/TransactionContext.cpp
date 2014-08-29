#include "TransactionContext.h"
#include <ostream>

std::ostream& operator<<(std::ostream& os, const TransactionContext ctx) {
    
    os << "ctx:(" << ctx.token_ << ',' << ctx.expiry_ << ',' << ctx.status_ << ')';
    return os;
}

// eof
