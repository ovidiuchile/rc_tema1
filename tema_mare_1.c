#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <pwd.h>
#include <grp.h>

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

	char file_access[11];
	switch(informatii.st_mode & S_IFMT)
	{
		case S_IFDIR: strcat(rezultat,"directory\n"); file_access[0]='d';break;
		case S_IFCHR: strcat(rezultat,"character-oriented device file\n"); file_access[0]='c' ; break;
		case S_IFBLK: strcat(rezultat,"block-oriented device file\n"); file_access[0]='b' ; break;
		case S_IFREG: strcat(rezultat,"regular file\n"); file_access[0]='-' ; break;
		case S_IFLNK: strcat(rezultat,"symbolic link\n"); file_access[0]='l' ; break;
		case S_IFSOCK:strcat(rezultat,"socket\n"); file_access[0]='s' ;  break;
		case S_IFIFO: strcat(rezultat,"FIFO/pipe\n"); file_access[0]='p' ; break;
		default: strcat(rezultat,"unkown file type\n"); file_access[0]='u' ; break;
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
    
	strcat(rezultat,"Access: (0");
	long long octal_access_mode=0;
	if(informatii.st_mode & S_IRUSR) {file_access[1]='r'; octal_access_mode=octal_access_mode+400;} else file_access[1]='-';
	if(informatii.st_mode & S_IWUSR) {file_access[2]='w'; octal_access_mode=octal_access_mode+200;} else file_access[2]='-';
	if(informatii.st_mode & S_IXUSR) {file_access[3]='x'; octal_access_mode=octal_access_mode+100;} else file_access[3]='-';
	if(informatii.st_mode & S_IRGRP) {file_access[4]='r'; octal_access_mode=octal_access_mode+ 40;} else file_access[4]='-';
	if(informatii.st_mode & S_IWGRP) {file_access[5]='w'; octal_access_mode=octal_access_mode+ 20;} else file_access[5]='-';
	if(informatii.st_mode & S_IXGRP) {file_access[6]='x'; octal_access_mode=octal_access_mode+ 10;} else file_access[6]='-';
	if(informatii.st_mode & S_IROTH) {file_access[7]='r'; octal_access_mode=octal_access_mode+  4;} else file_access[7]='-';
	if(informatii.st_mode & S_IWOTH) {file_access[8]='w'; octal_access_mode=octal_access_mode+  2;} else file_access[8]='-';
	if(informatii.st_mode & S_IXOTH) {file_access[9]='x'; octal_access_mode=octal_access_mode+  1;} else file_access[9]='-';
	file_access[10]='\0';
	strcat(rezultat,longlongtoarray(octal_access_mode));
	strcat(rezultat,"/");
	strcat(rezultat,file_access);
	strcat(rezultat,")    ");

	struct passwd *user;
	user=getpwuid((uid_t)informatii.st_uid);
	strcat(rezultat,"Uid: (");
	strcat(rezultat,longlongtoarray((long long)informatii.st_uid));
	strcat(rezultat,"/");
	if(user!=0)
		strcat(rezultat,user->pw_name);
	else
		strcat(rezultat,"-");
	strcat(rezultat,")    ");

	struct group *grup;
	grup=getgrgid((gid_t)informatii.st_gid);
	strcat(rezultat,"Gid: ( ");
	strcat(rezultat,longlongtoarray((long long)informatii.st_gid));
	strcat(rezultat,"/   ");
	if(grup!=0)
		strcat(rezultat,grup->gr_name);
	else
		strcat(rezultat,"-");
	strcat(rezultat,")\n");
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