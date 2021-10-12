
void bgp(char command[])
{
	char* list[1000] ;
	char* delimiter=" " ;
	char* word=strtok(command,delimiter) ;
	int cnt=0 ;
	pid_t pid=getpid() ;
	while(word!=NULL)
	{
		if(strcmp(word,"&")!=0)
		{
			list[cnt++]=word ;
			word=strtok(NULL,delimiter) ;
	    }
	    else
	    	break ;
	}
	list[cnt]=NULL ;
	bool f=false ;
	for(int i=0;i<1000;i++)
	{
		if(A[i].flag==false)
		{
			f=true ;
			break ;
		}
	}
	if(f==false)
	{
		printf("This background process cannot be executed since the count of background process exceeded 1000\n") ;
		return ;
	}
	int x=fork() ;
	if(x==0)
	{
		int y=execvp(list[0],list) ;
		if(y!=0)
		{
			printf("Cannot run background process\n") ;
			perror("execvp()") ;
		}
	}
	else
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
		printf("%d\n",x) ;
	}
}