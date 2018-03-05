/*
 * main.c
 *
 *  Created on: Mar 5, 2018
 *      Author: dachee
 */
#include <stdlib.h>
#include <unistd.h>
#include "version.h"
#include "utils.h"

static const char *confname = "mysocks.conf";
static const char *pidfile = NULL;

const char *mysocks_version = "0.0.1";

int main(int argc, char **argv)
{
    int opt;
    bool conftest = false;

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

    return EXIT_SUCCESS;
}

