
#ifndef __RESOLVE_THREAD__
#define __RESOLVE_THREAD__

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <string>

class DispatcherCallback; 
class EventQ; 

class ResolveThread {

public:

    ResolveThread(); 
    virtual ~ResolveThread(); 

    void init(DispatcherCallback *iDispatcher, EventQ *iReqQ, unsigned int iThreadID); 
    void start(); 
    void stop(); 
    void wakeup(); 
    void join(); 
    void resolve(const std::string& iHostname); 
    bool isFree();
    void setFreeState(bool iNewTest); 
    std::string sprint(); 


private:

    void run(); 

    DispatcherCallback *m_dispatcher; 
    unsigned long long m_lastJobTimestamp; 
    bool m_isFree; 
    unsigned int m_threadID; 
    bool m_keepRunning; 
    EventQ *m_reqQ; 
    boost::mutex m_threadMutex; 
    boost::condition_variable m_threadCond;

    boost::thread *m_thread; 

}; // class ResolveThread

#endif // __RESOLVE_THREAD__
