
#ifndef __UI__
#define __UI__

#include "ui_callback.h"

class InputHandler;
class DispatcherCallback; 
class Event; 

class UI : public UICallback {

public:
    UI(); 
    virtual ~UI(); 

    void init(DispatcherCallback *iDispatcher); 

    /**
     * Handle UI events from the resolver. Will print the result on 
     * the screen. 
     * 
     * @param iEvent The resut event.
     */
    void handleEvent(Event *iEvent); 

    // dispatch the events in the internal event queue
    bool dispatch(); 

private:

    void handleResponseEvent(Event *iEvent); 
    void handleErrorEvent(Event *iEvent); 


    /**
     * Handle input events from IO
     * 
     * @param iInput The hostname to resolve
     */
    void handleInput(const std::string& iInput); 
    bool validateHostname(const std::string& iHostname);

    InputHandler *m_inputHandler; 
    DispatcherCallback *m_dispatcher; 
    
}; // class UI 

#endif // __UI__
