#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
void myStat(char path_file[],char rezultat[])
{
	if(access(path_file,F_OK)==-1)
	{
		perror("");
		printf("Fisierul \"%s\" nu exista\n",path_file);
		exit(2);
	}
	else
	{
		printf("Succes");
	}
}
int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("%s\n","Comanda stat are nevoie de un argument. Exemplu: \"myStat file.txt\" ");
		exit(1);
	}
	char rezultat[2000]="";
	myStat(argv[1],rezultat);
	printf("%s\n",rezultat);
	return 0;
}