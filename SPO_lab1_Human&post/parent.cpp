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

void postRequest(char* departureType, char* money);
string decryptExitCode(int exitCode);

int main(int argc, const char * argv[]) {
    
    initscr();
    curs_set(0);
    
    while (true)
    {
        refresh();
        cout << "\rPlease, select an action:\n";
        cout << "\r1) Send a letter(10 BYN).\n";
        cout << "\r2) Send a parcel(50 BYN).\n";
        cout << "\r3) End work.\n\r";
        cout << "\rYour choice: ";
        int choice = 0;
        cin >> choice;
        cout << endl;
        
        char* departureType;
        
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
                cout << "\rGoodbye!\n";
                endwin();
                return 0;
            }
            default: {
                cout << "\rYour choice is incorrect.\n";
                endwin();
                return 0;
//                continue;
            }
        }
        
        cout << "\rPlease, deposit money: ";
        char money[10];
        cin >> money;
        
        postRequest(departureType, money);
    }
    
    return 0;
}

void postRequest(char* departureType, char* money) {
    
    pid_t pid = fork();
    
    
    time_t ltime;
    int status = 0;
    
    switch (pid) {
        case -1: {
            cout << "Fork error.\n\r";
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
                move(9,0);
                printw(ctime( &ltime ));
                
                if(waitpid(pid,&status,WNOHANG)>0)
                    break;
                
                napms(10);
            }
            move(20,0);
            
            if (WIFEXITED(status))
            {
                move(21,0);
                cout << "\r" << decryptExitCode((int)WEXITSTATUS(status)) << "\n";
            }
            
            break;
        }
    }
}

string decryptExitCode(int exitCode) {
    
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
