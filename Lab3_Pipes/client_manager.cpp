#include <iostream>
#include <vector>

#include <signal.h>

#include <unistd.h>

#include "Tgetch.cpp"


using namespace std;


pid_t creatNewPrinter();
void  closePrinter(vector<pid_t> & printerPids);
void  closeAllPrinters(vector<pid_t> & printerPids);
void  closeServer();


int main(int argc, const char * argv[]) {
    
    cout << "Client_Manager : input '+' or '-'" << endl;
    
    vector<pid_t> printerPids;
    
    while (true) {
        
        switch (getch()) {
                
            case '=': {
                
                pid_t pid = creatNewPrinter();
                printerPids.push_back(pid);
                
            } break;
                
            case '-': {

                closePrinter(printerPids);
                
            } break;
            
            case 'q': {
                
                closeAllPrinters(printerPids);
                closeServer();
                
                return 0;
            } break;
                
            default:
                break;
                
        }
        
    }
    
    return 0;
}


pid_t creatNewPrinter() {
    
    pid_t pid = fork();
    
    switch (pid)
    {
        case -1: {
            
            cout << "FORK ERROR" << endl;
            
        } break;
            
        case  0: {
            
            char* const argin[] = {"Printer", NULL};
            execvp("./Printer", argin);
            cout << "cannot run child pr" << endl;

        } break;
            
        
        default:
            break;
            
    }
    
    return pid;
}

void closePrinter(vector<pid_t> & printerPids) {
    
    if (printerPids.size()) {
    
        pid_t closedPrinerPID = printerPids.back();
        printerPids.pop_back();
        
        kill(closedPrinerPID, SIGUSR1);
        
    } else {
        
        cout << "Printers are not exist" << endl;
        
    }
    
}

void closeAllPrinters(vector<pid_t> & printerPids) {
    
    for (pid_t pid : printerPids) {
        
        kill(pid, SIGUSR1);
        
    }
    
    printerPids.clear();
    
}

void closeServer() {
    
    kill(getppid(), SIGUSR1);
    
}
