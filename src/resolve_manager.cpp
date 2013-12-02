
#include "resolve_manager.h"
#include "resolve_thread.h"
#include "event.h"
#include "event_req.h"
#include "log.h"

ResolveManager::ResolveManager()
{
    LOG_DEBUG("Creating a new resolve manager");
}

ResolveManager::~ResolveManager()
{
    LOG_DEBUG("Deleting resolve manager");

    ResolveThreadsArray::iterator it; 
    ResolveThreadsArray::iterator itend = m_threadsPool.end(); 
    for (it = m_threadsPool.begin(); it != itend; it++) {
        (*it)->stop(); // first - stopping the thread  
        (*it)->join();
        delete *it;  
    }
}

void ResolveManager::init(DispatcherCallback *iDispatcher, unsigned int iNumberOfThreads)
{
    LOG("Init Resolve manager, pool size: " << iNumberOfThreads);
    m_threadsNumber = iNumberOfThreads;
    ResolveThread *current = NULL;  
    for (unsigned int currentThread = 0; currentThread < m_threadsNumber; currentThread++) {
        current = new ResolveThread(); 
        m_threadsPool.push_back(current); 
        current->init(iDispatcher, &m_resolveReqQ, currentThread);
        current->start(); 
        current->join(); 
    }
}

void ResolveManager::handleEvent(Event *iEvent)
{
    LOG("Handle new event, type: " << iEvent->sprint()); 

    switch (iEvent->type()) {
    case RESOLVE_REQ:
        handleRequestEvent((EventReq*)iEvent); 
        break;
    default:
        LOG_ERROR("Cannot handle event, type: " << iEvent->type());
    }
}

void ResolveManager::handleRequestEvent(EventReq *iEventReq)
{
    // put the event in the Q, and try to allocate a thread for it. 
    // if a thread is not available, the event will wait in the Q
    // the first available thread will pop the request from the Q. 

    LOG_DEBUG("Handling req event: " << iEventReq->sprint());

    // allocate a thread to resolve or push the event to the queue 

    // First push the event to the Q
    m_resolveReqQ.push(iEventReq); 

    // Allocate an available thread, and if any, wake it up. Th thread will pop 
    // the event from the Q and will do some work. To prevent a scenario where 
    // the same thread always get selected, allocateFreeThread will return the 
    // an available thread according to a pre defined condition. It is needed
    // in a case where each thread is running on a different CPU core (so we don't want 
    // to make the same core work and the others to rest because the manager always selects 
    // the same thread.  
    ResolveThread *thread = allocateFreeThread(); 
    if (thread != NULL) { // an available thread found, 
        thread->wakeup(); 
    }

    // if no thread was found, just leave the request in the Q, the next available thread will 
    // handle it. 
}

ResolveThread* ResolveManager::allocateFreeThread()
{
    // for now, just return the first free thread
    ResolveThreadsArray::iterator it; 
    ResolveThreadsArray::iterator itend = m_threadsPool.end(); 
    for (it = m_threadsPool.begin(); it != itend; it++) {
        if ((*it)->isFree()) {
            LOG_DEBUG("Found: " << (*it)->sprint());
            return *it; // return the first available thread
        }
    }

    LOG("All threads are currently busy"); 
    return NULL; 
}

