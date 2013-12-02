
#include <iostream>
#include <sstream>


#include "event_req.h" 
#include "log.h"

EventReq::EventReq(EventType iEventType, const std::string& iHostname) : Event(iEventType) 
{
    m_hostname = iHostname; 
    LOG("Creating new event: " << sprint()); 
}

EventReq::~EventReq()
{
    LOG("Delete event: " << sprint());
}

std::string EventReq::sprint() 
{
    std::ostringstream out; 
    out << Event::sprint() << ", data: " << m_hostname; 
    return out.str(); 
}
