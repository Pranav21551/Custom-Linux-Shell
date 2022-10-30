#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc , char *argv[])
{
	if (argc == 2)
	{
		if (access(argv[1] , F_OK) != 0) printf("rm: %s: no such file\n", argv[1]);
		else remove(argv[1]);
	}	
	else if(argc == 3)
	{
		if (access(argv[2] , F_OK) != 0) printf("rm: %s: no such file\n", argv[1]);
		else if(!strcmp(argv[1] , "-v"))
		{
			remove(argv[2]);
			printf("removed '%s'\n", argv[2]);
		}
		else if(!strcmp(argv[1] , "-i"))
		{
			char ch;
			printf("rm: remove regular empty file '%s'? (y/n) " , argv[2]);
			scanf(" %c",&ch);
			if (ch == 'y') remove(argv[2]);			
		}
		else if( argv[1][0] == '-') printf("Invalid Option\n");
		else printf("Syntax Error : Unexpected Token\n");
	}
	return 0;
}
