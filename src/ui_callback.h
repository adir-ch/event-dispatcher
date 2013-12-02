
#ifndef __UI_CALLBACK_H__
#define __UI_CALLBACK_H__

#include <string>

class UICallback {

public:

    UICallback() {} 
    virtual ~UICallback() {} 

    virtual void handleInput(const std::string& iInput) = 0; 

}; // UICallback

#endif // __UI_CALLBACK_H__
