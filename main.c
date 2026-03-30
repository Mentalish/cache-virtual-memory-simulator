#include <stdbool.h>
#include "cmd_parser.h"
#include "memory_calculations.h"
#include "cache_calculations.h"
#include "print.h"

int main(int argc, char *argv[]) {
	Parameters *parameters = initParameters(3);
	bool err = parseCommandLine(argc, argv, parameters);

	if (err) {
		freeParameters(parameters);
		return 1;
	}
   
   //call functions
   
	freeParameters(parameters);

	return 0;
}
