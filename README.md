timerwheel
==========

Simulation of a timerwheel used to manage expiring transaction contexts. 

A set of (concurrent) tasks make an "async_call()" which returns the result at
varying times. Sometimes, it is too late, and the transaction contexts
expire. Periodic timer ticks (from a single OS timer) are multiplexed to a
timerwheel per task.

