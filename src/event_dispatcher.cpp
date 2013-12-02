
#include "log.h"
#include "event_dispatcher.h"
#include "event.h"
#include "ui.h"

EventDispatcher::EventDispatcher()
{
    LOG_DEBUG("Creating a new event dispatcher");
}

EventDispatcher::~EventDispatcher()
{
    // Nothing to do
}

void EventDispatcher::run()
{
    LOG("Starting Event dispatcher...\n\n");

    // dispatcher main loop 
    while (true) {
        {
            // the mutex MUST be before checking if the Q is empty 
            // to prevent a situation where a thread will call notify before the dispatcher 
            // managed to call cond wait - meaning, new events wait in the Q, but the dispatcher 
            // waits for a notify - dead lock!
            // Placing the mutex before the IF will prevent from the dispatcher to call cond wait because 
            // the Q is not empty. 
            //LOG_DEBUG("adir: tring to lock wait mutex");
            boost::unique_lock<boost::mutex> lock(m_eventsMutex); 
            //LOG_DEBUG("adir: tring to lock wait mutex - locked!");
            if (m_eq.isEmpty() == true) { // waiting for new events 
                LOG_DEBUG("No events to dispatch, waiting...");
                m_waitForEventsCond.wait(lock);
            }

        } // scoped lock mutex 
        dispatch(); // dispatch events

    } // while end 
} 

void EventDispatcher::init(UI *iUIAdapter)
{
    m_ui = iUIAdapter; 
    m_eq.init();
    m_rm.init(this);
}

void EventDispatcher::registerNewEvent(Event *iEvent)
{
    LOG_DEBUG("Received new event: " << iEvent->sprint());
    m_eq.push(iEvent);
    
    // new event has been added to the Q, notifing dispatcher. 
    {
        //LOG_DEBUG("adir 2: tring to lock mutex");
        boost::lock_guard<boost::mutex> lock(m_eventsMutex); 
        //LOG_DEBUG("adir 2: tring to lock mutex - locked!");
        LOG_DEBUG("New events are waiting in the Q, notifying dispatcher");
        m_waitForEventsCond.notify_one(); // will release the dispatcher cond wait and will dispatch
    }
}

void EventDispatcher::dispatch()
{
    Event *ev = m_eq.pop(); 

    if (ev == NULL) {
        LOG_ERROR("NULL event received to dispatch - aborting!");
        return;
    }

    LOG_DEBUG("Dispatching event: " << ev->sprint());

    switch (ev->type()) {
    case RESOLVE_REQ: 
        // all the resolver threds are working, need to delay  
        m_rm.handleEvent(ev);
        break;
    case RESOLVE_RES:
    case RESOLVE_ERR:
        m_ui->handleEvent(ev);
        break;
    default:
        LOG_ERROR("Unknown event detected: " << ev->sprint());
    } // case 
}
