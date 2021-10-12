void piping(char command[])
{
    int num_pipes = 0;
    for (int j = 0; j < strlen(command); j++)
    {
        if (command[j] == '|')
            num_pipes++;
    }
    char sub_command[num_pipes + 1][1000];
    for (int j = 0; j <=num_pipes; j++)
    {
        sub_command[j][0] = '\0';
    }
    char delimiter[] = "|";
    char *temp = strtok(command, delimiter);
    int p = 0;
    while (temp)
    {
        strcpy(sub_command[p++], temp);
        temp = strtok(NULL, delimiter);
    }
    sub_command[0][strlen(sub_command[0]) - 1] = '\0';
    for (int j = 1; j <= num_pipes; j++)
    {
        char temp[1000] ;
        temp[0]='\0' ;
        strcpy(temp, sub_command[j] + 1);
        strcpy(sub_command[j],temp) ;
        if(j!=num_pipes)
        sub_command[j][strlen(sub_command[j]) - 1] = '\0';
    }
    int n=num_pipes+1 ;
    int fd[n][2] ;
    for(int i=0;i<n;i++)
    {
        pipe(fd[i]) ;
        int input_fd ;
        int output_fd ;
        int num_arguments=0 ;
        char* arguments[1000] ;
        char* word=strtok(sub_command[i]," ") ;
        while(word)
        {
            arguments[num_arguments++]=word ;
            word=strtok(NULL," ") ;
        }
        int left_ind=-1 ;
        int right_ind=-1 ;
        for(int j=0;j<num_arguments;j++)
        {
            if(strcmp(arguments[j],"<")==0)
            {
                left_ind=j ;
            }
            else if(strcmp(arguments[j],">")==0||strcmp(arguments[j],">>")==0)
            {
                right_ind=j ;
            }
        }
        char actual_command[1000] ;
        actual_command[0]='\0' ;
        if (left_ind != -1)
        {
            for (int j = 0; j < left_ind; j++)
            {
                strcat(actual_command, arguments[j]);
                if (j != left_ind - 1)
                    strcat(actual_command, " ");
            }
        }
        else if (right_ind != -1)
        {
            for (int j = 0; j < right_ind; j++)
            {
                strcat(actual_command, arguments[j]);
                if (j != right_ind - 1)
                    strcat(actual_command, " ");
            }
        }
        else
        {
            for(int j=0;j<num_arguments;j++)
            {
                strcat(actual_command,arguments[j]) ;
                if(j!=num_arguments-1)
                {
                    strcat(actual_command," ") ;
                }
            }
        }
        if(left_ind!=-1)
        {
            input_fd=open(arguments[left_ind+1], O_RDONLY) ;
            if(input_fd<0)
            {
                perror("open()") ;
                return ;
            }
        }
        else if(i!=0)
        {
            input_fd=fd[i-1][0] ;
        }
        if(right_ind!=-1)
        {
            if(strcmp(arguments[right_ind],">")==0)
                output_fd=open(arguments[right_ind+1], O_WRONLY | O_CREAT | O_TRUNC, 0644) ;
            else
            {
                output_fd=open(arguments[right_ind+1], O_WRONLY | O_CREAT | O_APPEND, 0644) ;    
            }
            if(output_fd<0)
            {
                perror("open()") ;
                return ;
            }
        }
        else if(i!=n-1)
        {
            output_fd=fd[i][1] ;
        } 
        actual_stdin=dup(0) ;
        actual_stdout=dup(1) ;
        int x=fork() ;
        if(x<0)
        {
            perror("fork()") ;
            return ;
        }
        else if(x==0)
        {
           if(i!=0||left_ind!=-1)
            {
                int a=dup2(input_fd,0) ;
                if(a<0)
                {
                    perror("dup2()") ;
                    return ;
                }
                else
                {
                    close(input_fd) ;
                }
            }
            if(i!=n-1||right_ind!=-1)
            {
                int b=dup2(output_fd,1) ;
                if(b<0)
                {
                    perror("dup2()") ;
                    return ;
                }
            }
            if(strcmp(arguments[0],"echo")==0)
            {
                echo(actual_command) ;
            }
            else if(strcmp(arguments[0],"cd")==0)
            {
                cd(actual_command) ;
            }
            else if(strcmp(arguments[0],"ls")==0)
            {
                ls(actual_command) ;
            }
            else if(strcmp(arguments[0],"pwd")==0)
            {
                pwd(actual_command) ;
            }
            else if(strcmp(arguments[0],"pinfo")==0)
            {
                pinfo(actual_command) ;
            }
            else
            {
                fgp(actual_command) ;
            }
            exit(0) ;
        }
        else
        {
            int status ;
            waitpid(x,&status,0) ;
            close(output_fd) ;
            int x=dup2(actual_stdin,0) ;
            int y=dup2(actual_stdout,1) ;
        }
    }
}
