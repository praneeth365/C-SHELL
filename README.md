C-SHELL(Assignment-3)

This is the linux shell implemented in C

Running the Shell(command):
make
./a.out

Exiting from the shell(command):
exit

Assumptions:
Taking input:
In the input various commands are given seperated by  ';' note that there should be space before and after semicolon i.e ls -l;cd dir is not allowed 

Constraints on file paths and no of files in a directory:
Note that the length of the file name cannot exceed 100 characters and no of files in a directory cannot exceed 100. Since the sizes of arrays I used are 100 for file lengths and no of files in a directory is 100.

Background process:
Note that we cannot run more than 1000 background process "simultaneously" we can run as many process one after another but only 1000 background process should be running simultaneously because to retreive the name of the background process after completion of process I used array of length 1000.

When the file is 6 months old in ls I will check if the year differnce is greater than 1 or if the year is same then I will check the months difference if it is greater than 6 then I will print the year else I will print the hours instead of year.

Files:
main.c:
This file contains all the gloabal variables and it reads the input and tokenises them and calls the specific function accordingly.

prompt.c:
This file gets username and host name and prints the current working directory(as mentioned in the assignment doc).

Headers.h:
This file will contain all the headers required for the code to run.

background_process.c:
This file contains code required to run the background process when the & command is given.

pwd.c:
This file gets the current working directory when that command is given.

foreground_process.c:
This file will contain the code to run a foreground process when that command is given

ls.c:
This function will contain the code to run ls command(with flags) is given

finished_background_process.c:
This function will contain signal handler which will print background process completed with pid and name when background process completes.

pinfo.c
This function will contain the implementation of the pinfo which will return the process ID and status and memory and executable path for a given pid or the current process.

cd.c:
This function will contain the implementation of cd command(with flags).

echo.c:
This function will contain the implementation of echo command.

jobs.c:
This file contains implementations of jobs_sort() which will sort the jobs in alphabetical order and maintains the order of jobs and jobs() which will print the jobs and it also contains fg() and bg() implementations.

piping.c:
This file contains implementation of piping(along with redirection)

signals.c:
This file contains implementation of all signals Ctrl+C,Ctrl+Z,Ctrl+D.
