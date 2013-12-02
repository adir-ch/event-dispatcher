
#ifndef __EVENT_RES__ 
#define __EVENT_RES__

#include <string>
#include "event.h"

class EventRes : public Event {

public:
    EventRes(EventType iEventType, const std::string& iHostname, unsigned long iIpAddress); 
    virtual ~EventRes(); 
    virtual std::string getPresentation();
    std::string sprint(); 

private:
    
    std::string m_hostname; 
    int m_resolvedIp; 

}; // class end 

#endif // __EVENT__
