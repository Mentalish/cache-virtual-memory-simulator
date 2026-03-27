#include <stdlib.h>
#include <string.h>

typedef enum {
	RR,
	RND,
} ReplacementPolicy;

typedef struct {
	int numArguments;
	int capacity;
	char **arguments;
} Arguments;

int addArgument(Arguments *argumentList, char *str) {
	if (argumentList->numArguments == argumentList->capacity) {
		argumentList->capacity = argumentList->capacity * 2;
	}
   
   argumentList->numArguments++;
   argumentList->arguments[argumentList->numArguments - 1] = str;
	return 0;
}

int clearArguments(Arguments *argumentList) { 
   int i;
   for (i = argumentList->numArguments - 1; i < 0;i--) {
      free(argumentList->arguments[i]);
   }
   return 0; 
}

int main(int argc, char *argv[]) {
	int i;
	int cacheSize;
	int blockSize;
	int associativity;
	ReplacementPolicy replacementPolicy;
	int physicalMemory;
	int physicalMemoryOS;
	int timeSlice;
	char **traceFiles;
	Arguments *currentArguments = malloc(sizeof(Arguments));
	currentArguments->numArguments = 0;
	currentArguments->capacity = 4;
	currentArguments->arguments =
		 malloc(sizeof(char *) * currentArguments->capacity);

	for (i = 0; i < argc; i++) {
		if (argv[i][0] == '-' && strlen(argv[i]) == 2) {
			while (argv[i][0] != '-') {
				addArgument(currentArguments, argv[i]);
				i++;
			}
			switch (argv[i][1]) {
			case 's':
				break;
			case 'b':
				break;
			case 'a':
				break;
			case 'r':
				break;
			case 'p':
				break;
			case 'u':
				break;
			case 'n':
				break;
			case 'f':
				break;
			default:
				break;
			}
			clearArguments(currentArguments);
		}
	}

	return 0;
}
