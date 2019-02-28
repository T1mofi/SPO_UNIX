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
    cout << "\n\r" << "Departure processing...\n\r";
    
    string filePath = "PostStatus.txt";
    
    fstream file(filePath, ios::in);
    if (!file) {
        cout << "File is not open.";                    //!!!!!!!!!!!!!!!!!!!!
        return -1;
    }
    
    int quantitySendLetters;
    int quantitySendParcels;
    int credit;
    file >> quantitySendLetters;
    file >> quantitySendParcels;
    file >> credit;
    
    file.close();
    
    cout << "You deposit: " << atoi(argv[1]) << "\n\r";
    credit += atoi(argv[1]);
    cout << "Credit: " << credit << "\n\r";
    string departureType = argv[2];
    cout << "Departure type: " << departureType << "\n\r";

    if (departureType == "letter") {
        if(credit >= 10) {
            if (quantitySendLetters < 10) {
                quantitySendLetters++;
                credit -= 10;
                cout << "Sending letter..." << "\n\r";
            } else {
                waitExit();
                return EXCEED_DEPARTURE_LIMIT;
            }
        } else {
            waitExit();
            return LACK_MONEY;
        }
    }
    if (departureType == "parcel") {
        if (credit >= 50) {
            if (quantitySendParcels < 5) {
                quantitySendParcels++;
                credit -= 50;
                cout << "Sending parcel..." << "\n\r";
            } else {
                waitExit();
                return EXCEED_DEPARTURE_LIMIT;
            }
        } else {
            waitExit();
            return LACK_MONEY;
        }
    }

    file.open(filePath, ios::out);
    if (!file) {
        cout << "file is not open for wriring!";            //!!!!!!!!!!!!!!!!!!!!
        return - 1;
    }
    file << quantitySendLetters << " " << quantitySendParcels << " " << credit;
    file.close();
    
    cout << "\rDeparture processing end. Press 'Q'\n";
    waitExit();
    
    return 0;
}

void waitExit() {
    char c = '0';
    
    while(c != 'q')
    {
        halfdelay(2);
        c = getchar();
    }
}
