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
#include <dirent.h>
#include <sys/socket.h>
#include <ncurses.h>
#define MAX_CHAR_SIZE 10000
char* longlongtoarray(long long numar)
{
	char sir[500]="";
	int k=0,i;
	if(numar==0)
		sir[k++]='0';
	while(numar!=0) 			//introducerea cifrelor in sirul dorit(in ordine inversa)
	{							//
		sir[k]=numar%10+'0';	//
		numar/=10;				//
		k++;					//
	}							///
	sir[k]='\0';				//inchiderea sirului
	for(i=0;i<k/2;i++) 			//oglindirea sirului
	{							//
		char aux=sir[i];		//
		sir[i]=sir[k-1-i];		//
		sir[k-i-1]=aux;			//
	}							///
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
	strcat(sir," +0");								//introducerea fusului orar
	time_t abc=time(NULL);							//
  	struct tm t;									//
  	localtime_r(&abc, &t);							//
  	strcat(sir,longlongtoarray(t.tm_gmtoff/3600));	//
	strcat(sir,"00");								//

}
void myStat(char file_path[],char rezultat[])
{

	if(access(file_path,F_OK)==-1)	//daca fisierul exista
	{
		sprintf(rezultat,"Fisierul \"%s\" nu exista",file_path);
		return;
	}

	struct stat informatii;
	if(stat(file_path,&informatii)==-1)	//daca functioneaza functia stat din C
	{
		sprintf(rezultat,"Eroare la utilizarea stat \"%s\"",file_path);
		return;
	}

	strcpy(rezultat,"");
	strcat(rezultat,"  File: `"); //numele fisierului
	strcat(rezultat,file_path);
	strcat(rezultat,"'\n");

	strcat(rezultat,"  Size: "); //dimensiunea fisierului
	strcat(rezultat,longlongtoarray((long long)informatii.st_size));
	strcat(rezultat,"\t\t");

	strcat(rezultat,"Blocks: "); //numarul de blocuri
	strcat(rezultat,longlongtoarray((long long)informatii.st_blocks));
	strcat(rezultat,"\t   ");

	strcat(rezultat,"IO Block: ");	//dimensiunea preferata a blocului
	strcat(rezultat,longlongtoarray((long long)informatii.st_blksize));
	strcat(rezultat,"   ");

	char file_access[11];					//tipul fisierului
	switch(informatii.st_mode & S_IFMT)		////file_access[0] va fi folosit la informatiile despre accesul la fisier
	{
		case S_IFDIR: strcat(rezultat,"directory\n"); file_access[0]='d';break;
		case S_IFCHR: strcat(rezultat,"character-oriented device file\n"); file_access[0]='c' ; break;
		case S_IFBLK: strcat(rezultat,"block-oriented device file\n"); file_access[0]='b' ; break;
		case S_IFLNK: strcat(rezultat,"symbolic link\n"); file_access[0]='l' ; break;
		case S_IFREG: strcat(rezultat,"regular file\n"); file_access[0]='-' ; break;
		case S_IFSOCK:strcat(rezultat,"socket\n"); file_access[0]='s' ;  break;
		case S_IFIFO: strcat(rezultat,"fifo\n"); file_access[0]='p' ; break;
		default: strcat(rezultat,"unkown file type\n"); file_access[0]='u' ; break;
	}
	
	char sir[50]="";	//ID-ul device-ului in hexa, apoi in deci
	strcat(rezultat,"Device: ");
	sprintf(sir,"%x",(int)informatii.st_dev);
	strcat(rezultat,sir);
	strcat(rezultat,"h/");
	strcat(rezultat,longlongtoarray((long long)informatii.st_dev));
	strcat(rezultat,"d\t");

	strcat(rezultat,"Inode: ");  //Inodul fisierului
	strcat(rezultat, longlongtoarray((long long)informatii.st_ino));
	strcat(rezultat,"\t   ");	

	strcat(rezultat,"Links: ");  //numarul de linkuri
	strcat(rezultat,longlongtoarray((long long)informatii.st_nlink));
	strcat(rezultat,"\n");

	strcat(rezultat,"Access: (0");  //informatiile despre accesul la fisier
	long long octal_access_mode=0;  //va fi folosit la informatiile in octal
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
	strcat(rezultat,longlongtoarray(octal_access_mode)); //informatiile in octal
	strcat(rezultat,"/");
	strcat(rezultat,file_access);
	strcat(rezultat,")  ");

	struct passwd *user;	//Uid, impreuna cu numele user-ului ce corespunde cu Uid
	user=getpwuid((uid_t)informatii.st_uid);
	strcat(rezultat,"Uid: (");
	strcat(rezultat,longlongtoarray((long long)informatii.st_uid));
	strcat(rezultat,"/");
	if(user!=0)
		strcat(rezultat,user->pw_name);
	else
		strcat(rezultat,"-");
	strcat(rezultat,")   ");

	struct group *grup;		//Gid, impreuna cu numele grupului ce corespunde cu Gid
	grup=getgrgid((gid_t)informatii.st_gid);
	strcat(rezultat,"Gid: (  ");
	strcat(rezultat,longlongtoarray((long long)informatii.st_gid));
	strcat(rezultat,"/   ");
	if(grup!=0)
		strcat(rezultat,grup->gr_name);
	else
		strcat(rezultat,"-");
	strcat(rezultat,")\n");

	strcat(rezultat,"Access: ");	//Data ultimei accesari
	strcat(rezultat,time_ttoarray(informatii.st_atime));
	strcat(rezultat,"\n");

	strcat(rezultat,"Modify: ");	//Data ultimei modificari
	strcat(rezultat,time_ttoarray(informatii.st_mtime));
	strcat(rezultat,"\n");

	strcat(rezultat,"Change: ");	//Data ultimei schimbari
	strcat(rezultat,time_ttoarray(informatii.st_ctime));
}
void special_trim(char sir[]) //eliminarea spatiilor inutile din sirul "sir"
{
	while(strchr(" \n",sir[0])) 		//eliminarea spatiilor goale de la inceput
		memmove(sir,sir+1,strlen(sir));	//
	int k=strlen(sir);						//eliminarea spatiilor goale de la sfarsit
	while(strchr(" \n",sir[k-1])&&k>0)		//
	{										//
		k--;								//
		sir[k]='\0';						//
	}										//
	int i;													//eliminarea spatiilor duble din interior
	for(i=1;i<k-1;)											//
	{														//
		if(strchr(" \n",sir[i]) && strchr(" \n",sir[i+1]))	//
		{													//
			memmove(sir+i,sir+i+1,strlen(sir)-i);			//
			k--;											//
		}													//
		else												//
			i++;											//
	}														//
}
int count_words(char sir[]) //self-explanatory
{
	int i,count=0,k;
	k=strlen(sir);
	for(i=0;i<k;i++)
		if(strchr(" \n",sir[i]))
			count++;
	if(k!=0) count++;
	return count;
}
void directoryRecursion(char path[], char file_name[],char rezultat[])
{
	struct dirent *entry;
	DIR *directory;
	if(NULL == (directory=opendir(path)) )
	{
		strcat(rezultat,"\nEroare la deschiderea ");
		strcat(rezultat,path);
	}
	else
	{
		char newp[256];
		struct stat informatii;
		while(entry=readdir(directory))
		{
			int i,ok=1,k,l;
			k=strlen(file_name);
			l=strlen(entry->d_name);

			if(k!=l)
				ok=0;
			if(ok) 
				for(i=0;i<k;i++)
					if(file_name[i]!=entry->d_name[i])
						if(file_name[i]!='?')
							ok=0;
			
			strcpy(newp,path);
			strcat(newp,"/");
			strcat(newp,entry->d_name);
			if( -1 == stat(newp,&informatii) )
			{
				strcat(rezultat,"\nEroare la obtinerea informatiilor despre fisierul ");
				strcat(rezultat,newp);
			}	
			else
			{
				if(ok==1)
				{
					strcat(rezultat,"\n");
					strcat(rezultat,newp);
					
					strcat(rezultat,"\n    File type: ");
					char file_access[11];
					switch(informatii.st_mode & S_IFMT)		////file_access[0] va fi folosit la informatiile despre accesul la fisier
					{
						case S_IFDIR: strcat(rezultat,"directory\n"); file_access[0]='d';break;
						case S_IFCHR: strcat(rezultat,"character-oriented device file\n"); file_access[0]='c' ; break;
						case S_IFBLK: strcat(rezultat,"block-oriented device file\n"); file_access[0]='b' ; break;
						case S_IFLNK: strcat(rezultat,"symbolic link\n"); file_access[0]='l' ; break;
						case S_IFREG: strcat(rezultat,"regular file\n"); file_access[0]='-' ; break;
						case S_IFSOCK:strcat(rezultat,"socket\n"); file_access[0]='s' ;  break;
						case S_IFIFO: strcat(rezultat,"fifo\n"); file_access[0]='p' ; break;
						default: strcat(rezultat,"unkown file type\n"); file_access[0]='u' ; break;
					}

					strcat(rezultat,"  Last change: ");	//Data ultimei schimbari -- deoarece
														//data crearii nu este disponibila i.e. nu poate fi determinata
					strcat(rezultat,time_ttoarray(informatii.st_ctime));

					strcat(rezultat,"\n  Last modify: ");	//Data ultimei modificari
					strcat(rezultat,time_ttoarray(informatii.st_mtime));

					strcat(rezultat,"\n  Last access: ");	//Data ultimei accesari
					strcat(rezultat,time_ttoarray(informatii.st_atime));

					strcat(rezultat,"\nSize in bytes: "); //dimensiunea fisierului
					strcat(rezultat,longlongtoarray((long long)informatii.st_size));

					strcat(rezultat,"\n  File access: (0");  //informatiile despre accesul la fisier
					long long octal_access_mode=0;  //va fi folosit la informatiile in octal	
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
					strcat(rezultat,longlongtoarray(octal_access_mode)); //informatiile in octal
					strcat(rezultat,"/");
					strcat(rezultat,file_access);
					strcat(rezultat,")  ");
					strcat(rezultat,"\n");
				}
				if(strcmp(entry->d_name,"..")!=0 && strcmp(entry->d_name,".")!=0)
					if(S_ISDIR(informatii.st_mode))
						directoryRecursion(newp,file_name,rezultat);
			}
		}
		closedir(directory);
	}
}
void myFind(char path[], char file_name[], char rezultat[])
{
	strcpy(rezultat,"");
	DIR *directory;
	directory=opendir(path);
	if(directory)
	{
		closedir(directory);
		directoryRecursion(path,file_name,rezultat);
	}
	else
	{
		sprintf(rezultat,"Eroare la deschiderea %s",path);
	}
}
void myls(char path[],char rezultat[])
{
	strcpy(rezultat,"");
	DIR *directory;
	directory=opendir(path);
	if(directory)
	{
		struct dirent *entry;
		while(entry=readdir(directory))
		{
			if(entry->d_name[0]!='.')
			{
				strcat(rezultat,"\n");
				strcat(rezultat,entry->d_name);
			}
		}
		closedir(directory);
		strcat(rezultat,"\n");
	}
	else
	{
		sprintf(rezultat,"Eroare la deschiderea %s",path);
	}
}
void myCd(char directory[],char rezultat[])
{
	if(-1 == chdir(directory))
		strcpy(rezultat,"Eroare la deschiderea directorului");
	else
		strcat(rezultat,directory);
}
void myHelp(char functie[],char rezultat[])
{
	if(strcmp(functie,"help")==0)
	{
		strcpy(rezultat,"Comanda help ofera informatii despre comenzi si are nevoie de 0 sau 1 argument: \"help [comanda]\".\n");
		strcat(rezultat,"Comenzi disponibile: stat, find, cd, ls, help, pwd, quit.\n");
		strcat(rezultat,"Pentru informatii despre o anumita comanda, tastati \"help [comanda]\".\n");
		strcat(rezultat,"Ex: 'help', 'help find', 'help stat'.");
	}
	else if(strcmp(functie,"ls")==0)
	{
		strcpy(rezultat,"Comanda ls afiseaza continutul directorului dat si are nevoie de 0 sau 1 argument: \"ls [director]\".\n");
		strcat(rezultat,"Daca [director] este un camp gol, se considera ca fiind directorul curent.\n");
		strcat(rezultat,"Ex: 'ls', 'ls ..', 'ls /tmp'.");
	}
	else if(strcmp(functie,"cd")==0)
	{
		strcpy(rezultat,"Comanda cd schimba directorul curent in cel specificat si are nevoie de 0 sau 1 argument: \"cd [director]\".\n");
		strcat(rezultat,"Daca [director] este un camp gol, se considera ca fiind directorul home.\n");
		strcat(rezultat,"Ex: 'cd', 'cd ..', 'cd /tmp'.");
	}
	else if(strcmp(functie,"stat")==0)
	{
		strcpy(rezultat,"Comanda stat ofera informatii despre fisierul dat si are nevoie de 1 argument: \"stat [fisier]\".\n");
		strcat(rezultat,"Ex: 'stat director', 'stat myConsole', 'stat ..'.");
	}
	else if(strcmp(functie,"find")==0)
	{
		strcpy(rezultat,"Comanda find cauta recursiv fisierul (argv[2]) in directorul dat (argv[1]), ofera cateva informatii despre acesta si are nevoie de 1 sau 2 argumente: \"cd [director] file_name\".\n");
		strcat(rezultat,"Daca [director] este un camp gol, se considera ca fiind directorul curent.\n");
		strcat(rezultat,"Pentru numele fisierului cautat se poate folosi caracterul '?' pentru a desemna orice caracter.\n");
		strcat(rezultat,"Ex: 'find ..', 'find ../.. a.out', 'find ../.. ??', 'find . myConsole', 'find myConsole'");
	}
	else if(strcmp(functie,"quit")==0)
		strcpy(rezultat,"Comanda quit delogheaza utilizatorul si nu are argumente: 'quit'.");
	else if(strcmp(functie,"pwd")==0)
		strcpy(rezultat,"Comanda pwd afiseaza directorul curent si nu are argumente: 'pwd'.");
	else
		strcpy(rezultat,"Comanda necunoscuta.");
}
void myPwd(char rezultat[])
{
	strcpy(rezultat,"Directorul curent este ");
	strcat(rezultat,getcwd(NULL,256));
}
void manipulate(char sir[]) //manipularea sirului primit de catre procesul fiu, "sir" primind dupa
							//apelul manipulate(sir) rezultatul ce va fi apoi trimis catre parinte 
{
	if(count_words(sir)>5||count_words(sir)==0)
	{
		strcpy(sir,"Comanda necunoscuta");
	}
	else	//determinarea comenzii ce va trebui folosita
	{
		char *p, sir2[1000],cuvinte[6][100];	
		int count=0;
		p=strtok(sir,"\n ");
		while(p)
		{
			strcpy(cuvinte[count],p);
			count++;
			p=strtok(NULL,"\n ");
		}

		if(strcmp(cuvinte[0],"stat")==0)
			if(count==2)
				myStat(cuvinte[1],sir);
			else
				strcpy(sir,"Comanda stat are nevoie de un argument. Exemplu: \"stat file.txt\"");
		else if(strcmp(cuvinte[0],"quit")==0)
			if(count!=1)
				strcpy(sir,"Did you mean 'quit'?");
			else{}
		else if(strcmp(cuvinte[0],"find")==0)
			if(count==3)
			{
				myFind(cuvinte[1],cuvinte[2],sir);
				if(strcmp(sir,"")==0)
					strcpy(sir,"Niciun rezultat");
			}
			else if(count==2)
			{
				myFind(".",cuvinte[1],sir);
				if(strcmp(sir,"")==0)
					strcpy(sir,"Niciun rezultat");
			}
				else
					strcpy(sir,"Comanda find are nevoie de 1 sau 2 argumente. Exemplu: \"find path ceva.cpp\"");
		else if(strcmp(cuvinte[0],"cd")==0)
			if(count==2 || count==1)
			{
				strcpy(sir,"cd ");
				if(count==1)
				{
					struct passwd *user;	//Uid, impreuna cu numele user-ului ce corespunde cu Uid
					user=getpwuid(geteuid());
					strcpy(cuvinte[1],user->pw_dir);
				}
				myCd(cuvinte[1],sir);
			}
			else
				strcpy(sir,"Comanda cd are nevoie de un argument (exemplu: \"cd ..\"), sau 0 argumente pentru home directory.");
		else if(strcmp(cuvinte[0],"ls")==0)
			if(count==2 || count ==1)
			{
				if(count==1)
				{
					char cwd[256];
					strcpy(cuvinte[1],getcwd(cwd,256));
				}
				myls(cuvinte[1],sir);
			}
			else
				strcpy(sir,"Comanda ls are nevoie de 0 sau 1 argument: \"ls [path]\"");
		else if(strcmp(cuvinte[0],"help")==0)
			if(count==1 || count==2)
			{
				if(count==1)
					myHelp("help",sir);
				else
					myHelp(cuvinte[1],sir);
			}
			else
				strcpy(sir,"Comanda help are nevoie de 0 sau 1 argument: \"help [comanda]\"");
		else if(strcmp(cuvinte[0],"pwd")==0)
		{
			if(count==1)
				myPwd(sir);
			else
				strcpy(sir,"Comanda pwd nu are argumente");
		}
		else strcpy(sir,"Comanda necunoscuta");
	}
}
int main(int argc, char* argv[])
{
	int communication_type,fd;
	FILE* fp=fopen(".users.txt","w");
	if(fp==NULL)
	{
		printf("%s\n","Eroare la crearea users.txt");
		exit(1);
	}
	fprintf(fp,"chile.ovidiu\ncineva.altcineva\nceva\nadmin\novidiu.chile");
	fclose(fp);

	FILE* fpass=fopen(".pass.txt","w");
	if(fpass==NULL)
	{
		printf("%s\n","Eroare la creare pass.txt");
		exit(1);
	}
	fprintf(fpass,"1\ncineva\nceva\nadmin\nqwerty");
	fclose(fpass);
	char communication_type_string[256];//1-pipe,2-fifo,3-socket,everything else-nothing
	printf("%s\n", "Alegeti tipul de comunicare dorit: 1-pipe, 2-fifo, 3-socket.");
	while(1)
	{
		fgets(communication_type_string,256,stdin);
		int ok=0,i;
		for(i=0;i<strlen(communication_type_string);i++)
			if(!strchr(" \n",communication_type_string[i]))
				ok=1;
		if(ok)
		{
			special_trim(communication_type_string);
			if(strcmp(communication_type_string,"1")==0 || strcmp(communication_type_string,"pipe")==0)
				{printf("%s","Ati ales pipe.\n");communication_type=1;break;}
			else 
				if(strcmp(communication_type_string,"2")==0 || strcmp(communication_type_string,"fifo")==0)
					{printf("%s","Ati ales fifo.\n");communication_type=2;break;}
				else
					if(strcmp(communication_type_string,"3")==0 || strcmp(communication_type_string,"socket")==0)
						{printf("%s","Ati ales socket.\n");communication_type=3;break;}
					else
						if(strcmp(communication_type_string,"quit")==0) 
							exit(0);
						else
							printf("%s","Invalid number. Type quit if you want to exit myConsole.\n");
		}
	}
	fflush(stdout);
	int pid,pipefd1[2],pipefd2[2],fifo_fd_fiu_read,fifo_fd_fiu_write,fifo_fd_tata_read,fifo_fd_tata_write;
	char *sir;
	sir=malloc(MAX_CHAR_SIZE);
	switch(communication_type)
	{
		case 1:
		{
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
			break;
		}
		case 2:
		{
			mknod(".fifo.fifo1", S_IFIFO | 0777, 0);
			mknod(".fifo.fifo2", S_IFIFO | 0777, 0);
			//nu e nevoie de tratarea cazului cand deja exista
			break;
		}
		case 3:
		{
			if (socketpair(AF_LOCAL, SOCK_STREAM, 0, pipefd1) < 0) //tata->fiu
	        { 
	        	perror("Err... socketpair1"); 
	        	exit(1); 
	      	}
	      	if (socketpair(AF_LOCAL, SOCK_STREAM, 0, pipefd2) < 0) //fiu->tata
	        { 
	        	perror("Err... socketpair2"); 
	        	exit(1); 
	      	}
			break;
		}
	}
	switch(pid=fork())		//fork
	{
		case -1:perror("eroare la fork"); exit(1);		//eroare
		case 0:		//procesul fiu
		{
			char sirDinFiu[MAX_CHAR_SIZE],users[200];
			int nrBytes,ok=0;
			switch(communication_type)
			{
				case 1:
				{
					close(pipefd1[1]);
					close(pipefd2[0]);
					break;
				}
				case 2:
				{
					if(-1 == (pipefd1[0]=open(".fifo.fifo1",O_RDONLY)))
					{
						perror("fifo1");
						exit(40);
					}
					if(-1 == (pipefd2[1]=open(".fifo.fifo2",O_WRONLY)))
					{
						perror("fifo2");
						exit(40);
					}
					break;
				}
				case 3:
				{
					close(pipefd1[1]);
					close(pipefd2[0]);
					break;
				}
			}
			//citire user din tata
			int counter1=0;
			while(1)
			{
				if( (nrBytes=read(pipefd1[0],&sirDinFiu,MAX_CHAR_SIZE)) <0) 
					ok=2;
				else
				{
					sirDinFiu[nrBytes]='\0';
					//verificare daca exista user
					if(-1 == (fd=open(".users.txt",O_RDONLY)))
					{
						perror(".users.txt");
						ok=2;
						write(pipefd2[1],&ok,sizeof(int));
						exit(30);
					}
					if(-1==read(fd,users,200))
						ok=2;
					else
					{
						ok=0;
						char *p;
						p=strtok(users,"\n ");
						counter1=0;
						while(p)
						{
							if(strcmp(p,sirDinFiu)==0)
							{
								ok=1;
								break;
							}
							counter1++;
							p=strtok(NULL,"\n ");
						}
						close(fd);
					}
				}
				//trimitere raspuns catre tata daca exista user
				if(-1 == write(pipefd2[1],&ok,sizeof(int)))
					break;
				if(ok>0)
					break;
			}

			///////////////pasword////////////////////////
			ok=0;
			while(1)
			{
				if( (nrBytes=read(pipefd1[0],&sirDinFiu,MAX_CHAR_SIZE)) <0) 
					ok=2;
				else
				{
					sirDinFiu[nrBytes]='\0';
					//verificare daca parola e cea corespunzatoare userului
					if(-1 == (fd=open(".pass.txt",O_RDONLY)))
					{
						perror(".pass.txt");
						ok=2;
						write(pipefd2[1],&ok,sizeof(int));
						exit(30);
					}
					if(-1==(nrBytes=read(fd,users,200)))
						ok=2;
					else
					{
						ok=0;
						users[nrBytes]='\0';
						char *p;
						p=strtok(users,"\n ");
						int counter2=0;
						while(p)
						{
							if(strcmp(p,sirDinFiu)==0)
							{
								if(counter1==counter2)
									ok=1;
								else
									ok=0;
								break;
							}
							counter2++;
							p=strtok(NULL,"\n ");
						}
						close(fd);
					}
				}
				//trimitere raspuns catre tata daca exista user
				if(-1 == write(pipefd2[1],&ok,sizeof(int)))
					break;
				if(ok>0)
					break;
			}

			///////////////paswordend////////////////////////

			while(0< (nrBytes=read(pipefd1[0],sirDinFiu,MAX_CHAR_SIZE)))
			{
				sirDinFiu[nrBytes]='\0';
				manipulate(sirDinFiu);
				if(-1 == write(pipefd2[1],sirDinFiu,strlen(sirDinFiu)))
					break;
				if(strcmp(sirDinFiu,"quit")==0) 
					break;
			}
			close(pipefd1[0]);
			close(pipefd2[1]);
			exit(0);
		}
		default:   //procesul tata
		{
			switch(communication_type)
			{
				case 1:
				{
					close(pipefd1[0]);
					close(pipefd2[1]);
					break;
				}
				case 2:
				{
					if(-1 == (pipefd1[1]=open(".fifo.fifo1",O_WRONLY)))
					{
						perror("fifo1");
						exit(40);
					}
					if(-1 == (pipefd2[0]=open(".fifo.fifo2",O_RDONLY)))
					{
						perror("fifo2");
						exit(40);
					}
					break;
				}
				case 3:
				{
					close(pipefd1[0]);
					close(pipefd2[1]);
					break;
				}
			}
			printf("login as: ");
			int ok,i,nr;
			char* utilizator=(char*)malloc(MAX_CHAR_SIZE);
			while(1)
			{
				fgets(sir,MAX_CHAR_SIZE,stdin);
				ok=0;
				for(i=0;i<strlen(sir);i++)
					if(!strchr(" \n",sir[i]))
						ok=1;
				if(ok==0)
					printf("Introduceti userul\nlogin as: ");
				else
				{
					special_trim(sir);
					strcpy(utilizator,sir);
					if(-1==write(pipefd1[1],sir,strlen(sir)))
						exit(99);
					nr=read(pipefd2[0],&ok,sizeof(int));
					if(nr==-1) exit(99);
					printf("Numar de octeti primiti: %d \n",sizeof(int));
					fflush(stdout);
					if(ok==1)
						break;
					else if(ok==2)
							exit(30);
						 else
							printf("User inexistent\nlogin as: ");
				}
			}

			///////////////pasword////////////////////////
			printf("%s","Password: ");
			while(1)
			{
				fgets(sir,MAX_CHAR_SIZE,stdin);
				ok=0;
				for(i=0;i<strlen(sir);i++)
					if(!strchr(" \n",sir[i]))
						ok=1;
				if(ok==0)
					printf("%s","Acces denied\nPassword: ");
				else
				{
					special_trim(sir);
					if(-1==write(pipefd1[1],sir,strlen(sir)))
						exit(99);
					nr=read(pipefd2[0],&ok,sizeof(int));
					if(nr==-1) exit(99);
					printf("Numar de octeti primiti: %d \n",sizeof(int));
					fflush(stdout);
					if(ok==1)
						break;
					else if(ok==2)
							exit(30);
						 else
							printf("%s","Acces denied\nPassword: ");
				}
			}

			///////////////paswordend////////////////////////
			
			
			unlink(".users.txt");
			unlink(".pass.txt");
			printf("%s\n\n","Access granted!");
			printf("%s\n","Comenzi disponibile: stat, find, cd, ls, help, pwd, quit.");

			struct passwd *user;	//Uid, impreuna cu numele user-ului ce corespunde cu Uid
			char *host=(char*)malloc(100);
			char *cwd=(char*)malloc(256);
			user=getpwuid(geteuid());
			gethostname(host,100);
			cwd=getcwd(cwd,256);
			if(strstr(cwd,user->pw_dir))
			{
				cwd[0]='~';
				strcpy(cwd+1,cwd+strlen(user->pw_dir));
			}
			printf("[%s@%s %s] $ ",user->pw_name,host,cwd);
			fflush(stdout);
			while(fgets(sir,MAX_CHAR_SIZE,stdin))
			{
				i=0;
				ok=0;
				for(i=0;i<strlen(sir);i++)
					if(!strchr(" \n",sir[i]))
						ok=1;
				if(ok==1)
				{
					special_trim(sir);
					if(-1==write(pipefd1[1],sir,strlen(sir)))
						break;
					if(-1 == (nr=read(pipefd2[0],sir,MAX_CHAR_SIZE)))
						break;
					sir[nr]='\0';
					printf("Numar de octeti primiti: %d \n",nr);
					fflush(stdout);
					if(nr!=0)
					{
						if(strcmp(sir,"quit")==0) 
							break;
						if(sir[0]=='c'&&sir[1]=='d')
						{
							chdir(sir+3);
							strcpy(sir,"Director curent schimbat la ");
							strcat(sir, getcwd(cwd,256));
						}
						printf("%s\n",sir);
						fflush(stdout);
					}
					free(sir);
					sir=malloc(MAX_CHAR_SIZE);	
				}
				user=getpwuid(geteuid());
				gethostname(host,100);
				cwd=getcwd(cwd,256);
				if(strstr(cwd,user->pw_dir))
				{
					cwd[0]='~';
					strcpy(cwd+1,cwd+strlen(user->pw_dir));
				}
				printf("[%s@%s %s] $ ",user->pw_name,host,cwd);
				fflush(stdout);
			}
			if(communication_type==2)
			{
				unlink(".fifo.fifo1");
				unlink(".fifo.fifo2");
			}
			else
			{
				close(pipefd1[1]);
				close(pipefd2[0]);
			}
		}
	}
	return 0;
}
