
#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <iostream>
#include <sstream> 
#include "logger.h"

#ifndef NO_LOG_LOW

#define LOG_ERROR(iMessage)  { std::ostringstream stream; Logger::instance()->log(Logger::LOG_ERROR, __PRETTY_FUNCTION__, (stream << iMessage)); }
#define LOG(iMessage)  { std::ostringstream stream; Logger::instance()->log(Logger::LOG_INFO, __PRETTY_FUNCTION__, (stream << iMessage)); }
#define LOG_DEBUG(iMessage) { std::ostringstream stream; Logger::instance()->log(Logger::LOG_DEBUG, __PRETTY_FUNCTION__, (stream << iMessage)); }

#else // NO_LOG_LOW

#define LOG_ERROR(iMessage)  { std::ostringstream stream; Logger::instance()->log(Logger::LOG_ERROR, __PRETTY_FUNCTION__, (stream << iMessage)); }
#define LOG(iMessage)  { std::ostringstream stream; Logger::instance()->log(Logger::LOG_INFO, __PRETTY_FUNCTION__, (stream << iMessage)); }
#define LOG_DEBUG(iMessage)

#endif // NO_LOG_LOW

#endif // _LOG_H_
