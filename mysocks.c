/*
 * mysocks.c
 *
 *  Created on: Mar 6, 2018
 *      Author: dachee
 */
#include <event.h>
#include "types.h"

static parser_entry mysocks_entries[] = {
		{ .key = "local_ip",		.type = pt_in_addr },
		{ .key = "local_port",		.type = pt_uint16  },
		{ .key = "ip",				.type = pt_in_addr },
		{ .key = "port",			.type = pt_uint16  },
		{ .key = "type",			.type = pt_pchar   },
		{ .key = "login", 			.type = pt_pchar   },
		{ .key = "password",		.type = pt_pchar   },
		{ .key = "listenq",			.type = pt_uint16  },
		{ .key = "splice",			.type = pt_bool    },
		{ .key = "disclose_src",    .type = pt_disclose_src  },
		{ .key = "on_proxy_fail",	.type = pt_on_proxy_fail },
		{ },
};

static int mysocks_onenter(parser_section *section)
{
	return -1;
}

static int mysocks_onexit(parser_section *section)
{
	return -1;
}

static parser_section mysocks_conf_section = {
	.name = "mysocks",
	.entries = mysocks_entries,
	.onenter = mysocks_onenter,
	.onexit = mysocks_onexit,
};

static int mysocks_init(struct event_base* evbase)
{
	return -1;
}

static int mysocks_fini(void)
{
	return -1;
}

app_subsys mysocks_subsys = {
	.init = mysocks_init,
	.fini = mysocks_fini,
	.conf_section = &mysocks_conf_section,
};

