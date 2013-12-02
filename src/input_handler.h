
#ifndef __INPUT_HANDLER__
#define __INPUT_HANDLER__

class UICallback;

class InputHandler {

public:

    InputHandler();
    virtual ~InputHandler(); 
    void init(UICallback *iCallback); 

protected: 

    bool socketRecv(int iSocketFD);
    virtual void handleInput(); 

    UICallback *m_ui;

}; // class InputHandler

#endif // __INPUT_HANDLER__ 

