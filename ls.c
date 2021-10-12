
void ls(char command[])
{
	char* permission_map[8] ;
	permission_map[7]="rwx" ;
	permission_map[6]="rw-" ;
	permission_map[5]="r-x" ;
	permission_map[4]="r--" ;
	permission_map[3]="-wx" ;
	permission_map[2]="-w-" ;
	permission_map[1]="--x" ;
	permission_map[0]="---" ;
	char* months[12] ;
	months[0]="Jan" ;
	months[1]="Feb" ;
	months[2]="Mar" ;
	months[3]="Apr" ;
	months[4]="May" ;
	months[5]="Jun" ;
	months[6]="Jul" ;
	months[7]="Aug" ;
	months[8]="Sep" ;
	months[9]="Oct" ;
	months[10]="Nov" ;
	months[11]="Dec" ;		
	if(strcmp(command,"ls")==0||strcmp(command,"ls -a")==0||strcmp(command,"ls .")==0)
	{
		bool flag=true ;
		if(strlen(command)==2||strcmp(command+3,".")==0)
		{
			flag=false ;
		}
		struct dirent* dir ;
		DIR* directory=opendir(".") ;
		if(directory==NULL)
		{
			printf("Unable to open direcotry\n") ;
			perror("opendir()") ;
		}
		else
		{
			while((dir=readdir(directory))!=NULL)
			{
				if(flag||dir->d_name[0]!='.')
				printf("%s\n",dir->d_name) ;
			}
		}
	}
	else if(strcmp(command+3,"..")==0)
	{
		bool flag=false ;
		struct dirent* dir ;
		char curr_dir[1000] ;
		if(getcwd(curr_dir,1000)==NULL)
		{
			printf("Unable to open directory\n") ;
			perror("get_cwd()") ;
		}
		int x=chdir("..") ;
		if(x!=0)
		{
			printf("Unable to change Directory\n") ;
			perror("chdir()") ;
		}
		DIR* directory=opendir(".") ;
		if(directory==NULL)
		{
			printf("Unable to open direcotry\n") ;
			perror("opendir()") ;
		}
		else
		{
			while((dir=readdir(directory))!=NULL)
			{
				if(flag||dir->d_name[0]!='.')
				printf("%s\n",dir->d_name) ;
			}
		}
		int y=chdir(curr_dir) ;
		if(y!=0)
		{
			printf("Unable to change Directory\n") ;
			perror("chdir()") ;
		}		
	}
	else if(strcmp(command+3,"~")==0)
	{
		char* username=(char*)malloc(1000) ;
		uid_t uid=getuid() ;
		struct passwd* udetails=getpwuid(uid) ;
		if(udetails==NULL)
		{
		    printf("Cannot retrieve user name\n") ;
		    perror("getpwuid(): ") ;
		}
		else
		{
		    username=udetails->pw_name ;
		}		
		bool flag=false ;
		struct dirent* dir ;
		char home[1000]="/home/" ;
		strcat(home,username) ;
		DIR* directory=opendir(home) ;
		if(directory==NULL)
		{
			printf("Unable to open direcotry\n") ;
			perror("opendir()") ;
		}
		else
		{
			while((dir=readdir(directory))!=NULL)
			{
				if(flag||dir->d_name[0]!='.')
				printf("%s\n",dir->d_name) ;
			}
		}		
	}
	/*else if(strcmp(command,"ls -l")==0||strcmp(command,"ls -al")==0||strcmp(command,"ls -la")==0||strcmp(command,"ls -a -l")==0||strcmp(command,"ls -l -a")==0)
	{	
		char* list_files[1000] ;
		int cnt=0 ;
		bool flag=true ;
		if(strcmp(command+3,"-l")==0)
		{
			flag=false ;
		}
		struct dirent* dir ;
		DIR* directory=opendir(".") ;
		if(directory==NULL)
		{
			printf("Unable to open direcotry\n") ;
			perror("opendir()") ;
		}
		else
		{
			while((dir=readdir(directory))!=NULL)
			{
				if(flag||dir->d_name[0]!='.')
				list_files[cnt++]=dir->d_name ;
			}
		}
		char permissions[cnt][1000] ;
		int links[cnt] ;
		char* owner[cnt] ;
		char* group[cnt] ;
		int size[cnt] ;
		char* month[cnt] ;
		char day[cnt][3] ;
		int Day[cnt] ;
		char hour[cnt][6] ;
		bool year_flag[cnt] ;
		int int_year[cnt] ;
		for(int i=0;i<cnt;i++)
			year_flag[i]=false ;
		int total=0 ;
		for(int i=0;i<cnt;i++)
		{
			struct stat file ;
			if(stat(list_files[i],&file) < 0)    
	        {
	        	printf("Could not get file permissions\n") ;
	        	perror("stat()") ;
	        }
	        else
	        {
	        	total+=file.st_blocks ;
	          
	        	int a=file.st_mode ;
	        	int bits[3] ;
	        	int b = 0,p = 1;
			    while (a!= 0)
			    {
			        b+=(a%8)*p;
			        a/=8;
			        p*=10;
			    }
	        	for(int j=2;j>=0;j--)
	        	{
	        		bits[j]=b%10 ;
	        		b=b/10 ;
	        	}
	        	bool check=S_ISDIR(file.st_mode) ;
	        	if(check)
	        	{
	        		permissions[i][0]='d' ;
	        		permissions[i][1]='\0' ;
	        	}
	        	else
	        	{
	        		permissions[i][0]='-' ;
	        		permissions[i][1]='\0' ;
	        	}
	        	strcat(permissions[i],permission_map[bits[0]]) ;
	        	strcat(permissions[i],permission_map[bits[1]]) ;
	        	strcat(permissions[i],permission_map[bits[2]]) ;
	        	

	        	 links[i]=file.st_nlink ; 

                
	        	 uid_t ownerid=file.st_uid ;
	        	 uid_t groupid=file.st_gid ;
			    struct passwd* ownerdet=getpwuid(ownerid) ;
			    if(ownerdet==NULL)
			    {
			        printf("Cannot retrieve user name\n") ;
			        perror("getpwuid(): ") ;
			    }
			    else
			    {
			        owner[i]=ownerdet->pw_name ;
			    }

			    struct passwd* groupdet=getpwuid(groupid) ;
			    if(groupdet==NULL)
			    {
			        printf("Cannot retrieve user name\n") ;
			        perror("getpwuid(): ") ;
			    }
			    else
			    {
			        group[i]=groupdet->pw_name ;
			    }				    	        	 
			    

			    size[i]=file.st_size ; 

			   
			    char t[1000] ;
			    char year[10] ;
			    year[0]='\0' ;
			    strftime(t, 100, "%d/%m/%Y %H:%M:%S", localtime( &file.st_mtime));
			    char d1=t[0] ;
			    char d2=t[1] ;
			    day[i][0]=d1 ;
			    day[i][1]=d2 ;
			    day[i][2]='\0' ;
			    Day[i]=atoi(day[i]) ;
			    int m1=t[3]-'0';
			    int m2=t[4]-'0' ;
			    strcpy(year,t+6) ;
			    int_year[i]=atoi(year) ;
			    time_t t1;
                struct tm*  ptm;
  				t1 = time(NULL);
  				ptm = localtime(&t1);
  				int curr_month=ptm->tm_mon+1 ;
  				int curr_year=ptm->tm_year ;
			    if(curr_year+1900>int_year[i]||(curr_month-m2-10*m1)>6)
			    	year_flag[i]=true ;
			    month[i]=months[m2+10*m1-1] ;
			    for(int j=0;j<5;j++)
			    {
			    	hour[i][j]=t[11+j] ;
			    }
			    hour[i][5]='\0' ;
			   
	        }
        }
        int max_size=INT_MIN ;
        for(int i=0;i<cnt;i++)
        {
        	if(max_size<size[i])
        		max_size=size[i] ;
        }
        int N=0 ;
        while(max_size>0)
        {
        	max_size=max_size/10 ;
        	N++ ;
        }
        if(N==0)
        	N=1 ;
        printf("total %d\n",total/2) ;
        for(int i=0;i<cnt;i++)
        {
        	if(year_flag[i]==false)
        	printf("%s %d %s %s %*d %s %2d %s %s\n",permissions[i],links[i],owner[i],group[i],N,size[i],month[i],Day[i],hour[i],list_files[i]) ;
        	else
        	printf("%s %d %s %s %*d %s %2d %5d %s\n",permissions[i],links[i],owner[i],group[i],N,size[i],month[i],Day[i],int_year[i],list_files[i]) ;	
        }
	}
	*/else
	{
		char* flags[4] ;
		int num_flags=0 ;
		char* directories[1000] ;
		int num_directories=0 ;
		char* delimiter=" " ;
		char* word ;
	    word=strtok(command+3,delimiter) ;
		bool f1=false ;
		bool f2=false ;
		bool f3=false ;
		bool f4=false ;
		while(word!=NULL)
		{
			if(strcmp(word,"-a")==0&&f1==false)
			{
				flags[num_flags++]="-a" ;
				f1=true ;
			}
			else if(strcmp(word,"-l")==0&&f2==false)
			{
				flags[num_flags++]="-l" ;
				f2=true ;
			}
			else if(strcmp(word,"-al")==0&&f3==false)
			{
				flags[num_flags++]="-al" ;
				f3=true ;
			}
			else if(strcmp(word,"-la")==0&&f4==false)
			{
				flags[num_flags++]="-la" ;
				f4=true ;
			}
			else if(strcmp(word,"-a")!=0&&strcmp(word,"-l")!=0&&strcmp(word,"-al")!=0&&strcmp(word,"-la")!=0)
			{
				directories[num_directories++]=word ;
			}
			word=strtok(NULL,delimiter) ;
		}
		if(num_flags==1||num_flags==0)
		{
			if(num_flags==0||strcmp(flags[0],"-a")==0)
			{
				for(int i=0;i<num_directories;i++)
				{
					int cnt=0 ;
					char list_files[1000][200] ;
					bool flag=true ;
					if(num_flags==0)
						flag=false ;
			    	struct dirent* dir ;
					DIR* directory=opendir(directories[i]) ;
					if(directory==NULL)
					{
						printf("Unable to open direcotry\n") ;
						perror("opendir()") ;
					}
					else
					{
						while((dir=readdir(directory))!=NULL)
						{
							if(flag||dir->d_name[0]!='.')
								strcpy(list_files[cnt++],dir->d_name) ;
						}
					}
					printf("%s\n:",directories[i]) ;
					for(int j=0;j<cnt;j++)
						printf("%s\n",list_files[j]) ;
				}
				if(num_directories==0)
				{
					bool flag=true ;
					if(num_flags==0)
						flag=false ;
					struct dirent* dir ;
					DIR* directory=opendir(".") ;
					if(directory==NULL)
					{
						printf("Unable to open direcotry\n") ;
						perror("opendir()") ;
					}
					else
					{
						while((dir=readdir(directory))!=NULL)
						{
							if(flag||dir->d_name[0]!='.')
							printf("%s\n",dir->d_name) ;
						}
					}					
				}
		    }
		    else
		    {
				for(int i=0;i<num_directories;i++)
				{
					int cnt=0 ;
					int cnt1=0 ;
					char list_files[1000][200] ;
					char file_names[1000][200] ;
					for(int j=0;j<1000;j++)
					{
						strcpy(list_files[j],directories[i]) ;
						strcat(list_files[j],"/") ;
					}
					bool flag=true ;
					if(strcmp(flags[0],"-l")==0)
						flag=false ;
			    	struct dirent* dir ;
					DIR* directory=opendir(directories[i]) ;
					if(directory==NULL)
					{
						printf("Unable to open direcotry\n") ;
						perror("opendir()") ;
					}
					else
					{
						while((dir=readdir(directory))!=NULL)
						{
							if(flag||dir->d_name[0]!='.')
							{
								strcat(list_files[cnt++],dir->d_name) ;
								strcpy(file_names[cnt1++],dir->d_name) ;
							}
						}
					}
					char permissions[cnt][1000] ;
					int links[cnt] ;
					char* owner[cnt] ;
					char* group[cnt] ;
					int size[cnt] ;
					char* month[cnt] ;
					char day[cnt][3] ;
					int Day[cnt] ;
					char hour[cnt][6] ;
					int total=0 ;				
				    for(int j=0;j<cnt;j++)
					{
						struct stat file ;
						if(stat(list_files[j],&file) < 0)    
				        {
				        	printf("Could not get file permissions\n") ;
				        	perror("stat()") ;
				        }
				        else
				        {
				        	total+=file.st_blocks ;
				          /********************************** getting permissions of files **********************************/
				        	int a=file.st_mode ;
				        	int bits[3] ;
				        	int b = 0,p = 1;
						    while (a!= 0)
						    {
						        b+=(a%8)*p;
						        a/=8;
						        p*=10;
						    }
				        	for(int j=2;j>=0;j--)
				        	{
				        		bits[j]=b%10 ;
				        		b=b/10 ;
				        	}
				        	bool check=S_ISDIR(file.st_mode) ;
				        	if(check)
				        	{
				        		permissions[j][0]='d' ;
				        		permissions[j][1]='\0' ;
				        	}
				        	else
				        	{
				        		permissions[j][0]='-' ;
				        		permissions[j][1]='\0' ;
				        	}
				        	strcat(permissions[j],permission_map[bits[0]]) ;
				        	strcat(permissions[j],permission_map[bits[1]]) ;
				        	strcat(permissions[j],permission_map[bits[2]]) ;
				        	/********************************** getting permissions of files **********************************/

				        	 links[j]=file.st_nlink ; // getting number of links of files

			                /********************************** getting owner and group names of files **********************************/
				        	 uid_t ownerid=file.st_uid ;
				        	 uid_t groupid=file.st_gid ;
						    struct passwd* ownerdet=getpwuid(ownerid) ;
						    if(ownerdet==NULL)
						    {
						        printf("Cannot retrieve user name\n") ;
						        perror("getpwuid(): ") ;
						    }
						    else
						    {
						        owner[j]=ownerdet->pw_name ;
						    }

						    struct passwd* groupdet=getpwuid(groupid) ;
						    if(groupdet==NULL)
						    {
						        printf("Cannot retrieve user name\n") ;
						        perror("getpwuid(): ") ;
						    }
						    else
						    {
						        group[j]=groupdet->pw_name ;
						    }				    	        	 
						    /********************************** getting owner and group names of files **********************************/

						    size[j]=file.st_size ; // getting size of file

						    /********************************** getting last modified time of files **********************************/
						    char t[1000] ;
						    strftime(t, 100, "%d/%m/%Y %H:%M:%S", localtime( &file.st_mtime));
						    char d1=t[0] ;
						    char d2=t[1] ;
						    day[j][0]=d1 ;
						    day[j][1]=d2 ;
						    day[j][2]='\0' ;
						    Day[j]=atoi(day[j]) ;
						    int m1=t[3]-'0';
						    int m2=t[4]-'0' ;
						    month[j]=months[m2+10*m1-1] ;
						    for(int k=0;k<5;k++)
						    {
						    	hour[j][k]=t[11+k] ;
						    }
						    hour[j][5]='\0' ;
						    /********************************** getting last modified time of files **********************************/
				        }
			        }
			        int max_size=INT_MIN ;
			        for(int j=0;j<cnt;j++)
			        {
			        	if(max_size<size[j])
			        		max_size=size[j] ;
			        }
			        int N=0 ;
			        while(max_size>0)
			        {
			        	max_size=max_size/10 ;
			        	N++ ;
			        }
			        if(N==0)
			        	N=1 ;
			        printf("total %d\n",total/2) ;
			        for(int j=0;j<cnt;j++)
			        {
			        	printf("%s %d %s %s %*d %s %2d %s %s\n",permissions[j],links[j],owner[j],group[j],N,size[j],month[j],Day[j],hour[j],file_names[j]) ;
			        }
			    }

			   if(num_directories==0)
		       {
		    	num_directories=1 ;
		    	char directories1[1][1000] ;
		    	directories1[0][0]='\0' ;
		        char path[1000] ;
    		    if(getcwd(path,1000)==NULL)
    			{
       				printf("Cannot retrieve path of current working directory\n") ;
        		    perror("getcwd(): ") ;
    			} 	
    			strcpy(directories1[0],path) ;
				for(int i=0;i<num_directories;i++)
				{
					int cnt=0 ;
					int cnt1=0 ;
					char list_files[1000][200] ;
					char file_names[1000][200] ;
					for(int j=0;j<1000;j++)
					{
						strcpy(list_files[j],directories1[i]) ;
						strcat(list_files[j],"/") ;
					}
					bool flag=true ;
					if(strcmp(flags[0],"-l")==0)
						flag=false ;
			    	struct dirent* dir ;
					DIR* directory=opendir(directories1[i]) ;
					if(directory==NULL)
					{
						printf("Unable to open direcotry\n") ;
						perror("opendir()") ;
					}
					else
					{
						while((dir=readdir(directory))!=NULL)
						{
							if(flag||dir->d_name[0]!='.')
							{
								strcat(list_files[cnt++],dir->d_name) ;
								strcpy(file_names[cnt1++],dir->d_name) ;
							}
						}
					}
					char permissions[cnt][1000] ;
					int links[cnt] ;
					char* owner[cnt] ;
					char* group[cnt] ;
					int size[cnt] ;
					char* month[cnt] ;
					char day[cnt][3] ;
					int Day[cnt] ;
					char hour[cnt][6] ;
					int total=0 ;				
				    for(int j=0;j<cnt;j++)
					{
						struct stat file ;
						if(stat(list_files[j],&file) < 0)    
				        {
				        	printf("Could not get file permissions\n") ;
				        	perror("stat()") ;
				        }
				        else
				        {
				        	total+=file.st_blocks ;
				          /********************************** getting permissions of files **********************************/
				        	int a=file.st_mode ;
				        	int bits[3] ;
				        	int b = 0,p = 1;
						    while (a!= 0)
						    {
						        b+=(a%8)*p;
						        a/=8;
						        p*=10;
						    }
				        	for(int j=2;j>=0;j--)
				        	{
				        		bits[j]=b%10 ;
				        		b=b/10 ;
				        	}
				        	bool check=S_ISDIR(file.st_mode) ;
				        	if(check)
				        	{
				        		permissions[j][0]='d' ;
				        		permissions[j][1]='\0' ;
				        	}
				        	else
				        	{
				        		permissions[j][0]='-' ;
				        		permissions[j][1]='\0' ;
				        	}
				        	strcat(permissions[j],permission_map[bits[0]]) ;
				        	strcat(permissions[j],permission_map[bits[1]]) ;
				        	strcat(permissions[j],permission_map[bits[2]]) ;
				        	/********************************** getting permissions of files **********************************/

				        	 links[j]=file.st_nlink ; // getting number of links of files

			                /********************************** getting owner and group names of files **********************************/
				        	 uid_t ownerid=file.st_uid ;
				        	 uid_t groupid=file.st_gid ;
						    struct passwd* ownerdet=getpwuid(ownerid) ;
						    if(ownerdet==NULL)
						    {
						        printf("Cannot retrieve user name\n") ;
						        perror("getpwuid(): ") ;
						    }
						    else
						    {
						        owner[j]=ownerdet->pw_name ;
						    }

						    struct passwd* groupdet=getpwuid(groupid) ;
						    if(groupdet==NULL)
						    {
						        printf("Cannot retrieve user name\n") ;
						        perror("getpwuid(): ") ;
						    }
						    else
						    {
						        group[j]=groupdet->pw_name ;
						    }				    	        	 
						    /********************************** getting owner and group names of files **********************************/

						    size[j]=file.st_size ; // getting size of file

						    /********************************** getting last modified time of files **********************************/
						    char t[1000] ;
						    strftime(t, 100, "%d/%m/%Y %H:%M:%S", localtime( &file.st_mtime));
						    char d1=t[0] ;
						    char d2=t[1] ;
						    day[j][0]=d1 ;
						    day[j][1]=d2 ;
						    day[j][2]='\0' ;
						    Day[j]=atoi(day[j]) ;
						    int m1=t[3]-'0';
						    int m2=t[4]-'0' ;
						    month[j]=months[m2+10*m1-1] ;
						    for(int k=0;k<5;k++)
						    {
						    	hour[j][k]=t[11+k] ;
						    }
						    hour[j][5]='\0' ;
						    /********************************** getting last modified time of files **********************************/
				        }
			        }
			        int max_size=INT_MIN ;
			        for(int j=0;j<cnt;j++)
			        {
			        	if(max_size<size[j])
			        		max_size=size[j] ;
			        }
			        int N=0 ;
			        while(max_size>0)
			        {
			        	max_size=max_size/10 ;
			        	N++ ;
			        }
			        if(N==0)
			        	N=1 ;
			        printf("total %d\n",total/2) ;
			        for(int j=0;j<cnt;j++)
			        {
			        	printf("%s %d %s %s %*d %s %2d %s %s\n",permissions[j],links[j],owner[j],group[j],N,size[j],month[j],Day[j],hour[j],file_names[j]) ;
			        }
			    }		    	
		    }
		    	
		    }
	    }
	    else
	    {
			for(int i=0;i<num_directories;i++)
			{
				int cnt=0 ;
				int cnt1=0 ;
				char list_files[1000][100] ;
				char file_names[1000][100] ;
				for(int j=0;j<1000;j++)
				{
					strcpy(list_files[j],directories[i]) ;
					strcat(list_files[j],"/") ;
				}
				bool flag=true ;
		    	struct dirent* dir ;
				DIR* directory=opendir(directories[i]) ;
				if(directory==NULL)
				{
					printf("Unable to open direcotry\n") ;
					perror("opendir()") ;
				}
				else
				{
					while((dir=readdir(directory))!=NULL)
					{
						if(flag||dir->d_name[0]!='.')
						{
							strcat(list_files[cnt1++],dir->d_name) ;
							strcpy(file_names[cnt++],dir->d_name) ;
						}
					}
				}
				char permissions[cnt][1000] ;
				int links[cnt] ;
				char* owner[cnt] ;
				char* group[cnt] ;
				int size[cnt] ;
				char* month[cnt] ;
				char day[cnt][3] ;
				int Day[cnt] ;
				char hour[cnt][6] ;
				int total=0 ;				
			    for(int j=0;j<cnt;j++)
				{
					struct stat file ;
					if(stat(list_files[j],&file) < 0)    
			        {
			        	printf("Could not get file permissions\n") ;
			        	perror("stat()") ;
			        }
			        else
			        {
			        	total+=file.st_blocks ;
			          /********************************** getting permissions of files **********************************/
			        	int a=file.st_mode ;
			        	int bits[3] ;
			        	int b = 0,p = 1;
					    while (a!= 0)
					    {
					        b+=(a%8)*p;
					        a/=8;
					        p*=10;
					    }
			        	for(int j=2;j>=0;j--)
			        	{
			        		bits[j]=b%10 ;
			        		b=b/10 ;
			        	}
			        	bool check=S_ISDIR(file.st_mode) ;
			        	if(check)
			        	{
			        		permissions[j][0]='d' ;
			        		permissions[j][1]='\0' ;
			        	}
			        	else
			        	{
			        		permissions[j][0]='-' ;
			        		permissions[j][1]='\0' ;
			        	}
			        	strcat(permissions[j],permission_map[bits[0]]) ;
			        	strcat(permissions[j],permission_map[bits[1]]) ;
			        	strcat(permissions[j],permission_map[bits[2]]) ;
			        	/********************************** getting permissions of files **********************************/

			        	 links[j]=file.st_nlink ; // getting number of links of files

		                /********************************** getting owner and group names of files **********************************/
			        	 uid_t ownerid=file.st_uid ;
			        	 uid_t groupid=file.st_gid ;
					    struct passwd* ownerdet=getpwuid(ownerid) ;
					    if(ownerdet==NULL)
					    {
					        printf("Cannot retrieve user name\n") ;
					        perror("getpwuid(): ") ;
					    }
					    else
					    {
					        owner[j]=ownerdet->pw_name ;
					    }

					    struct passwd* groupdet=getpwuid(groupid) ;
					    if(groupdet==NULL)
					    {
					        printf("Cannot retrieve user name\n") ;
					        perror("getpwuid(): ") ;
					    }
					    else
					    {
					        group[j]=groupdet->pw_name ;
					    }				    	        	 
					    /********************************** getting owner and group names of files **********************************/

					    size[j]=file.st_size ; // getting size of file

					    /********************************** getting last modified time of files **********************************/
					    char t[1000] ;
					    strftime(t, 100, "%d/%m/%Y %H:%M:%S", localtime( &file.st_mtime));
					    char d1=t[0] ;
					    char d2=t[1] ;
					    day[j][0]=d1 ;
					    day[j][1]=d2 ;
					    day[j][2]='\0' ;
					    Day[j]=atoi(day[j]) ;
					    int m1=t[3]-'0';
					    int m2=t[4]-'0' ;
					    month[j]=months[m2+10*m1-1] ;
					    for(int k=0;k<5;k++)
					    {
					    	hour[j][k]=t[11+k] ;
					    }
					    hour[j][5]='\0' ;
					    /********************************** getting last modified time of files **********************************/
			        }
		        }
		        int max_size=INT_MIN ;
		        for(int j=0;j<cnt;j++)
		        {
		        	if(max_size<size[j])
		        		max_size=size[j] ;
		        }
		        int N=0 ;
		        while(max_size>0)
		        {
		        	max_size=max_size/10 ;
		        	N++ ;
		        }
		        if(N==0)
		        	N=1 ;
		        printf("total %d\n",total/2) ;
		        for(int j=0;j<cnt;j++)
		        {
		        	printf("%s %d %s %s %*d %s %2d %s %s\n",permissions[j],links[j],owner[j],group[j],N,size[j],month[j],Day[j],hour[j],file_names[j]) ;
		        }

	         }
            if(num_directories==0)
		    {
		    	num_directories=1 ;
		    	char directories1[1][1000] ;
		    	directories1[0][0]='\0' ;
		        char path[1000] ;
    		    if(getcwd(path,1000)==NULL)
    			{
       				printf("Cannot retrieve path of current working directory\n") ;
        		    perror("getcwd(): ") ;
    			} 	
    			strcpy(directories1[0],path) ;
				for(int i=0;i<num_directories;i++)
				{
					int cnt=0 ;
					int cnt1=0 ;
					char list_files[1000][200] ;
					char file_names[1000][200] ;
					for(int j=0;j<1000;j++)
					{
						strcpy(list_files[j],directories1[i]) ;
						strcat(list_files[j],"/") ;
					}
					bool flag=true ;
			    	struct dirent* dir ;
					DIR* directory=opendir(directories1[i]) ;
					if(directory==NULL)
					{
						printf("Unable to open direcotry\n") ;
						perror("opendir()") ;
					}
					else
					{
						while((dir=readdir(directory))!=NULL)
						{
							if(flag||dir->d_name[0]!='.')
							{
								strcat(list_files[cnt++],dir->d_name) ;
								strcpy(file_names[cnt1++],dir->d_name) ;
							}
						}
					}
					char permissions[cnt][1000] ;
					int links[cnt] ;
					char* owner[cnt] ;
					char* group[cnt] ;
					int size[cnt] ;
					char* month[cnt] ;
					char day[cnt][3] ;
					int Day[cnt] ;
					char hour[cnt][6] ;
					int total=0 ;				
				    for(int j=0;j<cnt;j++)
					{
						struct stat file ;
						if(stat(list_files[j],&file) < 0)    
				        {
				        	printf("Could not get file permissions\n") ;
				        	perror("stat()") ;
				        }
				        else
				        {
				        	total+=file.st_blocks ;
				          /********************************** getting permissions of files **********************************/
				        	int a=file.st_mode ;
				        	int bits[3] ;
				        	int b = 0,p = 1;
						    while (a!= 0)
						    {
						        b+=(a%8)*p;
						        a/=8;
						        p*=10;
						    }
				        	for(int j=2;j>=0;j--)
				        	{
				        		bits[j]=b%10 ;
				        		b=b/10 ;
				        	}
				        	bool check=S_ISDIR(file.st_mode) ;
				        	if(check)
				        	{
				        		permissions[j][0]='d' ;
				        		permissions[j][1]='\0' ;
				        	}
				        	else
				        	{
				        		permissions[j][0]='-' ;
				        		permissions[j][1]='\0' ;
				        	}
				        	strcat(permissions[j],permission_map[bits[0]]) ;
				        	strcat(permissions[j],permission_map[bits[1]]) ;
				        	strcat(permissions[j],permission_map[bits[2]]) ;
				        	/********************************** getting permissions of files **********************************/

				        	 links[j]=file.st_nlink ; // getting number of links of files

			                /********************************** getting owner and group names of files **********************************/
				        	 uid_t ownerid=file.st_uid ;
				        	 uid_t groupid=file.st_gid ;
						    struct passwd* ownerdet=getpwuid(ownerid) ;
						    if(ownerdet==NULL)
						    {
						        printf("Cannot retrieve user name\n") ;
						        perror("getpwuid(): ") ;
						    }
						    else
						    {
						        owner[j]=ownerdet->pw_name ;
						    }

						    struct passwd* groupdet=getpwuid(groupid) ;
						    if(groupdet==NULL)
						    {
						        printf("Cannot retrieve user name\n") ;
						        perror("getpwuid(): ") ;
						    }
						    else
						    {
						        group[j]=groupdet->pw_name ;
						    }				    	        	 
						    /********************************** getting owner and group names of files **********************************/

						    size[j]=file.st_size ; // getting size of file

						    /********************************** getting last modified time of files **********************************/
						    char t[1000] ;
						    strftime(t, 100, "%d/%m/%Y %H:%M:%S", localtime( &file.st_mtime));
						    char d1=t[0] ;
						    char d2=t[1] ;
						    day[j][0]=d1 ;
						    day[j][1]=d2 ;
						    day[j][2]='\0' ;
						    Day[j]=atoi(day[j]) ;
						    int m1=t[3]-'0';
						    int m2=t[4]-'0' ;
						    month[j]=months[m2+10*m1-1] ;
						    for(int k=0;k<5;k++)
						    {
						    	hour[j][k]=t[11+k] ;
						    }
						    hour[j][5]='\0' ;
						    /********************************** getting last modified time of files **********************************/
				        }
			        }
			        int max_size=INT_MIN ;
			        for(int j=0;j<cnt;j++)
			        {
			        	if(max_size<size[j])
			        		max_size=size[j] ;
			        }
			        int N=0 ;
			        while(max_size>0)
			        {
			        	max_size=max_size/10 ;
			        	N++ ;
			        }
			        if(N==0)
			        	N=1 ;
			        printf("total %d\n",total/2) ;
			        for(int j=0;j<cnt;j++)
			        {
			        	printf("%s %d %s %s %*d %s %2d %s %s\n",permissions[j],links[j],owner[j],group[j],N,size[j],month[j],Day[j],hour[j],file_names[j]) ;
			        }
			    }		    		         
	    }
    }
}
}