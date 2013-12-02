
#include "dispatcher_callback.h"
#include "event.h"
#include "event_q.h"
#include "log.h"

EventQ::EventQ()
{
    LOG_DEBUG("Creating new event Q");
}

EventQ::~EventQ()
{
    LOG_DEBUG("Deleting event Q, size: " << m_eq.size());

    // deleting elements in the queue 
    Event *ev = NULL; 
    {
        boost::mutex::scoped_lock lock(m_qMutex);
        while (m_eq.empty() == false) {
            ev = m_eq.front();
            LOG_DEBUG("Removing pending event: " << ev->sprint());
            delete ev;
        } // while 
    } // scoped lock
}

void EventQ::init()
{
    
}

bool EventQ::isEmpty()
{
    boost::mutex::scoped_lock lock(m_qMutex);
    return m_eq.empty();
}

void EventQ::push(Event *iEvent)
{
    LOG_DEBUG("Adding a new event to the queue: " << iEvent->sprint()); 
    boost::mutex::scoped_lock lock(m_qMutex);
    m_eq.push_back(iEvent);
}

Event* EventQ::pop()
{
    Event *ev = NULL; 
    boost::mutex::scoped_lock lock(m_qMutex);
    ev = m_eq.front();
    m_eq.pop_front();
    return ev;
}
