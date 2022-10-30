#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc , char *argv[])
{
	char buf[256] = {0};
	if (argc == 1)
	{	
		time_t T = time(NULL);
		struct tm *tm = localtime(&T);
		strftime(buf , 256 , "%a %b %d %H:%M:%S IST %Y" , tm);
		puts(buf);
	}
	else
	{
		if(!strcmp(argv[1] , "-u"))
		{
			time_t T = time(NULL);
			struct tm *tm = gmtime(&T);
			strftime(buf , 256 , "%a %b %d %H:%M:%S UTC %Y" , tm);
			puts(buf);			
		}
		else if(!strcmp(argv[1] , "-r"))
		{	
			if (argc == 2) printf("Missing Operand : File Path\n");
			else if (access(argv[2] , F_OK) != 0) printf("date: %s: no such file\n" , argv[2]);
			else
			{
				char *file = argv[2];	
				struct stat b;
				if(!stat(file , &b)) strftime(buf , 256 , "%a %b %d %H:%M:%S IST %Y" , localtime(&b.st_mtime));
				puts(buf);
			}
		}
		else if(argv[1][0] == '-') printf("Invalid Option\n");
		else printf("Syntax Error : Unexpected Token\n");
	}
	return 0;
}
