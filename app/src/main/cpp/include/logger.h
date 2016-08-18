/*
 * logger.h
 *
 *  Description: Logger functions declaration.
 *
 *  Created on: 22.09.2013
 *      Author: Maxim Alov <m.alov@samsung.com>
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <stdarg.h>

#define ENABLED_LOGGING 1

#ifdef ANDROID
  #include <android/log.h>
	/* Basic logging */
	// ----------------------------------------------
  #define PROMPT_SUGGEST "] >>> "
  #define COLOR_OPEN
  #define COLOR_ENCLOSING
  #define NEWLINE "\n"
  #define WHITESPACE " "
  #define COLON ":"
  #define LINE "%i"
  #define PROMPT_CLOSE COLOR_ENCLOSING NEWLINE

  #define CRT_COLOR       // Bold Red
  #define ERR_COLOR       // Red
  #define WRN_COLOR       // Yellow
  #define WRN1_COLOR      // Bold Yellow
  #define INF_COLOR       // Green
  #define INF1_COLOR      // Bold Green
  #define DBG_COLOR       // Cyan
  #define DBG1_COLOR      // Blue
  #define DBG2_COLOR      // Magenta
  #define DBG3_COLOR      // Bold Blue
  #define DBG4_COLOR      // Bold Magenta
  #define TRC_COLOR       // Grey
  #define MSG_COLOR       // White

  #define CRT_STRING "  CRT  in ["
  #define ERR_STRING "  ERR  in ["
  #define WRN_STRING "  WRN  in ["
  #define INF_STRING "  INF  in ["
  #define DBG_STRING "  DBG  in ["
  #define TRC_STRING "  TRC  in ["
  #define MSG_STRING "  MSG  in ["

  #define LOG_TAG "Arkanoid_Native"

  #define CRT_SUGGEST COLOR_OPEN CRT_COLOR CRT_STRING __FILE__ COLON LINE PROMPT_SUGGEST
  #define ERR_SUGGEST COLOR_OPEN ERR_COLOR ERR_STRING __FILE__ COLON LINE PROMPT_SUGGEST
  #define WRN_SUGGEST COLOR_OPEN WRN_COLOR WRN_STRING __FILE__ COLON LINE PROMPT_SUGGEST
  #define WRN1_SUGGEST COLOR_OPEN WRN1_COLOR WRN_STRING __FILE__ COLON LINE PROMPT_SUGGEST
  #define INF_SUGGEST COLOR_OPEN INF_COLOR INF_STRING __FILE__ COLON LINE PROMPT_SUGGEST
  #define INF1_SUGGEST COLOR_OPEN INF1_COLOR INF_STRING __FILE__ COLON LINE PROMPT_SUGGEST
  #define DBG_SUGGEST COLOR_OPEN DBG_COLOR DBG_STRING __FILE__ COLON LINE PROMPT_SUGGEST
  #define DBG1_SUGGEST COLOR_OPEN DBG1_COLOR DBG_STRING __FILE__ COLON LINE PROMPT_SUGGEST
  #define DBG2_SUGGEST COLOR_OPEN DBG2_COLOR DBG_STRING __FILE__ COLON LINE PROMPT_SUGGEST
  #define DBG3_SUGGEST COLOR_OPEN DBG3_COLOR DBG_STRING __FILE__ COLON LINE PROMPT_SUGGEST
  #define DBG4_SUGGEST COLOR_OPEN DBG4_COLOR DBG_STRING __FILE__ COLON LINE PROMPT_SUGGEST
  #define TRC_SUGGEST COLOR_OPEN TRC_COLOR TRC_STRING __FILE__ COLON LINE PROMPT_SUGGEST
  #define MSG_SUGGEST COLOR_OPEN MSG_COLOR MSG_STRING __FILE__ COLON LINE PROMPT_SUGGEST

#if ENABLED_LOGGING
  #define MSG(fmt, ...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, (MSG_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__)
  #define CRT(fmt, ...) __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, (CRT_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__)
  #define ERR(fmt, ...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, (ERR_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__)
  #define WRN(fmt, ...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, (WRN_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__)
  #define INF(fmt, ...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, (INF_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__)
  #define DBG(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, (DBG_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__)
  #define TRC(fmt, ...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, (TRC_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__)
#else
  #define MSG(fmt, ...)
  #define CRT(fmt, ...)
  #define ERR(fmt, ...)
  #define WRN(fmt, ...)
  #define INF(fmt, ...)
  #define DBG(fmt, ...)
  #define TRC(fmt, ...)
#endif

#elif ENABLED_LOGGING
/* Basic logging */
// ----------------------------------------------
  #define PROMPT_SUGGEST "] >>> "
  #define COLOR_OPEN "\e[5;"
  #define COLOR_ENCLOSING "\e[m"
  #define NEWLINE "\n"
  #define WHITESPACE " "
  #define COLON ":"
  #define LINE "%i"
  #define PROMPT_CLOSE COLOR_ENCLOSING NEWLINE

  #define CRT_COLOR "01;31m"
  #define ERR_COLOR "00;31m"
  #define WRN_COLOR "00;33m"
  #define INF_COLOR "00;32m"
  #define DBG_COLOR "00;36m"
  #define TRC_COLOR "00;37m"

  #define CRT_STRING " CRT in ["
  #define ERR_STRING " ERR in ["
  #define WRN_STRING " WRN in ["
  #define INF_STRING " INF in ["
  #define DBG_STRING " DBG in ["
  #define TRC_STRING " TRC in ["

  #define CRT_SUGGEST COLOR_OPEN CRT_COLOR CRT_STRING __FILE__ COLON LINE PROMPT_SUGGEST
  #define ERR_SUGGEST COLOR_OPEN ERR_COLOR ERR_STRING __FILE__ COLON LINE PROMPT_SUGGEST
  #define WRN_SUGGEST COLOR_OPEN WRN_COLOR WRN_STRING __FILE__ COLON LINE PROMPT_SUGGEST
  #define INF_SUGGEST COLOR_OPEN INF_COLOR INF_STRING __FILE__ COLON LINE PROMPT_SUGGEST
  #define DBG_SUGGEST COLOR_OPEN DBG_COLOR DBG_STRING __FILE__ COLON LINE PROMPT_SUGGEST
  #define TRC_SUGGEST COLOR_OPEN TRC_COLOR TRC_STRING __FILE__ COLON LINE PROMPT_SUGGEST

  #include <iostream>

  #define CRT(fmt, ...) printf((CRT_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__); std::cout << std::flush;
  #define ERR(fmt, ...) printf((ERR_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__); std::cout << std::flush;
  #define WRN(fmt, ...) printf((WRN_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__); std::cout << std::flush;
  #define INF(fmt, ...) printf((INF_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__); std::cout << std::flush;
  #define DBG(fmt, ...) printf((DBG_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__); std::cout << std::flush;
  #define TRC(fmt, ...) printf((TRC_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__); std::cout << std::flush;
  #define MSG(fmt, ...) printf((TRC_SUGGEST #fmt PROMPT_CLOSE), __LINE__, ##__VA_ARGS__); std::cout << std::flush;

#else

  #define MSG(fmt, ...)
  #define CRT(fmt, ...)
  #define ERR(fmt, ...)
  #define WRN(fmt, ...)
  #define INF(fmt, ...)
  #define DBG(fmt, ...)
  #define TRC(fmt, ...)

#endif

#endif /* LOGGER_H_ */
