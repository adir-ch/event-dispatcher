
#include <sstream>
#include "event_err.h" 
#include "log.h"

EventErr::EventErr(EventType iEventType, const std::string& iMessage) : Event(iEventType) 
{
    LOG("Creating new event: " << sprint()); 
    m_message = iMessage; 
}

EventErr::~EventErr()
{
    LOG("Delete event: " << sprint());
}

std::string EventErr::sprint() 
{
    std::ostringstream out;
    out << Event::sprint() << " " << m_message; 
    return out.str(); 
}
