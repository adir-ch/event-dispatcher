
#ifndef __EVENT_REQ__ 
#define __EVENT_REQ__

#include <string>
#include "event.h"

class EventReq : public Event {

public:
    EventReq(EventType iEventType, const std::string& iHostname); 
    virtual ~EventReq(); 
    std::string getHostName() { return m_hostname; }
    std::string sprint(); 

private:
    
    std::string m_hostname; 

}; // class end 

#endif // __EVENT__
