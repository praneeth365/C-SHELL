#include "headers.h"

#define BLK "\e[0;30m"
#define GRN "\e[0;32m"
#define WHT "\e[0;37m"
char root[1000];
int num1 = 0;
int num2 = 0;
char *child[1000];
int shell_id;
int id[1000];
int repeat_flag = 0;
char repeat_command[1000];
char *bg_commands[1000];
int actual_stdin;
int actual_stdout;
int curr_pid ;
int job_num ;
struct process
{
    int pid;
    char name[1000];
    int job_order ;
    bool flag;
};
char history[20][1000];
int history_cnt = 0;
struct process A[1000];
char lastwd[2][200];
int num_cd = 0;

#define GRN "\e[0;32m"
#define BLU "\e[0;34m"
#include "prompt.c"
#include "echo.c"
#include "pwd.c"
#include "cd.c"
#include "ls.c"
#include "finished_background_process.c"
#include "background_process.c"
#include "foreground_process.c"
#include "pinfo.c"
#include "signals.c"
#include "piping.c"
#include "jobs.c"

void execute_command(char command[], int commands[], char *arguments[], int i, int num_commands, int num_arguments)
{
    actual_stdin=dup(0) ;
    actual_stdout=dup(1) ;
    int last;
    char input_file[1000];
    char output_file[1000];
    char actual_command[1000];
    actual_command[0] = '\0';
    input_file[0] = '\0';
    output_file[0] = '\0';
    if (i != num_commands - 1)
    {
        last = commands[i + 1] - 1;
    }
    else
    {
        last = num_arguments - 1;
    }
    int left_ind = -1;
    int right_ind = -1;
    for (int j = commands[i]; j < last; j++)
    {
        if (strcmp(arguments[j], "<") == 0)
        {
            left_ind = j;
        }
        if (strcmp(arguments[j], ">") == 0||strcmp(arguments[j],">>")==0)
        {
            right_ind = j;
        }
    }
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
        strcpy(actual_command, command);
    }
    int input_fd, output_fd;
    if (left_ind != -1)
    {
        input_fd = open(arguments[left_ind + 1], O_RDONLY);
        if (input_fd < 0)
        {
            perror("open()");
            return;
        }
    }
    if (right_ind != -1)
    {
        if(strcmp(arguments[right_ind],">")==0)
            output_fd = open(arguments[right_ind + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else
            output_fd = open(arguments[right_ind + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (output_fd < 0)
        {
            perror("open()");
            return;
        }
        
    }
    if (left_ind != -1)
    {
        int k = dup2(input_fd, 0);
        close(input_fd);
        if (k < 0)
        {
            perror("dup2()");
        }
    }
    if (right_ind != -1)
    {
        int k=dup2(output_fd,1) ;
        close(output_fd) ;
        if(k<0)
        {
            perror("dup2()") ;
        }
    }
    if (strcmp(arguments[commands[i]], "echo") == 0)
    {
        echo(actual_command);
    }
    else if (strcmp(arguments[commands[i]], "cd") == 0)
    {
        cd(actual_command);
    }
    else if (strcmp(arguments[commands[i]], "ls") == 0)
    {
        ls(actual_command);
    }
    else if (strcmp(arguments[commands[i]], "pwd") == 0)
    {
        pwd(actual_command);
    }
    else if (strcmp(arguments[commands[i]], "pinfo") == 0)
    {
        pinfo(actual_command);
    }
    else if (strstr(command, "&"))
    {
        bgp(actual_command);
    }
    else if(strcmp(arguments[commands[i]],"fg")==0)
    {
        fg(actual_command) ;
    }
    else if(strcmp(arguments[commands[i]],"bg")==0)
    {
        bg(actual_command) ;
    }
    else if(strcmp(arguments[commands[i]],"sig")==0)
    {
        sig(actual_command) ;
    }
    else if(strcmp(arguments[commands[i]],"jobs")==0)
    {
        jobs(actual_command) ;
    }
    else if (strcmp(arguments[commands[i]], "repeat") == 0)
    {
        int x = commands[i];
        repeat_flag = atoi(arguments[x + 1]);
        int size = 8 + strlen(arguments[x + 1]);
        strcpy(repeat_command, command + size);
    }
    else if(strcmp(arguments[commands[i]],"exit")==0)
    {
        exit(0) ;
    }
    else
    {
        fgp(actual_command);
    }

    if (repeat_flag > 0)
    {
        char *list_command[1000];
        int num_args = 0;
        char *token = strtok(repeat_command, "\t ");
        while (token)
        {
            list_command[num_args++] = token;
            token = strtok(NULL, "\t ");
        }
        char temp_command[1000];
        temp_command[0] = '\0';
        for (int j = 0; j < num_args; j++)
        {
            strcat(temp_command, list_command[j]);
            if (j != num_args - 1)
                strcat(temp_command, " ");
        }

        for (int j = 0; j < repeat_flag; j++)
        {
            if (strcmp(list_command[0], "echo") == 0)
            {
                echo(temp_command);
            }
            else if (strcmp(list_command[0], "cd") == 0)
            {
                cd(temp_command);
            }
            else if (strcmp(list_command[0], "ls") == 0)
            {
                ls(temp_command);
            }
            else if (strcmp(list_command[0], "pwd") == 0)
            {
                pwd(temp_command);
            }
            else if (strcmp(list_command[0], "pinfo") == 0)
            {
                pinfo(temp_command);
            }
            else if(strcmp(list_command[0],"jobs")==0)
            {
                jobs(temp_command) ;
            }
            else if(strcmp(list_command[0],"fg")==0)
            {
                fg(temp_command) ;
            }
            else if(strcmp(list_command[0],"bg")==0)
            {
                bg(temp_command) ;
            }
            else if(strcmp(list_command[0],"sig")==0)
            {
                sig(temp_command) ;
            }
            else
            {
                fgp(temp_command);
            }
        }
    }
    repeat_flag = 0;
    int p = dup2(actual_stdout,1) ;
    int q = dup2(actual_stdin,0) ;
    if(p<0 || q<0)
    {
        printf("ERROR\n") ;
    }
}

int main(void)
{
    signal(SIGCHLD, sig_handler);
    signal(SIGINT, ctrlc) ;
    signal(SIGTSTP,ctrlz) ;
    actual_stdin=dup(0) ;
    actual_stdout=dup(1) ;
    curr_pid=-1 ;
    job_num=0 ;
    pid_t p_id;
    p_id = getpid();
    shell_id = p_id;    
    for (int i = 0; i < 2; i++)
    {
        lastwd[i][0] = '\0';
    }
    for (int i = 0; i < 1000; i++)
    {
        A[i].flag = false;
        A[i].name[0] = '\0';
        A[i].job_order=-1 ;
    }
    repeat_command[0] = '\0';
    printf("--------------------------------------------------------------WELCOME TO MY SHELL-------------------------------------------------------------\n");
    if (getcwd(root, 1000) == NULL)
    {
        printf("Unable to retrieve current working directory\n");
        perror("getcwd()");
    }
    bool finish_flag = true;
    while (1)
    {
        /**************************** reading commands ****************************/
        char command[1000];
        command[0] = '\0';
        curr_pid=-1 ;
        printf(GRN) ;
        prompt();
        printf(WHT) ;
        gets(command);
        if(feof(stdin))
        {
            printf("\n") ;
            exit(0) ;
        }
        char *delimiter = " \t";
        char *word = strtok(command, delimiter);
        char *arguments[1000];
        int num_arguments = 0;
        int num_commands = 0;
        int commands[1000];
        while (word)
        {
            if (num_arguments == 0 && word[0] != ';')
            {
                commands[num_commands] = 0;
                num_commands++;
            }

            if (word[0] == ';')
            {
                commands[num_commands] = num_arguments;
                num_commands++;
                word = strtok(NULL, delimiter);
                continue;
            }
            arguments[num_arguments] = word;
            num_arguments++;
            word = strtok(NULL, delimiter);
        }
        char command_arr[num_commands][1000];
        for (int i = 0; i < num_commands; i++)
        {
            command_arr[i][0] = '\0';
        }
        for (int i = 0; i < num_commands; i++)
        {
            if (i != num_commands - 1)
            {
                for (int j = commands[i]; j < commands[i + 1]; j++)
                {
                    strcat(command_arr[i], arguments[j]);
                    if (j != commands[i + 1] - 1)
                        strcat(command_arr[i], " ");
                }
            }
            else
            {
                for (int j = commands[i]; j < num_arguments; j++)
                {
                    strcat(command_arr[i], arguments[j]);
                    if (j != num_arguments - 1)
                        strcat(command_arr[i], " ");
                }
            }
        }

        /**************************** reading commands ****************************/

        /**************************** executing commands **************************/

        for (int i = 0; i < num_commands; i++)
        {
            if(strstr(command_arr[i],"|"))
            {
                piping(command_arr[i]) ;
            }
            else
            {
                execute_command(command_arr[i], commands, arguments, i, num_commands, num_arguments);
            }
        }
    }
}