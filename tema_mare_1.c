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
#include <time.h>


char* longlongtoarray(long long numar)
{
	char sir[500]="";
	int k=0,i;
	if(numar==0)
		sir[k++]='0';
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
char* time_ttoarray(time_t timp)
{
	struct tm *timp_spart=localtime(&timp);
	char sir[100]="";
	strcat(sir,longlongtoarray((long long)(timp_spart->tm_year+1900)));
	strcat(sir,"-");
	strcat(sir,longlongtoarray((long long)(timp_spart->tm_mon+1)));
	strcat(sir,"-");
	strcat(sir,longlongtoarray((long long)(timp_spart->tm_mday)));
	strcat(sir," ");
	strcat(sir,longlongtoarray((long long)(timp_spart->tm_hour)));
	strcat(sir,":");
	strcat(sir,longlongtoarray((long long)(timp_spart->tm_min)));
	strcat(sir,":");
	strcat(sir,longlongtoarray((long long)(timp_spart->tm_sec)));
	strcat(sir," +0");
	time_t abc=time(NULL);
  	struct tm t;
  	localtime_r(&abc, &t);
  	strcat(sir,longlongtoarray(t.tm_gmtoff/3600));
	strcat(sir,"00\n");

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
	strcat(rezultat,"\t\t");

	strcat(rezultat,"Blocks: ");
	strcat(rezultat,longlongtoarray((long long)informatii.st_blocks));
	strcat(rezultat,"\t   ");

	strcat(rezultat,"IO Block: ");
	strcat(rezultat,longlongtoarray((long long)informatii.st_blksize));
	strcat(rezultat,"   ");

	char file_access[11];
	switch(informatii.st_mode & S_IFMT)
	{
		case S_IFDIR: strcat(rezultat,"directory\n"); file_access[0]='d';break;
		case S_IFCHR: strcat(rezultat,"character-oriented device file\n"); file_access[0]='c' ; break;
		case S_IFBLK: strcat(rezultat,"block-oriented device file\n"); file_access[0]='b' ; break;
		case S_IFREG: strcat(rezultat,"regular file\n"); file_access[0]='-' ; break;
		case S_IFLNK: strcat(rezultat,"symbolic link\n"); file_access[0]='l' ; break;
		case S_IFSOCK:strcat(rezultat,"socket\n"); file_access[0]='s' ;  break;
		case S_IFIFO: strcat(rezultat,"fifo\n"); file_access[0]='p' ; break;
		default: strcat(rezultat,"unkown file type\n"); file_access[0]='u' ; break;
	}
	
	char sir[50]="";
	strcat(rezultat,"Device: ");
	sprintf(sir,"%x",(int)informatii.st_dev);
	strcat(rezultat,sir);
	strcat(rezultat,"h/");
	strcat(rezultat,longlongtoarray((long long)informatii.st_dev));
	strcat(rezultat,"d\t");

	strcat(rezultat,"Inode: ");
	strcat(rezultat, longlongtoarray((long long)informatii.st_ino));
	strcat(rezultat,"\t   ");	

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
	strcat(rezultat,")  ");

	struct passwd *user;
	user=getpwuid((uid_t)informatii.st_uid);
	strcat(rezultat,"Uid: (");
	strcat(rezultat,longlongtoarray((long long)informatii.st_uid));
	strcat(rezultat,"/");
	if(user!=0)
		strcat(rezultat,user->pw_name);
	else
		strcat(rezultat,"-");
	strcat(rezultat,")   ");

	struct group *grup;
	grup=getgrgid((gid_t)informatii.st_gid);
	strcat(rezultat,"Gid: (  ");
	strcat(rezultat,longlongtoarray((long long)informatii.st_gid));
	strcat(rezultat,"/   ");
	if(grup!=0)
		strcat(rezultat,grup->gr_name);
	else
		strcat(rezultat,"-");
	strcat(rezultat,")\n");

	strcat(rezultat,"Access: ");
	strcat(rezultat,time_ttoarray(informatii.st_atime));

	strcat(rezultat,"Modify: ");
	strcat(rezultat,time_ttoarray(informatii.st_mtime));

	strcat(rezultat,"Change: ");
	strcat(rezultat,time_ttoarray(informatii.st_ctime));
}
void special_trim(char sir[])
{
	while(sir[0] == ' ')
		memmove(sir,sir+1,strlen(sir));
	int k=strlen(sir);
	while(sir[k-1]==' '&&k>0)
	{
		k--;
		sir[k]='\0';
	}
	int i;
	for(i=1;i<k-1;)
	{
		if(sir[i]==' ' && sir[i+1]==' ')
		{
			memmove(sir+i,sir+i+1,strlen(sir)-i);
			k--;
		}
		else
			i++;
	}
}
int count_words(char sir[])
{
	int i,count=0,k;
	k=strlen(sir);
	for(i=0;i<k;i++)
		if(sir[i]==' ')
			count++;
	if(k!=0) count++;
	return count;
}
void manipulate(char sir[])
{
	sir[0]='k';
}
int main(int argc, char* argv[])
{
	/*
	if(argc != 2)
	{
		printf("%s\n","Comanda myStat are nevoie de un argument. Exemplu: \"myStat file.txt\" ");
		exit(1);
	}
	char rezultat[2000]="";
	myStat(argv[1],rezultat);
	printf("%s",rezultat);
	*/
	
	/////////////////////////////////

	/*char sir[2000];
	while(0 != read(0,sir,2000))
	{
		printf("bla\n");
	}*/

	//////////////////////////////////

	int pid,pipefd1[2],pipefd2[2];
	char sir[2000];
	while(fgets (sir, 2000, stdin))
	{
		special_trim(sir);
		if(-1 == pipe(pipefd1)) //tata->fiu
		{
			perror("pipe1");
			exit(2);
		}
		if(-1 == pipe(pipefd2)) //fiu->tata
		{
			perror("pipe2");
			exit(3);
		}
		switch(fork())
		{
			case -1:printf("eroare la fork"); exit(1);
			case 0:
			{
				close(pipefd1[1]);
				close(pipefd2[0]);
				read(pipefd1[0],sir,2000);
				manipulate(sir);
				write(pipefd2[1],sir,strlen(sir));
				close(pipefd1[0]);
				close(pipefd2[1]);
				exit(0);
			}
			default:
			{
				close(pipefd1[0]);
				close(pipefd2[1]);
				write(pipefd1[1],sir,strlen(sir));
				int nr=read(pipefd2[0],sir,2000);
				printf("%d: \n%s",nr-1,sir);
				close(pipefd1[1]);
				close(pipefd2[0]);
				wait(NULL);
			}
		}
		
	}





	/*
	if(-1 == write(pipefd1[1],sir,strlen(sir)))
		{
			printf("Eroare la write in pipe1: \"%s\"\n",sir);
			exit(4);
		}
		int byte_count;
		if(0 == (byte_count=read(pipefd2[0],sir,2000)))
		{
			perror("read din pipe2");
			exit(6);
		}
		printf("Numar de octeti: %d\n%s",byte_count,sir);
	if(-1 == pipe(pipefd1)) //tata->fiu
	{
		perror("pipe1");
		exit(2);
	}
	if(-1 == pipe(pipefd2)) //fiu->tata
	{
		perror("pipe2");
		exit(3);
	}
	switch(pid=fork())
	{
		case -1:
		{
			//eroare
			printf("Eroare la fork\n");
			exit(1);
		}
		case 0:
		{
			//in fiu
			close(pipefd1[1]);
			close(pipefd2[0]);
			char sir[2000];
			while(0 != read(pipefd1[0],sir,2000))
			{
				manipulate(sir);
				if(-1 == write(pipefd2[1],sir,strlen(sir)))
				{
					perror("write in pipe2");
					exit(5);
				}
			}

			close(pipefd1[0]);
			close(pipefd2[1]);
			break;
		}
		default:
		{
			//in tata
			close(pipefd1[0]);
			close(pipefd2[1]);
			char sir[2000];
			while(0 != read(0,sir,2000))
			{
				special_trim(sir);
				if(-1 == write(pipefd1[1],sir,strlen(sir)))
				{
					printf("Eroare la write in pipe1: \"%s\"\n",sir);
					exit(4);
				}
				int byte_count;
				if(0 == (byte_count=read(pipefd2[0],sir,2000)))
				{
					perror("read din pipe2");
					exit(6);
				}
				printf("Numar de octeti: %d\n%s",byte_count,sir);

			}

			close(pipefd1[1]);
			close(pipefd2[0]);
			break;
		}
	}
	*/
	return 0;
}