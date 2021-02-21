#include "exception.h"
#include <stdlib.h>
const char* exce_tag[60] =
{
	"invalid data position",		"invalid expression",		"invalid for expression",
	"invalid declaration",			"invalid next token",		"invalid stack size",
	"invalid container pointer",	"invalid requiered data",	"invalid operation",
	"invalid const char/char*",		"invalid number",			"invalid token",
	"invalid container size"
};
void catch_exce(int _tag)
{
	printf("%s\n", exce_tag[_tag]);
	printf("complie failed\n");
	exit(0);
}