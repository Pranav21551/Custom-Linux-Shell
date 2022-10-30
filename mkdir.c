#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc , char *argv[])
{	
	if (argc == 2) 
	{
		if (access(argv[1] , F_OK) == 0) printf("mkdir: cannot create directory %s: file exists\n" , argv[1]);
		else
		{
			int c;
			c = mkdir(argv[1] , 0777);
			if(!c);
			else printf("mkdir: error: Parent Directory does not exist\n");
		}
	}
	else if (argc == 3)
	{
		if (!strcmp(argv[1] , "-v"))
		{
			if (access(argv[2] , F_OK) == 0) printf("mkdir: cannot create directory %s: file exists\n" , argv[2]);
			else
			{
				printf("mkdir: created directory '%s'" , argv[2]);
				printf("\n");
				int c;
				c = mkdir(argv[2] , 0777);
				if(!c);
				else printf("mkdir: error: Parent Directory does not exist\n");
			}
		}
		else if(!strcmp(argv[1] , "-p"))
		{
			if (access(argv[2] , F_OK) == 0);
			else
			{
				char *path = malloc(100);
				int ct = 0;
				for (int i = 0; argv[2][i] != '\0' ; i++)
				{
					if (argv[2][i] == '/')
					{
						ct++; 
						if (ct > 1) 
						{
							if (access(path , F_OK) != 0) mkdir(path , 0777);
						}
					}
					strncat(path , &argv[2][i] , 1);				
				}
				mkdir(path , 0777);
			}	
		}
		else if (argv[1][0] == '-') printf("Invalid Option\n");
		else printf("Syntax Error : Unexpected Token\n");
	}
	return 0;
}
