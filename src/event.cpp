
#include <sstream>
#include <string>

#include "event.h"
#include "log.h"



Event::Event(EventType iEventType) 
{ 
    m_type = iEventType; 
}

Event::~Event() 
{

}

EventType Event::type()
{
    return m_type;
}

std::string Event::sprint() 
{ 
    std::ostringstream out; 
    out << "Type: " << getPresentation(); 
    return out.str(); 
}

std::string Event::getPresentation() 
{ 
    switch (m_type) {
    case RESOLVE_REQ:
        return "Resolve Req";
    case RESOLVE_RES: 
        return "Resolve Res";
    case RESOLVE_ERR: 
        return "Resolve ERR!";
    }

    return "Unknown";  
} 

