
#ifndef __DISPATHCER_CALLBACK_H__
#define __DISPATHCER_CALLBACK_H__

class Event; 

class DispatcherCallback {

public:

    DispatcherCallback() {}
    virtual ~DispatcherCallback() {} 
    virtual void registerNewEvent(Event *iEvent) = 0; 

}; // DispatcherCallback

#endif // __DISPATHCER_CALLBACK_H__