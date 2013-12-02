
#include <boost/bind.hpp>
#include <sstream>
#include <netdb.h>
#include <arpa/inet.h>

#include "dispatcher_callback.h"
#include "resolve_thread.h"
#include "event_q.h"
#include "event_req.h"
#include "event_res.h"
#include "event_err.h"
#include "log.h"


// thread specific log 
#define THREAD_LOG(msg) LOG("THREAD ("<< m_threadID <<") " << msg) 
#define THREAD_DEBUG(msg) LOG_DEBUG("THREAD ("<< m_threadID <<") " << msg) 
#define THREAD_ERROR(msg) LOG_ERROR("THREAD ("<< m_threadID <<") " << msg) 


ResolveThread::ResolveThread()
{

}

ResolveThread::~ResolveThread()
{

}

void ResolveThread::init(DispatcherCallback *iCallback, EventQ *iReqQ, unsigned int iThreadId)
{
    m_dispatcher = iCallback;
    m_lastJobTimestamp = -1; 
    m_isFree = true; 
    m_keepRunning = false; 
    m_threadID = iThreadId; 
    m_reqQ = iReqQ;
}

void ResolveThread::start()
{
    THREAD_LOG("Starting thread"); 
    m_keepRunning = true; 
    
    m_thread = new boost::thread(boost::bind(&ResolveThread::run, this));
}

void ResolveThread::stop()
{
    // signal thread loop, and stop it. 
    m_keepRunning = false; 
    
    // signal thread
    boost::mutex::scoped_lock lock(m_threadMutex); 
    m_threadCond.notify_one();
}

void ResolveThread::wakeup()
{
    //boost::mutex::scoped_lock lock(m_threadMutex); 
    THREAD_DEBUG("Got wakeup call");
    m_threadCond.notify_one();
}

void ResolveThread::join()
{
    //m_thread->join(); 
}

void ResolveThread::resolve(const std::string& iHostname)
{
    // invoke gethostbyname and register a new event
    struct in_addr reslovedAddress;
    struct hostent *hp = gethostbyname(iHostname.c_str());

    if (hp == NULL) {
        std::string message("Resolver: unable to reslove ip address of: " + iHostname); 
        THREAD_ERROR(message);
        EventErr *ev = new EventErr(RESOLVE_ERR, message); 
        m_dispatcher->registerNewEvent(ev);
        return;
    }

    unsigned int ipListCounter = 0;
    //while ( hp -> h_addr_list[ipListCounter] != NULL && hp->h_addr_list[ipListCounter] != NULL) {

        reslovedAddress = *( struct in_addr* ) (hp -> h_addr_list[ipListCounter]);
        THREAD_DEBUG("Resolver: resolved: name: " << iHostname << " ip: " << inet_ntoa(reslovedAddress));
        //oIpAddressesList.push_back(IpAddr(ntohl(reslovedAddress.s_addr)));
        //ipListCounter++;
    //}

    EventRes *ev = new EventRes(RESOLVE_RES, iHostname, reslovedAddress.s_addr); 
    m_dispatcher->registerNewEvent(ev);
}

bool ResolveThread::isFree()
{
    THREAD_DEBUG("Checking free state");
    boost::mutex::scoped_lock lock(m_threadMutex);
    return m_isFree; 
}

void ResolveThread::setFreeState(bool iNewState)
{
    boost::mutex::scoped_lock lock(m_threadMutex);
    m_isFree = iNewState;
}

std::string ResolveThread::sprint()
{
    std::ostringstream out; 
    out << "Thread ID: " << m_threadID << " State: " << (m_isFree ? "Free":"Busy"); 
    return out.str(); 
}

void ResolveThread::run()
{
    THREAD_DEBUG("Thread is starting");
    setFreeState(true);
    bool hasMoreJob = false; 

    while (m_keepRunning == true) {

        if (hasMoreJob == false ) { // when the thread finishs it should check if mroe event are waiting in the Q
                                    // and handle it as well, hence skipping on the condition wait is needed. 
            {
                boost::mutex::scoped_lock lock(m_threadMutex); 
                m_isFree = true; 
                THREAD_DEBUG("Thread is waiting for events"); 
                m_threadCond.wait(lock);
                THREAD_DEBUG("Thread - wait notify called - thread will start work...");
                m_isFree = false; 
            } // scoped lock

            if (m_keepRunning == false) {
                THREAD_DEBUG("Resolve thread has been stopped");
                break;
            }
        }

        EventReq *ev = (EventReq*) m_reqQ->pop();
        if (ev == NULL) {
            THREAD_ERROR("got NULL event from Q - aborting");
            continue;
        }

        THREAD_DEBUG("Got request to resolve hostname: " << ev->getHostName());
        resolve(ev->getHostName()); 
        hasMoreJob = (m_reqQ->isEmpty() == false); 
    }

    // thread was stopped - exit thread 
}

