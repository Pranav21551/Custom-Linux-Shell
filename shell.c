#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <pthread.h>

void ThreadFunc(char *command)
{
	pthread_t thread_id;
	pthread_create(&thread_id , NULL , (void *)system , command);
	pthread_join(thread_id , NULL);
}

void ThreadFunc2(char *command , char *args[] , int n)
{
	pthread_t thread_id;
	char *truecomm = malloc(100);
	strcpy(truecomm , command);
	for (int i = 1 ; i < n ; i++)
	{
		strcat(truecomm , " ");
		strcat(truecomm , args[i]);
	}
	pthread_create(&thread_id , NULL , (void *)system , truecomm);
	pthread_join(thread_id , NULL);
}

void echo(int argc , char* argv[])
{
	int i;
	if (argc == 1) printf("\n");
	else if (!strcmp(argv[1] , "--help"))
	{
		printf("NAME\n");
		printf("\t\techo\tdisplay a line of text\n");
		printf("SYNOPSIS\n");
		printf("\t\techo [option] [string]\n");
		printf("OPTIONS\n");
		printf("\t\t-n\tdo not output the trailing newline\n");
		printf("\t\t-E\tdisable interpretation of backslash escapes (Default)\n");
		printf("\t\t--help\tdisplay this help and exit\n");	
	}
	else
	{
		if (argv[1][0] == '-')
		{
			if (argv[1][1] != 'n' && argv[1][1] != 'E')
			{
				printf("Invalid Option.\n");
			}
			else if (argv[1][1] == 'n')
			{
				for (int i = 2 ; i < argc ; i++)
				{
					printf("%s " , argv[i]);
				}
			}
			else if (argv[1][1] == 'E')
			{
				for(int i = 2 ; i < argc ; i++)
				{
					printf("%s " , argv[i]);
				}
				printf("\n");
			}
		}
		else
		{
			for(int i = 1 ; i < argc ; i++)
			{
				printf("%s " , argv[i]);
			}
			printf("\n");
		}
	}
}

void cd(int argc , char *argv[])
{
	if(argc == 1)
	{
		char *pth = "~";
		pth++;
		char *cur_path = getenv("HOME");
		strcat(cur_path , pth);
		chdir(cur_path);
	}
	else if(argc == 2)
	{	
		if (!strcmp(argv[1],"~"))
		{
			argv[1]++;		
			char *cur_path = getenv("HOME");
			strcat(cur_path , argv[1]);
			chdir(cur_path);	
		}
		else 
		{
			if (chdir(argv[1]) != 0) printf("cd: %s: No such file or directory.\n" , argv[1]);	
		}
	}
	else 
	{
		printf("Too many arguments.\n");
	}
}

void pwd(int argc , char *argv[])
{
	char path[1000];
	if (argc == 1)
	{	
		printf("%s\n" , getenv("PWD"));
	}
	else if (argc == 2)
	{
		if(!strcmp(argv[1] , "-L"))
		{
			printf("%s\n" , getenv("PWD"));
		}
		else if(!strcmp(argv[1] , "-P"))
		{
			getcwd(path , 1000);
			printf("%s\n" , path);
		}
		
		else if (argv[1][0] == '-') printf("Invalid Option.\n");
		else printf("Syntax Error : Unexpected Token\n"); 
	}
	else printf("Too many arguments\n");
}

int main()
{
	while(true)
	{
		printf("> ");
		char cwd[1024];
		getcwd(cwd , 1024);
		printf("Dir: %s " , cwd);
		char *line = malloc(sizeof(char) * 1024);
		int c;
		int pos = 0;
		while(1)
		{
			c = getchar();
			if (c == EOF || c == '\n')
			{
				line[pos] = '\0';
				break;
			}
			else
			{
				line[pos] = c;
			}
			pos++;		
		}	
		int size = 64;
		pos = 0;
		char **toks = malloc(size * sizeof(char *));
		char *tok;
		tok = strtok(line , " \t\r\n");
		while(tok != NULL)
		{
			toks[pos] = tok;
			pos++;
			tok = strtok(NULL , " \t\r\n");
		}
		toks[pos] = NULL;
		// Internal Commands		
		if (!strcmp(toks[0] , "echo") || !strcmp(toks[0] , "echo&t"))
		{
			echo(pos , toks);
		}
		else if (!strcmp(toks[0] , "cd") || !strcmp(toks[0] , "cd&t"))
		{
			cd(pos , toks);
		}
		else if (!strcmp(toks[0] , "pwd") || !strcmp(toks[0] , "pwd&t"))
		{
			pwd(pos , toks);
		}
		// External Commands
		else if (!strcmp(toks[0] , "ls"))
		{
			pid_t p;
			p = fork();
			if (p == -1) printf("There is an error while calling.");
			if (p == 0)
			{
				if (pos == 1) execl("/home/Ass1/ls" , toks[0] , cwd, NULL);
				else if (pos == 2) execl("/home/Ass1/ls" , toks[0] , toks[1] , cwd,  NULL);
				else printf("Too many arguments.\n");				
			}
			else
			{
				int status;
				wait(&status);
			}
		}
		else if (!strcmp(toks[0] , "ls&t"))
		{
			char *command = malloc(100);
			if (pos == 1) 
			{	
				strcat(command , "/home/Ass1/ls");
				strcat(command , " ");
				strcat(command , cwd);
				ThreadFunc(command);			
			}
			else if (pos == 2) 
			{
				strcat(command , "/home/Ass1/ls");
				strcat(command , " ");
				strcat(command , toks[1]);
				strcat(command , " ");
				strcat(command , cwd);
				ThreadFunc(command);
			}
			else printf("Too many arguments.\n");		
		}
		else if (!strcmp(toks[0] , "cat"))
		{
			pid_t p;
			p = fork();
			if (p == -1) printf("There is an error while calling.");
			if (p == 0)
			{
				if (pos == 1) printf("Missing Operand: File path\n");
				else if (pos == 2) execl("/home/Ass1/cat" , toks[0] , toks[1], NULL);
				else if (pos == 3) execl("/home/Ass1/cat" , toks[0] , toks[1] , toks[2], NULL);
				else printf("Too many arguments.\n");
			}
			else
			{
				int status;
				wait(&status);
			}		
		}
		else if (!strcmp(toks[0] , "cat&t"))
		{
			char *command = malloc(100);
			strcpy(command , "/home/Ass1/cat");
			if (pos == 1) printf("Missing Operand: File path\n");
			else if (pos > 3) printf("Too many arguments.\n");
			else ThreadFunc2(command , toks , pos);		
		}
		else if (!strcmp(toks[0] , "date"))
		{
			pid_t p;
			p = fork();
			if (p == -1) printf("There is an error while calling.");
			if (p == 0)
			{
				if (pos == 1) execl("/home/Ass1/date" , toks[0] , NULL);
				else if (pos == 2) execl("/home/Ass1/date" , toks[0] , toks[1] , NULL);
				else if (pos == 3) execl("/home/Ass1/date" , toks[0] , toks[1] , toks[2], NULL);
				else printf("Too many arguments.\n");				
			}
			else
			{
				int status;
				wait(&status);
			}
		}
		else if (!strcmp(toks[0] , "date&t"))
		{
			char *command = malloc(100);
			strcpy(command , "/home/Ass1/date");
			if (pos > 3) printf("Too many arguments.\n");
			else ThreadFunc2(command , toks , pos);		
		}
		else if (!strcmp(toks[0] , "rm"))
		{
			
			pid_t p;
			p = fork();
			if (p == -1) printf("There is an error while calling.");
			if (p == 0)
			{
				if (pos == 1) printf("Missing Operand: File path\n");
				else if (pos == 2) execl("/home/Ass1/rm" , toks[0] , toks[1] , NULL);
				else if (pos == 3) execl("/home/Ass1/rm" , toks[0] , toks[1] , toks[2], NULL);
				else printf("Too many arguments.\n");				
			}
			else
			{
				int status;
				wait(&status);
			}
		}
		else if (!strcmp(toks[0] , "rm&t"))
		{
			char *command = malloc(100);
			strcpy(command , "/home/Ass1/rm");
			if (pos == 1) printf("Missing Operand: File path\n");
			else if (pos > 3) printf("Too many arguments.\n");
			else ThreadFunc2(command , toks , pos);		
		}
		else if (!strcmp(toks[0] , "mkdir"))
		{
			pid_t p;
			p = fork();
			if (p == -1) printf("There is an error while calling.");
			if (p == 0)
			{
				if (pos == 1) printf("Missing Operand: File Path\n");
				else if (pos == 2) execl("/home/Ass1/mkdir" , toks[0] , toks[1] , NULL);
				else if (pos == 3) execl("/home/Ass1/mkdir" , toks[0] , toks[1] , toks[2], NULL);
				else printf("Too many arguments.\n");				
			}
			else
			{
				int status;
				wait(&status);
			}
		}
		else if (!strcmp(toks[0] , "mkdir&t"))
		{
			char *command = malloc(100);
			strcpy(command , "/home/Ass1/mkdir");
			if (pos == 1) printf("Missing Operand: File path\n");
			else if (pos > 3) printf("Too many arguments.\n");
			else ThreadFunc2(command , toks , pos);		
		}
		else printf("No such command\n");
		free(line);
		free(toks);
	}
	return 0;
}
