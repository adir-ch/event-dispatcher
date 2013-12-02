
#ifndef __EVENT_DISPATCHER_H__
#define __EVENT_DISPATCHER_H__

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include "log.h"
#include "dispatcher_callback.h"
#include "resolve_manager.h"
#include "event_q.h"

class UI; 

class EventDispatcher : public DispatcherCallback {

public:

    EventDispatcher(); 
    virtual ~EventDispatcher(); 

    void run(); 
    void init(UI *iUIAdapter);
    void registerNewEvent(Event *iEvent); 
    DispatcherCallback* getDispatcherCallback() { return this;  }

private:
    
    void dispatch(); 
        
    UI *m_ui; 
    ResolveManager m_rm; 
    EventQ m_eq; 

    boost::mutex m_eventsMutex; 
    boost::condition_variable m_waitForEventsCond;


}; // EventDispatcher

#endif // __EVENT_DISPATCHER_H__
