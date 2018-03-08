/*
 * main.c
 *
 *  Created on: Mar 5, 2018
 *      Author: dachee
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
//#include <event.h>
#include "types.h"
#include "version.h"
#include "utils.h"

extern app_subsys debug_subsys;
extern app_subsys mysocks_subsys;

app_subsys *subsystems[] = {
	&mysocks_subsys
#ifdef DBG_BUILD
	, &debug_subsys
#endif
};

static const char *confname = "mysocks.conf";
static const char *pidfile = NULL;

const char *mysocks_version = "0.0.1";


int main(int argc, char **argv)
{
	int error;
    int opt;
    int exit_signals[2] = { SIGTERM, SIGINT };
    bool conftest = false;
    app_subsys **ss;

    while ((opt = getopt(argc, argv, "h?vtc:p:")) != -1)
    {
    	switch (opt)
    	{
    	case 'v':
    		puts(mysocks_version);
    		return EXIT_SUCCESS;

    	case 't':
    		conftest = true;
    		break;

    	case 'c':
    		confname = optarg;
    		break;

    	case 'p':
    		pidfile = optarg;
    		break;

    	default:
    		printf(
    			"Usage: %s [-?hvt] [-c config] [-p pidfile]\n"
    			"	-h, -?			Print help message;\n"
    			"	-v				Print version;\n"
    			"	-t				Test the config file;\n"
    			"	-p				Write pid to pidfile;\n",
				argv[0]);
    		return (opt == 'h' || opt == '?') ? EXIT_SUCCESS : EXIT_FAILURE;
    	}
    }

    FILE *f = fopen(confname, "r");
    if (!f)
    {
    	perror("Failed to open configuration file.");
    	return EXIT_FAILURE;
    }


    parser_context *context = parser_start(f);
    if (!context)
    {
    	perror("Not enough memory.");
    	return EXIT_SUCCESS;
    }

    FOREACH(ss, subsystems)
    	if ((*ss)->conf_section)
    		parser_add_section(context, (*ss)->conf_section);

    error = parser_run(context);
    parser_stop(context);
    fclose(f);

    if (error)
    {
    	return EXIT_FAILURE;
    }

    if (conftest)
    {
    	return EXIT_SUCCESS;
    }


    return EXIT_SUCCESS;
}

