
#include <boost/regex.hpp>
#include <boost/array.hpp>
#include<boost/tokenizer.hpp>
#include <iostream>
#include<string>

#include "ui.h"
#include "log.h"
#include "event.h"
#include "event_req.h"
#include "input_handler.h"
#include "dispatcher_callback.h"


UI::UI()
{
    // do nothing 
}

UI::~UI()
{
    // do nothing 
}

void UI::init(DispatcherCallback *iDispatcher)
{
    LOG("Init UI");
    m_dispatcher = iDispatcher;
    m_inputHandler = new InputHandler(); 
    m_inputHandler->init(this);
}

bool UI::dispatch()
{
    return false; // no job has done 
}

void UI::handleEvent(Event *iEvent)
{
    LOG("Handle new event, type: " << iEvent->sprint()); 

    switch (iEvent->type()) {
    case RESOLVE_RES:
        handleResponseEvent(iEvent); 
        break;
    case RESOLVE_ERR:
        handleErrorEvent(iEvent); 
        break;
    default:
        LOG_ERROR("Cannot handle event, type: " << iEvent->sprint());
    }
}

void UI::handleResponseEvent(Event *iEvent)
{
    printf("Resolved: %s\n", iEvent->getPresentation().c_str()); 
    delete iEvent; 
}

void UI::handleErrorEvent(Event *iEvent)
{
    printf("Cannot resolve: %s\n", iEvent->getPresentation().c_str()); 
    delete iEvent; 
}

void UI::handleInput(const std::string& iInput)
{
    LOG("Received new data from Input: " << iInput); 

    // parsing the data 
    boost::tokenizer<> token(iInput);

    for(boost::tokenizer<>::iterator it=token.begin(); it!=token.end(); ++it){
        LOG_DEBUG("Working on token: " << it.current_token());
        if (validateHostname(it.current_token()) == true) {
            // create a new hostname req event and send it to the main event Q
            LOG_DEBUG("Creating new event");
            m_dispatcher->registerNewEvent(new EventReq(RESOLVE_REQ, it.current_token())); 
        }
    }
}

bool UI::validateHostname(const std::string& iHostname)
{
    LOG_DEBUG("Validating hostname string: " << iHostname);
    boost::regex hostnamePattern("^([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\-]{0,61}[a-zA-Z0-9])(\.([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\-]{0,61}[a-zA-Z0-9]))*$");
    boost::smatch matches;
    if (boost::regex_match(iHostname, matches, hostnamePattern)) {
        LOG_DEBUG("Hostname is valid: " << iHostname);
        return true;
    }

    LOG_ERROR("Illegal hostname: " << iHostname);
    return false; 
}

