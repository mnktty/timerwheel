#ifndef _ASYNCCALL_H_
#define _ASYNCCALL_H_

/**
 * Model the async call api(). Returns response after a random time (time varies
 * on work load) */

class Message;
class Task;

void async_call(Message* const request, Task* caller);


#endif /* _ASYNCCALL_H_ */
