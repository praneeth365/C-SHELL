
void fgp(char command[])
{
	char* list[1000] ;
	char* delimiter=" " ;
	char* word=strtok(command,delimiter) ;
	int cnt=0 ;
	while(word!=NULL)
	{
		if(strcmp(word,"&")!=0)
		{
			list[cnt++]=word ;
			word=strtok(NULL,delimiter) ;
		}
	}
	list[cnt]=NULL ;
	int x=fork() ;
	if(x==0)
	{
		int x=execvp(list[0],list) ;
		if(x!=0)
		{
			printf("Cannot run background process\n") ;
			perror("execvp()") ;
		}
	}
	else
	{
		curr_pid=x ;
		printf("%d\n",x) ;
		int status ;
		waitpid(x,&status,WUNTRACED) ;
		if(WIFSTOPPED(status))
		{
			for(int i=0;i<1000;i++)
			{
				if(A[i].flag==false)
				{
					A[i].flag=true ;
					A[i].pid=x ;
					A[i].job_order=job_num ;
					job_num++ ;
					strcpy(A[i].name,list[0]) ;
					break ;
				}
			}
			return ;
		}		
	}	
}