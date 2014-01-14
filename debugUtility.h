/*
 * The following license idents are currently accepted as indicating free
 * software 
 *
 *  "GPL"               [GNU Public License v2 or later]
 *
 *
 * @Author	Archer Chang
 * @file
 * date
 *
 */


#ifndef _MY_DEBUG_UTILITY_H_
#define _MY_DEBUG_UTILITY_H_

#include <stdio.h>

#define NONE            "\033[m"
#define RED             "\033[0;32;31m"
#define LIGHT_RED       "\033[1;31m"
#define GREEN           "\033[0;32;32m"
#define LIGHT_GREEN     "\033[1;32m"
#define BLUE            "\033[0;32;34m"
#define LIGHT_BLUE      "\033[1;34m"
#define DARY_GRAY       "\033[1;30m"
#define CYAN            "\033[0;36m"
#define LIGHT_CYAN      "\033[1;36m"
#define PURPLE          "\033[0;35m"
#define LIGHT_PURPLE    "\033[1;35m"
#define BROWN           "\033[0;33m"
#define YELLOW          "\033[1;33m"
#define LIGHT_GRAY      "\033[0;37m"
#define WHITE           "\033[1;37m"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG_OFF
#define DEBUG_PRINT(format, ...)                                        
#else
#define DEBUG_PRINT(format, ...)                                        \
do {                                                                    \
    fprintf(stderr, "[File:%s][Line:%d][Func:%s]: "format,              \
            __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__);    \
} while(0)
#endif

#ifdef DEBUG_OFF
#define DEBUG_PRINT_COLOR(color, format, ...)
#else
#define DEBUG_PRINT_COLOR(color, format, ...)                           \
do {                                                                    \
    fprintf(stderr, "%s", color);                                       \
    fprintf(stderr, "[File:%s][Line:%d][Func:%s]: "format,              \
            __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__);    \
    fprintf(stderr, "%s", NONE);                                        \
} while(0)
#endif

#ifdef DEBUG_OFF
#define DEBUG_WIRTE_LOG(file, format, ...)                              
#else
#define DEBUG_WIRTE_LOG(file, format, ...)                              \
do {                                                                    \
    char log[1024] = {0};                                               \
    sprintf(log, format, ##__VA_ARGS__);                                \
    debug_write_log(file, log);                                         \
} while(0)
#endif

void memory_dump(const char *buf, int len);
int debug_write_log(char *file, char *log);

#ifdef __cplusplus
}
#endif
#endif

