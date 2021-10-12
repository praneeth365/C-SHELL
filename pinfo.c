
void pinfo(char command[])
{
	pid_t processid=getpid();
	if(strlen(command)!=5)
	{
		processid=atoi(command+6) ;
	}
	char procid[1000] ;
	sprintf(procid,"%d",processid) ;
	char path1[1000] ;
	path1[0]='\0' ;
	strcat(path1,"/proc/") ;
	strcat(path1,procid) ;
	char path2[1000] ;
	path2[0]='\0' ;
	strcat(path2,path1) ;
	strcat(path2,"/") ;
	strcat(path2,"exe") ;
	strcat(path1,"/") ;
	strcat(path1,"stat") ;
	char buf[1000] ;
	FILE* fp=fopen(path1,"r") ;
	if(fp==NULL)
	{
		printf("Process with pid %s does not exist\n",procid) ;
		perror("fopen()") ;
	}
	else
	{
		fgets(buf,1000,fp) ;
		char* delimiter=" " ;
		char* word=strtok(buf,delimiter) ;
		char* info[1000] ;
		int cnt=0 ;
		while(word)
		{
			info[cnt++]=word ;
			word=strtok(NULL,delimiter) ;
		}
		char* status=info[2] ;
		if(strlen(command)==5)
		{
			if(strcmp(status,"R")==0||strcmp(status,"S")==0)
				strcat(status,"+") ;
		}
		char* vmem=info[22] ;
		char exepath[1000] ;
		readlink(path2,exepath,1000) ;
	    int n=strlen(root) ;
	    bool flag=true ;
	    if(strlen(exepath)>=strlen(root))
	    {
	        for(int i=0;i<n;i++)
	        {
	            if(exepath[i]!=root[i])
	            {
	                flag=false ;
	                break ;
	            }
	        }
	    }
	    else
	    {
	        n=0 ;
	    }
	    if(flag==false)
	    {
	        n=0 ;
	    }	
		printf("pid -- %d\n",processid) ;
		printf("Process Status -- %s\n",status) ;
		printf("memory -- %s  {Virtual Memory}\n",vmem) ;
		printf("Executable Path -- %s\n",exepath+n) ;
}
}