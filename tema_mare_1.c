#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char* longlongtoarray(long long numar)
{
	char sir[500]="";
	int k=0,i;
	while(numar!=0)
	{
		sir[k]=numar%10+'0';
		numar/=10;
		k++;
	}
	sir[k]='\0';
	for(i=0;i<k/2;i++)
	{
		char aux=sir[i];
		sir[i]=sir[k-1-i];
		sir[k-i-1]=aux;
	}
	return sir;

}
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

	strcat(rezultat,"  File: `");
	strcat(rezultat,file_path);
	strcat(rezultat,"'\n");

	strcat(rezultat,"  Size: ");
	strcat(rezultat,longlongtoarray((long long)informatii.st_size));
	strcat(rezultat,"    ");

	strcat(rezultat,"Blocks: ");
	strcat(rezultat,longlongtoarray((long long)informatii.st_blocks));
	strcat(rezultat,"    ");

	strcat(rezultat,"IO Block: ");
	strcat(rezultat,longlongtoarray((long long)informatii.st_blksize));
	strcat(rezultat,"    ");

	switch(informatii.st_mode & S_IFMT)
	{
		case S_IFDIR: strcat(rezultat,"directory\n"); break;
		case S_IFCHR: strcat(rezultat,"character-oriented device file\n"); break;
		case S_IFBLK: strcat(rezultat,"block-oriented device file\n"); break;
		case S_IFREG: strcat(rezultat,"regular file\n"); break;
		case S_IFLNK: strcat(rezultat,"symbolic link\n"); break;
		case S_IFSOCK:strcat(rezultat,"socket\n");  break;
		case S_IFIFO: strcat(rezultat,"FIFO/pipe\n"); break;
		default: strcat(rezultat,"unkown file type\n"); break;
	}
	
	char sir[50]="";
	strcat(rezultat,"Device: ");
	sprintf(sir,"%x",(int)informatii.st_dev);
	strcat(rezultat,sir);
	strcat(rezultat,"h/");
	strcat(rezultat,longlongtoarray((long long)informatii.st_dev));
	strcat(rezultat,"d    ");

	strcat(rezultat,"Inode: ");
	strcat(rezultat, longlongtoarray((long long)informatii.st_ino));
	strcat(rezultat,"    ");	

	strcat(rezultat,"Links: ");
	strcat(rezultat,longlongtoarray((long long)informatii.st_nlink));
	strcat(rezultat,"\n");
    
    
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
	printf("%s",rezultat);
	return 0;
}