void sig_handler()
{
	int status ;
	int pid=waitpid(-1,&status,WNOHANG | WUNTRACED) ;

	char name[1000] ;
	name[0]='\0' ;
	if(pid>0 && WIFEXITED(status)>0)
	{
		for(int i=0;i<1000;i++)
		{
			if(A[i].pid==pid&&A[i].flag==true)
			{
				A[i].flag=false ;
				strcpy(name,A[i].name) ;
				break ;
			}
		}
		fprintf(stderr,"\n%s with pid %d exited normally\n",name,pid) ;
	}
	
	if(pid>0 && WIFSIGNALED(status)>0)
	{
		for(int i=0;i<1000;i++)
		{
			if(A[i].pid==pid&&A[i].flag==true)
			{
				A[i].flag=false ;
				strcpy(name,A[i].name) ;
				break ;
			}
		}
		fprintf(stderr,"\n%s with pid %d exited abnormally\n",name,pid) ;		
	}
}