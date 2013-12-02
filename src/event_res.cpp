

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include "event_res.h" 
#include "log.h"

EventRes::EventRes(EventType iEventType, const std::string& iHostname, unsigned long iIpaddress) : Event(iEventType) 
{
    m_hostname = iHostname; 
    m_resolvedIp = iIpaddress;  
    LOG_DEBUG("Creating new event: " << sprint()); 
}

EventRes::~EventRes()
{
    LOG("Delete event: " << sprint());
}


std::string EventRes::getPresentation()
{
    struct sockaddr_in addr_in; 
    in_addr addr;
    addr.s_addr = m_resolvedIp;// htonl(m_resolvedIp); 
    addr_in.sin_addr = addr;
    std::ostringstream out;
    out << m_hostname << " IP: " << inet_ntoa(addr_in.sin_addr);
    return out.str(); 
}

std::string EventRes::sprint() 
{
    std::ostringstream out;
    if (m_resolvedIp == 0) {
        out << Event::sprint() << " " << m_hostname << " IP: not resolved"; 
    } else {
        out << Event::sprint() << " " << m_hostname << " IP: " << m_resolvedIp; 
    }

    return out.str(); 
}
