/*
 * log.h
 *
 *  Created on: Mar 5, 2018
 *      Author: dachee
 */

#ifndef LOG_H_
#define LOG_H_

#include <stdbool.h>
#include <stdarg.h>
#include <syslog.h>

#define log_errno(prio, msg...) _log_write(__FILE__, __LINE__, __func__, 1, prio, ## msg)
#define log_error(prio, msg...) _log_write(__FILE__, __LINE__, __func__, 0, prio, ## msg)

extern const char *error_low_memory;

int log_preopen(const char *dst, bool log_debug, bool log_info);

void log_open(void);

bool should_log(int priority);

void _log_vwrite(
		const char *file
		, int line
		, const char *func
		, int do_errno
		, int priority
		, const char *format
		, va_list ap);

void _log_write(
		const char *file
		, int line
		, const char *func
		, int do_errno
		, int priority
		, const char *format
		, ...)
#if defined(__GNUC__)
	__attribute__((format(printf, 6, 7)))
#endif
;

#endif /* LOG_H_ */
