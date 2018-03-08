/*
 * types.h
 *
 *  Created on: Mar 6, 2018
 *      Author: dachee
 */

#ifndef TYPES_H_
#define TYPES_H_

#include "parser.h"

struct event_base;
//struct parser_section;

typedef struct app_subsys_t {
	int (*init)(struct event_base*);
	int (*fini)(void);
	parser_section* conf_section;
} app_subsys;


#endif /* TYPES_H_ */
