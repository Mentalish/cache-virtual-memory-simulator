#include <stdbool.h>
#include "cmd_parser.h"

int main(int argc, char *argv[]) {
	Parameters *parameters = initParameters(3);
   bool err = parseCommandLine(argc, argv, parameters);

	if (err) {
		return 1;
	}
	return 0;
}
