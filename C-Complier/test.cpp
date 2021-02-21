#include "parser.h"
int main()
{
	FILE* fp = NULL;

	fp = fopen("D:\\test.txt", "r");
	parse(fp);
	return 0;
}
