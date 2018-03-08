/*
 * parser.c
 *
 *  Created on: Mar 7, 2018
 *      Author: dachee
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "stddef.h"
#include "parser.h"

//***************************************************************
// ???
typedef int (*value_parser)(parser_context *context, void *addr, const char *token);

//***************************************************************
// A parser_context instance represents a configuration file.
//
// Defining the data structure in the source file instead of the header
// for protect the data field accessing from outside.
struct parser_context_t
{
	FILE			*fd;
	parser_section	*sections;
	int 			line;
	int				error;
	struct
	{
		size_t	size;
		size_t	filled;
		char	*data;
	} buffer;
};

//***************************************************************
// ???
void parser_error(parser_context *context, const char *fmt, ...)
{

}

//***************************************************************
// Create an instance of parser_context for representing a configuration
// file. Initialize the instance.
parser_context* parser_start(FILE *fd)
{
	parser_context *ret = calloc(1, sizeof(parser_context));
	if (!ret)
	{
		return NULL;
	}

	// Initialize the instance.
	ret->fd = fd;
	ret->buffer.size = 128;
	ret->buffer.data = malloc(ret->buffer.size);
	if (!ret->buffer.data)
	{
		free(ret);
		return NULL;
	}

	return ret;
}

//***************************************************************
// Add a supported section into a parser_context instance;
// NOTE:
//		Just add the section instance into the section linked list data
// 		structure.
void parser_add_section(parser_context *context, parser_section *section)
{
	section->next = context->sections;
	context->sections = section;
	section->context = context;
}

//***************************************************************
// Destruct the context instance, and free the memory back to OS.
//
// QUESTION:
//	1. Is there a memory leak? (The context->sections) have not been free???
//
// ANSWER:
//	1. The sections is decleared in each app_subsys instances,
//		and they cannot(no need to) be released here.
void parser_stop(parser_context *context)
{
	free(context->buffer.data);
	free(context);
}

//***************************************************************
// ???
int parser_run(parser_context *context)
{
	char *section_token = NULL;
	char *key_token = NULL;
	char *value_token = NULL;
	parser_section *section = NULL;
	bool need_more_space = false;	// ???
	bool need_more_data = false;	// ???

	while (!(context->error) && !feof(context->fd))
	{
		assert(context->buffer.filled < context->buffer.size);

		if (need_more_space)
		{
			char *new_buf = realloc(context->buffer.data, context->buffer.size * 2);
			if (!new_buf)
			{
				parser_error(context, "realloc failure");
				return -1;
			}
			context->buffer.data = new_buf;
			context->buffer.size *= 2;
			need_more_space = false;
		}

		if (need_more_data)
		{

		}
	}

	return -1;
}

//***************************************************************
// ???
static char unescape(int c)
{
	return 0;
}

//***************************************************************
// ???
static char *gettoken(parser_context *context, char **iter)
{
	return NULL;
}

//***************************************************************
// ???
static void context_filled_add(parser_context *context, int shift)
{

}

//***************************************************************
// ???
static void context_filled_set(parser_context *context, size_t val)
{

}

static int vp_pbool(parser_context *context, void *addr, const char *token)
{
	return -1;
}








