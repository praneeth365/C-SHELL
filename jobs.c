
void swap(struct process* x, struct process* y)
{
    struct process temp=*x ;
    *x=*y ;
    *y=temp ;
}

void jobs_sort(void)
{
    int n=1000 ;
    int k=1 ;
    for(int i=0;i<n-1;i++)
    {
        for(int j=0;j<n-i-1;j++)
        {
            if(A[j].job_order>A[j+1].job_order)
            {
                swap(&A[j],&A[j+1]) ;
            }
        }
    }

    for(int i=0;i<n;i++)
    {
        if(A[i].flag==true)
        {
            A[i].job_order=k ;
            k++ ;
        }
    }

    for(int i=0;i<n-1;i++)
    {
        for(int j=0;j<n-i-1;j++)
        {
            if(strcmp(A[j].name,A[j+1].name)>0)
            {
                swap(&A[j],&A[j+1]) ;
            }
        }
    }
}


void jobs(char command[])
{
    bool r_flag=false ;
    bool s_flag=false ;
    int num=1 ;
    jobs_sort() ;
    if(strstr(command,"-r"))
    {
        r_flag=true ;
    }
    if(strstr(command,"-s"))
    {
        s_flag=true ;
    }
    char file[1000] ;
    file[0]='\0' ;
    char status[1000] ;
    status[0]='\0' ;
    for(int i=0;i<1000;i++)
    {
        if(A[i].flag==false)
        {
            continue ;
        }
        sprintf(file,"/proc/%d/stat",A[i].pid) ;
        FILE* fp =fopen(file,"r") ;
        if(fp==NULL)
        {
            perror("fopen()") ;
        }
        else
        {
            char buf[1000] ;
            buf[0]='\0' ;
            fgets(buf,1000,fp) ;
            fclose(fp) ;
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
            if(strcmp(status,"T")==0)
            {
                strcpy(status,"Stopped") ;
            }
            else
            {
                strcpy(status,"Running") ;
            }
            if(!r_flag&&!s_flag)
            printf("[%d] %s %s [%d]\n",A[i].job_order,status,A[i].name,A[i].pid) ;
            else if(r_flag&&strcmp(status,"Running")==0)
            {
                printf("[%d] %s %s [%d]\n",A[i].job_order,status,A[i].name,A[i].pid) ;
            }
            else if(s_flag&&strcmp(status,"Stopped")==0)
            {
                printf("[%d] %s %s [%d]\n",A[i].job_order,status,A[i].name,A[i].pid) ;
            }
        }
    }
}

void fg(char command[])
{
    jobs_sort() ;
    char str_job_num[10] ;
    str_job_num[0]='\0' ;
    strcpy(str_job_num,command+3) ;
    int job_num=atoi(str_job_num) ;
    int pid ;
    char name[1000] ;
    name[0]='\0' ;
    int job_ind=-1 ;
    for(int i=0;i<1000;i++)
    {
        if(A[i].flag==true)
        {
            if(A[i].job_order==job_num)
            {
                pid=A[i].pid ;
                strcpy(name,A[i].name) ;
                job_ind=i ;
                break ;
            }
        }
    }
    if(job_ind==-1)
    {
        printf("INVALID JOB NUMBER\n") ;
        return ;
    }
    int status ;
    signal(SIGTTIN,SIG_IGN) ;
    signal(SIGTTOU,SIG_IGN) ;
    tcsetpgrp(STDIN_FILENO,pid) ;
    kill(pid,SIGCONT) ;
    tcsetpgrp(STDIN_FILENO,getpgrp()) ;
    signal(SIGTTIN,SIG_DFL) ;
    signal(SIGTTOU,SIG_DFL) ;
    waitpid(pid,&status,WUNTRACED) ;    
    A[job_ind].flag=false ; 
    if(WIFSTOPPED(status))
    {
        printf("%s with PID %d suspended\n", name, pid);
        A[job_ind].flag=true ;
    }       
    return ;
}

void bg(char command[])
{
    jobs_sort() ;
    char str_job_num[10] ;
    str_job_num[0]='\0' ;
    strcpy(str_job_num,command+3) ;
    int job_num=atoi(str_job_num) ;
    int pid ;
    char name[1000] ;
    name[0]='\0' ;
    int job_ind=-1 ;
    for(int i=0;i<1000;i++)
    {
        if(A[i].flag==true)
        {
            if(A[i].job_order==job_num)
            {
                pid=A[i].pid ;
                strcpy(name,A[i].name) ;
                job_ind=i ;
                break ;
            }
        }
    }
    if(job_ind==-1)
    {
        printf("INVALID JOB NUMBER\n") ;
        return ;
    }
    kill(A[job_ind].pid,SIGCONT) ;
}

void sig(char command[])
{
    char* arguments[3] ;
    char* word=strtok(command," ") ;
    int arg_num=0 ;
    while(word)
    {
        arguments[arg_num++]=word ;
        word=strtok(NULL," ") ;
    }
    int job=atoi(arguments[1]) ;
    int signal_no=atoi(arguments[2]) ;
    jobs_sort() ;
    int id =-1 ;
    for(int i=0;i<1000;i++)
    {
        if(A[i].flag==true)
        {
            if(A[i].job_order==job)
            {
                id=A[i].pid ;
                break ;
            }
        }
    }
    if(id==-1)
    {
        printf("INVALID JOB ID\n") ;
        return ;
    }
    kill(id,signal_no) ;
}



