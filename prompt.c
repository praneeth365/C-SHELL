
void prompt()
{
    char* username=(char*)malloc(1000) ;
    char sysname[1000] ;
    sysname[0]='\0' ;
    char* path=(char*)malloc(1000) ;
    int hostname = gethostname(sysname,1000);
    if(hostname==-1)
    {
        printf("Cannot retrieve host name\n") ;
        perror("gethostname()") ;
    }
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
    if(getcwd(path,1000)==NULL)
    {
        printf("Cannot retrieve path of current working directory\n") ;
        perror("getcwd(): ") ;
    }
    char relpath[1000] ;
    int n=strlen(root) ;
    bool flag=true ;
    if(strlen(path)>=strlen(root))
    {
        for(int i=0;i<n;i++)
        {
            if(path[i]!=root[i])
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
    bool dir=true ;
    if(strlen(path)==strlen(root)&&strcmp(path,root)==0)
    {
        dir=false ;
    }
    if(dir==true&&n==0)
    printf("<%s@%s:%s>",username,sysname,path+n) ;
    else if(dir==true&&n!=0)
    printf("<%s@%s:~%s>",username,sysname,path+n) ;    
    else
    printf("<%s@%s:~>",username,sysname) ;    
}
