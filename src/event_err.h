
#ifndef __EVENT_EEE__ 
#define __EVENT_ERR__

#include <string>
#include "event.h"

class EventErr : public Event {

public:
    EventErr(EventType iEventType, const std::string& iHostname); 
    virtual ~EventErr(); 
    std::string sprint(); 

private:
    
    std::string m_message; 

}; // class end 

#endif // __EVENT__
