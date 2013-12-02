
#ifndef __EVENT_Q__
#define __EVENT_Q__

#include <boost/thread/mutex.hpp>
#include <deque>

class Event; 
class DispatcherCallback;

class EventQ {

public:

    EventQ(); 
    virtual ~EventQ(); 
    void init(); 

    bool isEmpty(); 
    void push(Event *iEvent); 
    Event* pop();

private:

    std::deque<Event*> m_eq; 
    DispatcherCallback *m_callback; 
    boost::mutex m_qMutex; 


}; // EventQ

#endif // __EVENT_Q__
