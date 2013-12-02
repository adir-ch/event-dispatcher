
#include <stdio.h>
#include <boost/thread/mutex.hpp>
#include <boost/format.hpp>
#include "logger.h"

Logger *gLogger = NULL; 
boost::mutex gLoggerMutex; 

Logger* Logger::instance()
{
    if (gLogger == NULL) {
        printf("Creating a new logger instance\n"); 
        gLogger = new Logger(); 
    }

    return gLogger;
}

void Logger::log(LoggerLogLevel iLevel, const std::string& iLocation, std::ostream& iMessage)
{
    std::ostringstream& message = dynamic_cast<std::ostringstream&>(iMessage);
    

    boost::mutex::scoped_lock lock(gLoggerMutex);
    std::cout << boost::format("%|1$-10s| %|2$-30s| %|100t| %3%\n") % level2Name(iLevel) % iLocation % message.str();
}

inline std::string Logger::level2Name(LoggerLogLevel iLevel)
{

    switch (iLevel) {
    case LOG_ERROR:
        return "ERROR"; 
    case LOG_INFO: 
        return "INFO";
    case LOG_DEBUG: 
        return "DEBUG"; 
    default:
        return "LOG";
    }
}
