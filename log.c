/*
 * log.c
 *
 *  Created on: Mar 5, 2018
 *      Author: dachee
 */

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <event.h>
#include "log.h"
#include "utils.h"

typedef void (*log_func)(
	const char *file
	, int line
	, const char *func
	, int priority
	, const char *message
	, const char *appendix
);

const char *error_low_memory = "[Can't print error, not enough memory]";

static const char* get_priority_name(int priority)
{
	switch (priority)
	{
	case LOG_EMERG:
		return "emerg";

	case LOG_ALERT:
		return "alert";

	case LOG_CRIT:
		return "crit";

	case LOG_ERR:
		return "err";

	case LOG_WARNING:
		return "warning";

	case LOG_NOTICE:
		return "notice";

	case LOG_INFO:
		return "info";

	case LOG_DEBUG:
		return "debug";

	default:
		return "?";
	}
}

static void fprint_timestamp(FILE *fd, const char * file, int line, const char *func, int priority, const char *message, const char *appendix)
{
	struct timeval tv = {};
	gettimeofday(&tv, 0);

	const char *sprio = get_priority_name(priority);
	if (appendix)
	{
		fprintf(fd, "%lu.%6.6lu %s %s:%u %s(...) %s: %s\n", tv.tv_sec, tv.tv_usec, sprio, file, line, func, message, appendix);
	}
	else
	{
		fprintf(fd, "%lu.%6.6lu %s %s:%u %s(...) %s\n", tv.tv_sec, tv.tv_usec, sprio, file, line, func, message);
	}
}

static void stderr_msg(const char *file, int line, const char *func, int priority, const char *message, const char *appendix)
{
	fprint_timestamp(stderr, file, line, func, priority, message, appendix);
}

static FILE *logfile = NULL;

static void logfile_msg(const char *file, int line, const char *func, int priority, const char *message, const char * appendix)
{
	fprint_timestamp(logfile, file, line, func, priority, message, appendix);
	fflush(logfile);
}

static void syslog_msg(const char *file, int line, const char *func, int priority, const char *message, const char * appendix)
{
	if (appendix)
	{
		syslog(priority, "%s: %s\n", message, appendix);
	}
	else
	{
		syslog(priority, "%s\n", message);
	}
}

static log_func log_msg = stderr_msg;
static log_func log_msg_next = NULL;
static bool should_log_info = true;
static bool should_log_debug = false;

int log_preopen(const char *dst, bool log_debug, bool log_info)
{
	const char *syslog_prefix = "syslog:";
	const char *file_prefix = "file:";
	should_log_debug = log_debug;
	should_log_info = log_info;

	if (strcmp(dst, "stderr") == 0)
	{
		log_msg_next = stderr_msg;
	}
	else if (strncmp(dst, syslog_prefix, strlen(syslog_prefix)) == 0)
	{
		const char *facility_name = dst + strlen(syslog_prefix);
		int facility = -1;
		int logmask;

		struct {
			char *name;
			int value;
		} *ptpl, tpl[] = {
				{"daemon",LOG_DAEMON},
				{"local0",LOG_LOCAL0},
				{"local1",LOG_LOCAL1},
				{"local2",LOG_LOCAL2},
				{"local3",LOG_LOCAL3},
				{"local4",LOG_LOCAL4},
				{"local5",LOG_LOCAL5},
				{"local6",LOG_LOCAL6},
				{"local7",LOG_LOCAL7},
		};

		FOREACH(ptpl, tpl)
			if (strcmp(facility_name, ptpl->name) == 0)
			{
				facility = ptpl->value;
				break;
			}

		if (facility == -1)
		{
			log_error(LOG_ERR, "log_preopen(%s, ...): unknown syslog facility", dst);
			return -1;
		}

		openlog("mysocks", LOG_NDELAY | LOG_PID, facility);

		logmask = setlogmask(0);
		if (!log_debug)
		{
			logmask &= ~(LOG_MASK(LOG_DEBUG));
		}
		if (!log_info)
		{
			logmask &= ~(LOG_MASK(LOG_INFO));
		}
		setlogmask(logmask);

		log_msg_next = syslog_msg;
	}
	else if (strncmp(dst, file_prefix, strlen(file_prefix)) == 0)
	{
		const char *filename = dst + strlen(file_prefix);
		if ((logfile = fopen(filename, "a")) == NULL)
		{
			log_error(LOG_ERR, "log_preopen(%s, ...): %s", dst, strerror(errno));
			return -1;
		}
		log_msg_next = logfile_msg;
	}
	else
	{
		log_error(LOG_ERR, "log_preopen(%s, ...): unknown destination", dst);
		return -1;
	}

	return 0;
}

void log_open(void)
{
	log_msg = log_msg_next;
	log_msg_next = NULL;
}

bool should_log(int priority)
{
	return (priority != LOG_DEBUG && priority != LOG_INFO)
			|| (priority == LOG_DEBUG && should_log_debug)
			|| (priority == LOG_INFO && should_log_info);
}

void _log_vwrite(const char *file, int line, const char *func, int do_errno, int priority, const char *format, va_list ap)
{
	int saved_errno = errno;
	struct evbuffer *evbuf = evbuffer_new();
	const char *message;

	if (evbuf)
	{
		evbuffer_add_vprintf(evbuf, format, ap);
		message = (const char *)evbuffer_pullup(evbuf, -1);
	}
	else
	{
		message = error_low_memory;
	}

	log_msg(file, line, func, priority, message, do_errno ? strerror(saved_errno) : NULL);

	if (evbuf)
	{
		evbuffer_free(evbuf);
	}
}

void _log_write(const char *file, int line, const char *func, int do_errno, int priority, const char *format, ...)
{
	if (!should_log(priority))
		return;

	va_list ap;

	va_start(ap, format);
	_log_vwrite(file, line, func, do_errno, priority, format, ap);
	va_end(ap);
}
