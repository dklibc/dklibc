#include <unistd.h>

char *optarg;
int optind = 1, opterr, optopt;

int getopt (int argc, char *argv[], const char *optstring)
{
	static int nextchar = 0;
	char c;

m1:	if (optind == argc)
		return -1;

	optarg = "";

	if (!nextchar) {
		if (argv[optind][0] != '-' || !argv[optind][1])
			return -1; // First non-option arg
		++nextchar;
	}
	
	c = argv[optind][nextchar++];

	if (!c) {
		++optind;
		nextchar = 0;
		goto m1;
	}

	// Find option in @optstring
	while (*optstring) {
		if (c == *optstring)
			break;
		++optstring;
	}

	optopt = c;

	if (!*optstring)
		return '?'; // Unknown option
	
	if (*(optstring + 1) == ':') { // Option has a value
		/*
		Option value is chars after option char or next arg
		*/
		if (!*(optarg = &argv[optind][nextchar])) {
			++optind;
			nextchar = 0;
			if (optind == argc)
				return ':'; // Missing value
		}
		optarg = &argv[optind][nextchar];
		++optind;
		nextchar = 0;
	}

	return c;
}
