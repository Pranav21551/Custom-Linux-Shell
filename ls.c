#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>

int compare(const void *a , const void *b)
{
	const char **str_a = (const char **)a;
	const char **str_b = (const char **)b;
	return strcmp(*str_a , *str_b);
}

void ls(const char *dir , int flag1 , int flag2)
{
	char *files[1000];
	struct dirent *d;
	DIR *dr = opendir(dir);
	if (!dr)
	{
		if (errno = ENOENT) perror("Directory does not exist.");
		else perror("Unable to read directory.");
		exit(EXIT_FAILURE);
	}	
	int pos = 0;
	while((d = readdir(dr)) != NULL)
	{	
		if(d->d_name[0] == '.' && flag1 == 0) continue;
		files[pos] = (char *) malloc (strlen(d->d_name)+1);
		strcpy(files[pos] , d->d_name);
		pos++;
	}
	qsort(files , pos , sizeof(char *) , compare);
	if (flag2 == 0)
	{
		for (int i = 0 ; i < pos ; i++) printf("%s  " , files[i]); 
		printf("\n");
	}
	else
	{
		for (int i = pos-1 ; i > -1 ; i--) printf("%s  " , files[i]); 
		printf("\n");
	}
}

int main(int argc , char *argv[])
{
	if (argc == 2)
	{
		ls(argv[1] , 0 , 0);
	}
	else if (argc == 3)
	{
		if(!strcmp(argv[1] , "-a"))
		{
			ls(argv[2] , 1 , 0);
		}
		else if(!strcmp(argv[1] , "-r"))
		{
			ls(argv[2] , 0 , 1);
		}
		else if(argv[1][0] == '-') printf("Invalid Option.\n");
		else printf("Syntax Error : Unexpected Token.\n");
	}
	else
	{
		printf("Too many arguments.\n");
	}
	return 0;
}

