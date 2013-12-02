
#ifndef __RESOLVE_MGR__
#define __RESOLVE_MGR__

#include <vector>
#include "event_q.h"

class Event; 
class EventReq; 
class ResolveThread;
class DispatcherCallback; 

class ResolveManager {

public:

    ResolveManager(); 
    virtual ~ResolveManager(); 

    void init(DispatcherCallback *iDispatcher, unsigned int iNumberOfThreads = 2); 
    void handleEvent(Event* iEvent); 

private:

    typedef std::vector<ResolveThread*> ResolveThreadsArray; 
    
    /**
     * The function will handle a request event. 
     * If a thread is available the function will 
     * allocate a thread which will work on request. 
     * If a thread is not available, the request will be 
     * pushed back to the event queue. 
     * 
     * @param iEvent The Event
     */
    void handleRequestEvent(EventReq *iEventReq); 

    ResolveThread* allocateFreeThread();  

    unsigned int m_threadsNumber;

    /// Memebers 
    EventQ m_resolveReqQ; 
    ResolveThreadsArray m_threadsPool; 


}; // ResolveManager

#endif // __RESOLVE_MGR__
