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
    
    //    cout << "You deposit: " << atoi(argv[1]) << "\n\r";
    credit += atoi(argv[1]);
    //    cout << "Credit: " << credit << "\n\r";
    string departureType = argv[2];
    //    cout << "Departure type: " << departureType << "\n\r";
    
        if (departureType == "letter") {
            if(quantitySendLetters < 10) {
                if (credit >= 10) {
                    quantitySendLetters++;
                    credit -= 10;
    //                cout << "Sending letter..." << "\n\r";
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
    //                cout << "Sending parcel..." << "\n\r";
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
    //        cout << "file is not open for wriring!";            //!!!!!!!!!!!!!!!!!!!!
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
//    endwin();
}
