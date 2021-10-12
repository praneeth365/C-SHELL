void cd(char* command)
{
    if(strcmp(command+3,"-")!=0&&strcmp(command+3,"~")!=0)
    {
        char* execute_command=command+3 ;
        int x=chdir(execute_command) ;
        if(x!=0)
        {
            printf("Unable to change Directory\n") ;
            perror("chdir()") ;
        }        
        if(num_cd>=2)
        {
            strcpy(lastwd[0],lastwd[1]) ;
            getcwd(lastwd[1],200) ;
            if(lastwd[1]==NULL)
            {
                printf("Unable to retrieve current working directory\n") ;
                perror("getcwd()") ;                
            }
        }
        else if(num_cd==0)
        {
            getcwd(lastwd[0],200) ;
            if(lastwd[0]==NULL)
            {
                printf("Unable to retrieve current working directory\n") ;
                perror("getcwd()") ;                
            }            
        }
        else
        {
            getcwd(lastwd[1],200) ;
            if(lastwd[1]==NULL)
            {
                printf("Unable to retrieve current working directory\n") ;
                perror("getcwd()") ;                
            }             
        }
        num_cd++ ;
    }
    else if(strcmp(command+3,"-")==0)
    {
        if(num_cd<2)
        {
            printf("OLDPWD not set\n") ;
            return ;
        }
        int x=chdir(lastwd[0]) ;
        if(x!=0)
        {
            printf("Unable to change Directory\n") ;
            perror("chdir()") ;
        }
        char temp[200] ;
        temp[0]='\0' ;
        strcpy(temp,lastwd[0]) ;
        strcpy(lastwd[0],lastwd[1]) ;
        strcpy(lastwd[1],temp) ;
        num_cd++ ;
    }
    else if(strcmp(command+3,"~")==0)
    {
       int x=chdir(root) ;
       if(x!=0)
       {
            printf("Unable to change directory\n") ;
            perror("chdir()") ;
       }
        if(num_cd>=2)
        {
            strcpy(lastwd[0],lastwd[1]) ;
            strcpy(lastwd[1],root) ;
        }
        else if(num_cd==0)
        {
            strcpy(lastwd[0],root) ;         
        }
        else
        {
            strcpy(lastwd[1],root) ;          
        }
        num_cd++ ;       
    }
}