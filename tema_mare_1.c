#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
void myStat(char file_path[],char rezultat[])
{
	if(access(file_path,F_OK)==-1)
	{
		perror("");
		printf("Fisierul \"%s\" nu exista\n",file_path);
		exit(2);
	}
	struct stat informatii;
	if(stat(file_path,&informatii)==-1)
	{
		perror("");
		printf("Eroare la utilizarea myStat \"%s\"\n",file_path);
		exit(3);
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