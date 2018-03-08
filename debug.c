/*
 * debug.c
 *
 *  Created on: Mar 6, 2018
 *      Author: dachee
 */
#include "types.h"
#include "parser.h"

static parser_entry debug_entries[] = {
	{ .key = "http_ip", .type = pt_pchar },
	{ .key = "http_port", .type = pt_uint16 },
	{ }
};

static int debug_onenter(parser_section *section)
{
	return -1;
}

static int debug_onexit(parser_section *section)
{
	return -1;
}

static parser_section debug_conf_section =
{
	.name = "debug",
	.entries = debug_entries,
	.onenter = debug_onenter,
	.onexit = debug_onexit,
};

static int debug_init()
{
	return -1;
}

static int debug_fini(void)
{
	return -1;
}

app_subsys debug_subsys =
{
	.init = debug_init,
	.fini = debug_fini,
	.conf_section = &debug_conf_section,
};

