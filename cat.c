#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc , char *argv[])
{
	FILE* fptr;
	if (argc == 2)
	{
		if (access(argv[1] , F_OK) != 0) printf("cat: %s: no such file\n" , argv[1]);
		else
		{
			fptr = fopen(argv[1] , "r");
			if(!fptr) printf("Error");
			char ch;
			while((ch = fgetc(fptr)) != EOF)
			{
				printf("%c" , ch);
			}
			fclose(fptr);
		}
	}
	else if (argc == 3)
	{
		if (access(argv[2] , F_OK) != 0) printf("cat: %s: no such file\n" , argv[1]);
		else
		{
			fptr = fopen(argv[2] , "r");
			if(!fptr) printf("Error");
			if (!strcmp(argv[1],"-n"))
			{
				char ch;
				int ct = 1;
				printf("%d  " , ct);
				char chold = ' ';
				while((ch = fgetc(fptr)) != EOF)
				{
					if (chold == '\n') printf("%d  ",++ct);
					printf("%c" , ch);
					chold = ch;
				}
			}
			else if (!strcmp(argv[1],"-s"))
			{
				char ch;
				char chold = '\n';
				char chold2 = ' ';
				while((ch = fgetc(fptr)) != EOF)
				{
					if (ch == '\n')
					{
						if (chold == '\n' && chold2 == '\n') continue;
						else printf("\n");
					}
					else
					{
						printf("%c" , ch);
					}
					chold2 = chold;
					chold = ch;
				}
			}
			else if (argv[1][0] == '-') printf("Invalid Option.\n");
			else printf("Syntax Error : Unexpected Token\n");
			fclose(fptr);
		}	
	}
}

