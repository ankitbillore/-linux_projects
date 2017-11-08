 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>

typedef enum list
{
	EXIT=0,NEW_FILE,LIST_FILE,OPEN_FILE,DELETE_FILE,BUILD_PROJECT,RUN_PROJECT,DEBUG_PROJECT,MEMORY_CHECK,CLEAN_PROJECT

}list_t;

typedef struct file
{
	char filename[50];
}file_t;


typedef struct project
{
	char projectName[40];
	int count;
	file_t files[10];
}project_t;


project_t project;



void update_project()
{
	int fd;


	fd= open(project.projectName,O_WRONLY|O_TRUNC|O_CREAT,0754);

	write(fd,&project,sizeof(project_t));
	
	close(fd);

}

void creat_project(char*dirpath,char*filename)
{
	int result,fd;
	
	result=chdir(dirpath);
	if(result==-1)
	{
    
		 printf("\n flile doesnot exists");

		result=mkdir(dirpath,0755);

		if(result==-1)//fails while creating directory
		{
			printf("\nerror in opening file");
			
		}
		else
		{
			chdir(dirpath);
			strcpy(project.projectName,filename);
			project.count=0;
			update_project();
                }
	 }

		else
		{
	        fd= open(filename,O_RDONLY);
			read(fd,&project,sizeof(project_t));
			close(fd);
		}
}



int exec_command(char command[])
{
	int i=0,ret,err,status;
	char *ptr,*args[32];
	//split command into tokens 
	ptr=strtok(command," \t\n");
	args[i++]=ptr;
	while(ptr!=NULL)
	{
		ptr=strtok(NULL," \t\n");
		args[i++]=ptr;
	}
	ret=fork();
	if(ret==0)
	{
		err=execvp(args[0],args);
		_exit(1);
	}

		wait(&status);
		return WEXITSTATUS(status);
}

 new_file()
 {
	 char filename[20];
	 char cmd[64];
	 printf("\n\tENTER FILE NAME	:	");
	 scanf("%s",filename);
     strcpy(project.files[project.count].filename,filename);
	 project.count=project.count+1;
	 update_project();
	 strcpy(cmd,"touch ");
	 strcat(cmd,filename);
	 exec_command(cmd);
	
 }
void open_file()
{
	char cmd[64],buff[64];
	printf("Enter filename,to be opened		:	");
	scanf("%s",buff);
	strcpy(cmd,"vim ");
	strcat(cmd,buff);
	exec_command(cmd);
}


list_file()
{
	int i;
	for (i=0;i<=project.count;i++)
	{
		printf("%s\t",project.files[i].filename);
	}

}



delete_file()
{
	int i,j;
	char filename[20];
	printf("\n\t ENTER FILE NAME TO BE DLETED	:	");
	scanf("%s",filename);

	for (i=0;i<=project.count;i++)
	{
	
		if((strcmp(filename,project.files[i].filename))==0)
		{
		for (j=i;j<project.count;j++)
		{
			strcpy(project.files[j].filename,project.files[j+1].filename);
			
		}
		strcpy(project.files[project.count].filename,"");
		}
    }
	project.count--;
	update_project();
	unlink(filename);



}

void build_project()
{

	int success=1,ret,i;
	char temp[50];
	char* ptr;
	char command[50];
	for(i=0;i<project.count;i++)
	{
		if(strstr(project.files[i].filename,".c")!=NULL)
		{
			strcpy(command,"gcc -ggdb3 -c ");
			strcat(command,project.files[i].filename);
			ret=exec_command(command);
			if (ret !=0)
			{
				success=0;
			}

		}
	}

	if(success==0)
	{
		printf("compilation faild \n");
		return ;
	}


	strcpy(command,"gcc -ggdb3 -o ");
			strcpy(temp,project.projectName);
			ptr=strtok(temp,".proj");
			strcpy(temp , ptr);
			strcat(temp,".out");
			strcat(command,temp);
			strcat(command," ");
		
	for(i=0;i<project.count;i++)
	{
		strcpy(temp,project.files[i].filename);
			ptr=strtok(temp,".c");
			strcpy(temp , ptr);
			strcat(ptr,".o");
			strcat(command,temp);
			strcat(command," ");

		
	
	}
	ret=exec_command(command);
	if(ret==0)
	printf("\n linking successfull\n");
		else
				
		printf("\n linking unsuccessfull\n");
		
}


void run_project()
{
	int ret;
	char temp[50];
	char command[50];
	
	char* ptr;
	
	strcpy(temp,project.projectName);
	ptr=strstr(temp,".proj");
	strcpy(ptr,".out");
	strcpy(command,"./");
	strcat(command,temp);
	
	ret=exec_command(command);


}

debug_project()
{
	int ret;
	char temp[50];
	char command[49];
	char* ptr;

	
	strcpy(temp,project.projectName);
	ptr=strstr(temp,".proj");
	strcpy(command,".out");
	strcpy(command,"gdb ./");
	strcat(command,temp);
	
	ret=exec_command(command);
}

memory_check()
{



	int ret;
	char temp[50];
	char command[49];
	char* ptr;

	
	strcpy(temp,project.projectName);
	ptr=strstr(temp,".proj");
	strcpy(ptr,".out");
	strcpy(command,"valgrind ./");
	strcat(command,temp);
	
	ret=exec_command(command);
}

clean_project()
{


	int success=1,ret,i;
	char temp[50];
	char* ptr;
	char command[50];
	for(i=0;i<project.count;i++)
	{
		if(strstr(project.files[i].filename,".c")!=NULL)
		{
			strcpy(temp,project.files[i].filename);
			
			ptr=strstr(temp,".c");
			strcpy(ptr,".o");
			unlink(temp);
		}

			strcpy(temp,project.projectName);
			ptr=strstr(temp,".proj");
			strcpy(ptr,".out");
			unlink(temp);
	}


			
		
}



list_t menu_list()
{

list_t choice;

	printf("\n0.EXIT\n");
		
	printf("1.New FIle\n");

	printf("2.List File\n");

	printf("3.Open file\n");
	
	printf("4.Delete File\n");

	printf("5.Build Project\n");

	printf("6.Run Project\n");

	printf("7.Debug Project\n");

	printf("8.Memory Check\n");

	printf("9.Clean Project\n");

	printf("Enter Your Choice	:	");
	
	scanf("%d",&choice);

	return choice;
	
}


int main(int argc,char*argv[])
{
	int choice;
     creat_project(argv[1],argv[2]);

	while((choice=menu_list())!=EXIT)
{
		switch(choice)
		{
			case NEW_FILE:
				new_file();
				break;

			case LIST_FILE :
				list_file();
				break;
			case OPEN_FILE:
				open_file();
				break;
			case DELETE_FILE:
				delete_file();
				break;
			
			case BUILD_PROJECT:
				build_project();
				break;

			case RUN_PROJECT:
                run_project();
				break;

			case DEBUG_PROJECT:
				debug_project();
				break;

			case MEMORY_CHECK:
				memory_check();
				break;

			case CLEAN_PROJECT:
				clean_project();
				break;
		}
	}

	return 0;

}