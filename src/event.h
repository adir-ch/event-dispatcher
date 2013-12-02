#ifndef __EVENT__ 
#define __EVENT__

#include <string>
#include <sstream>

typedef enum { 
    RESOLVE_REQ,
    RESOLVE_RES,
    RESOLVE_ERR
} EventType;

class Event {

public:
    Event(EventType iEventType);
    virtual ~Event();

    virtual EventType type(); 
    virtual std::string sprint();
    virtual std::string getPresentation();

private:

    EventType m_type; 

}; // class end 

#endif // __EVENT__
