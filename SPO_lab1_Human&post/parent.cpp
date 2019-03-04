#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"

using namespace std;

int postRequest(char* departureType, char* money);
char* decryptExitCode(int exitCode);

int main(int argc, const char * argv[]) {
    
    initscr();
    curs_set(0);
    
    while (true)
    {
        clear();
        refresh();
        move(1,0);
        printw("\rPlease, select an action:\n");
        printw("\r1) Send a letter(10 BYN).\n");
        printw("\r2) Send a parcel(50 BYN).\n");
        printw("\r3) End work.\n\r");
        printw("\rYour choice: ");
        
        int choice = 0;
        char* departureType;
        scanw("%d", &choice);

        switch (choice)
        {
            case 1: {
                departureType = "letter";
                break;
            }
            case 2: {
                departureType = "parcel";
                break;
            }
            case 3: {
                printw("\rGoodbye!");
                endwin();
                return 0;
            }
            default: {
                printw("\rYour choice is incorrect.");
                endwin();
                return 0;
            }
        }

        printw("\rPlease, deposit money: ");
        char money[10];
        scanw("%s", &money);

        int exitCode = postRequest(departureType, money);
        printw("\r%s", decryptExitCode(exitCode));
        
        char c = 0;
        while(c != 'q')
        {
            halfdelay(2);
            c = getch();
            
            refresh();
        }
    }
    
    endwin();
    return 0;
}

int postRequest(char* departureType, char* money) {
    
    pid_t pid = fork();
    time_t ltime;
    int status = 0;
    
    switch (pid) {
        case -1: {
            printw("\rFork error.");
            break;
        }
            
        case 0: {
            char* const argin[] = {"Post", money, departureType, NULL};
            execvp("./Post", argin);
            perror("cannot run Postoffice");

            break;
        }
            
        default: {
            while(true)
            {
                refresh();
                time(&ltime);
                move(20,0);
                printw(ctime( &ltime ));
                
                if(waitpid(pid,&status,WNOHANG)>0)
                    break;
                
                napms(10);
            }
            
            if (WIFEXITED(status))
            {
                return (int)WEXITSTATUS(status);
            }
            
            break;
        }
    }
}

char* decryptExitCode(int exitCode) {
    
    switch (exitCode) {
        case 0:
            return "Departure sended.";
            
        case LACK_MONEY:
            return "Not enough money.";
            
        case EXCEED_DEPARTURE_LIMIT:
            return "Exceed departure limit.";

        default:
            return "Something wrong.";
    }
}
