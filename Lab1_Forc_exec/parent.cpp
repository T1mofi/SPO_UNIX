#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "ncurses.h"
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

int Print = 0;
int Close = 1;
int flag = 0;

struct sigaction printSignal,
closeSignal;

void setPrint(int sign)
{
    Print = 1;
}

void setClose(int sign)
{
    Close = 1;
}

void PrintString()
{
    char string[20] = {"Dark spruce forest"};
    Close = 0;
    while(!Close)
    {
        usleep(10000);
        if(Print)
        {
            for(int i = 0; i < strlen(string); i++)
            {
                if(Close)
                {
                    Print = 0;
                    kill(getppid(), SIGUSR2);
                    return ;
                }
                printw("%c", string[i]);
                refresh();
                usleep(100000);
            }
            Print = 0;
            kill(getppid(), SIGUSR2);
        }
    }
    return;
}

void Work_with_Process(pid_t pid)
{
    char key;
    while(true)
    {
        halfdelay(1);
        key = (char)getchar();
        if (key == 'p')
        {
            if (flag == 0)
            {
                kill(pid,SIGUSR1);
                flag = 1;
                Close = 0;
            }
        }
        if (key == 'q')
        {
            kill(pid,SIGUSR2);
            waitpid(pid,0,0);
            break;
        }
        if (Close == 1)
        {
            flag = 0;
        }
    }
}

int main()
{
    initscr();
    clear();
    noecho();
    refresh();
    
    printSignal.sa_handler = setPrint;
    sigaction(SIGUSR1,&printSignal,NULL);
    
    closeSignal.sa_handler = setClose;
    sigaction(SIGUSR2, &closeSignal,NULL);
    
    pid_t pid = fork();
    
    switch (pid)
    {
        case -1:
            printw("Error");
            return 0;
        case 0:
            PrintString();
            break;
        default:
            Work_with_Process(pid);
            break;
    }
    clear();
    endwin();
    return 0;
}
