#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <time.h>

#ifdef _DEBUG
#define debug_printf(fmt, ...) do { \
		struct timespec ts[1];					\
		struct tm t[1];							\
		clock_gettime(CLOCK_REALTIME, ts);		\
		localtime_r(&ts->tv_sec, t);			\
		char timestamp[100 + 100] = "";				\
		strftime(timestamp, 100, "%Y-%m-%d %H:%M:%S", t);	\
		snprintf(timestamp+strlen(timestamp), 100, ".%.9ld", (long)ts->tv_nsec);	\
		fprintf(stderr, "\e[33m" "%s [DEBUG]::%s(%d)::%s()::" fmt "\e[39m" "\n",		\
			timestamp,							\
			__FILE__, __LINE__, __FUNCTION__,	\
			##__VA_ARGS__);						\
	} while(0)

#else
#define debug_printf(fmt, ...) do { } while(0)
#endif





#ifdef __cplusplus
}
#endif
#endif
