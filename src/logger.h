
#ifndef __LOGGER__
#define __LOGGER__

#include <string>
#include <iostream>

class Logger {

public:

    typedef enum {
        LOG_ERROR,
        LOG_INFO,
        LOG_DEBUG
    } LoggerLogLevel;

    static Logger* instance(); 
    virtual ~Logger() {}
    void log(LoggerLogLevel iLevel, const std::string& iLocation, std::ostream& iMessage);

private:

    Logger() {}
    inline std::string level2Name(LoggerLogLevel iLevel);

}; // Logger

#endif // __LOGGER__

