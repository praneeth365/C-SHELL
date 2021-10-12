void ctrlc(int signum)
{
    pid_t pid = getpid() ;
    if(pid<0)
    perror("pid()") ;

    if(pid!=shell_id)
    {
        return ;
    }
    else
    {
        if(curr_pid!=-1)
        {
            int k =kill(curr_pid,signum) ;
            if(k<0)
            {
                perror("kill()") ;
                return ;
            }
        }
        else
        {
            printf("\n") ;
            printf(GRN) ;
            prompt() ;
            printf(WHT) ;
            fflush(stdout) ;
        }
    }
}

void ctrld(int signum)
{
    exit(0) ;
}

void ctrlz(int signum)
{
    printf("\n") ;
    return ; // This function is just to catch the signal actual implementation is done in foregroundprocess() function
}
