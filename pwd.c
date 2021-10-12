void pwd()
{
    char path[1000] ;
    if(getcwd(path,1000)==NULL)
    {
        printf("Cannot retrieve path of current working directory\n") ;
        perror("getcwd(): ") ;
    }
    else
    {
        printf("%s\n",path) ;
    }
}