#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"

using namespace std;

void waitExit();

int main(int argc, char* argv[]) {
    
    initscr();
    noecho();
    
    move(7,0);
    printw("\rDeparture processing...");
    
    string filePath = "PostStatus.txt";
    fstream file(filePath, ios::in);
    
    if (!file) {
        printw("File is not open.");                    //!!!!!!!!!!!!!!!!!!!!
        return -1;
    }
    
    int quantitySendLetters;
    int quantitySendParcels;
    int credit;
    file >> quantitySendLetters;
    file >> quantitySendParcels;
    file >> credit;
    
    file.close();
    
    credit += atoi(argv[1]);
    string departureType = argv[2];
    
        if (departureType == "letter") {
            if(quantitySendLetters < 10) {
                if (credit >= 10) {
                    quantitySendLetters++;
                    credit -= 10;
                } else {
                    waitExit();
                    return LACK_MONEY;
                }
            } else {
                waitExit();
                return EXCEED_DEPARTURE_LIMIT;
            }
        }
        if (departureType == "parcel") {
            if (quantitySendParcels < 5) {
                if (credit >= 50) {
                    quantitySendParcels++;
                    credit -= 50;
                } else {
                    waitExit();
                    return LACK_MONEY;
                }
            } else {
                waitExit();
                return EXCEED_DEPARTURE_LIMIT;
            }
        }

        file.open(filePath, ios::out);
        if (!file) {
            return - 1;
        }
        file << quantitySendLetters << " " << quantitySendParcels << " " << credit;
        file.close();
    
        waitExit();

    return 0;
}

void waitExit() {
    move(8,0);
    printw("\rEnd processing");
    
    char c = 0;
    while(c != 'q')
    {
        move(9,0);
        printw("\rPress 'Q' for exit\r");
        
        halfdelay(2);
        c = getch();
        
        refresh();
    }
}
