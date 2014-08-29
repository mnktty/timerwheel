#ifndef _LOG_H_
#define _LOG_H_

/**
 * @file logif.h
 *
 * @brief Useful macros for some quick and dirty logging */

#include <iostream>


#define LOG_INTERNAL(os, level, str) os << #level << ": "<< str
#define LOG_FN_INTERNAL(level) std::cerr << #level << ": " << __PRETTY_FUNCTION__ << "\n"
#define LOG(level, str) LOG_INTERNAL(std::cerr, level, str)

#if defined(_DEBUG)
#define LOG_TRACE(str) std::cout << str
#define LOG_DEBUG(str) LOG(DEBUG, str)
#define LOG_FN() LOG_FN_INTERNAL(DEBUG)
#else 
#define LOG_TRACE(str)
#define LOG_DEBUG(str)
#define LOG_FN()
#endif  /* DEBUG */

#define LOG_INFO(str) LOG(INFO, str)
#define LOG_ERROR(str) LOG(ERROR, str)

#endif /* _LOG_H_ */
