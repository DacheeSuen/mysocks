/*
 * parser.h
 *
 *  Created on: Mar 7, 2018
 *      Author: dachee
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>

//***************************************************************
// ???
enum disclose_src_e {
	DISCLOSE_NONE,
	DISCLOSE_X_FORWARDED_FOR,
	DISCLOSE_FORWARDED_IP,
	DISCLOSE_FORWARDED_IPPORT,
};

//***************************************************************
// ???
enum on_proxy_fail_e {
	ONFAIL_CLOSE,
	ONFAIL_FORWARD_HTTP_ERR,
};

//***************************************************************
// Indicating the value type of a configuration item.
typedef enum {
	pt_bool,		// "bool" from stdbool.h, not "_Bool" or anything else
	pt_pchar,
	pt_uint16,
	pt_uint32,
	pt_in_addr,
	pt_in_addr2,	// inaddr[0] = net, inaddr[1] = netmask
	pt_disclose_src,
	pt_on_proxy_fail,				// ???
	pt_obsolete,					// ???
	pt_mysocks_max_accept_backoff,	// ???
} parser_type;

//***************************************************************
//	A configuration item that a specific configuration segment support.
typedef struct parser_entry_t {
	const char		*key;	// key name of the configuration item
	parser_type		type;	// value data type of the configuration item
	void			*addr;	// ???
} parser_entry;

//***************************************************************
// A parser_context instance represents a configuration file.
typedef struct parser_context_t parser_context;

//***************************************************************
// type define of parser_section_t
typedef struct parser_section_t parser_section;

//***************************************************************
// Function pointer:
// ??? Handle a configuration section???
typedef int (*parser_section_onenter)(parser_section *section);

//***************************************************************
// Function pointer:
// ??? Finish a handling of a configuration section???
typedef int (*parser_section_onexit)(parser_section *section);

//***************************************************************
// A parser_section_t instance represents a segment in the
// configuration file.
struct parser_section_t {
	parser_section			*next;
	parser_context			*context;
	const char				*name;
	parser_section_onenter	onenter;
	parser_section_onexit	onexit;
	parser_entry			*entries;
	void					*data;
};

//***************************************************************
// ???To start handling a configuration file.
parser_context* parser_start(FILE *fd);

//***************************************************************
// ???Adding a new configuration segment???
void parser_add_section(parser_context *context, parser_section *section);

//***************************************************************
// ???
// Interface for outside, to start parsing a configuration file???
int parser_run(parser_context *context);

//***************************************************************
// ??? Handler of parser???
void parser_error(parser_context *context, const char *fmt, ...)
#if defined(__GNUC__)
	__attribute__((format(printf, 2, 3)))
#endif
;

//***************************************************************
// ???
// Stop handling a configuration file???
void parser_stop(parser_context *context);


#endif /* PARSER_H_ */
