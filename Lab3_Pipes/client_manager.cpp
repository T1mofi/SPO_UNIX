#include <iostream>
#include <vector>

#include <signal.h>

#include <unistd.h>

#include "Tgetch.cpp"

#include <semaphore.h>


using namespace std;


pid_t creatNewPrinter();
void  closePrinter(vector<pid_t> & printerPids);
void  closeAllPrinters(vector<pid_t> & printerPids);
void  closeServer();

#define SEMAPHORE_NAME "/my_nam"


int main(int argc, const char * argv[]) {
    
    cout << "Client_Manager : input '+' or '-'" << endl;
    
    sem_t *sem;
    
    if ( (sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0777, 0)) == SEM_FAILED ) {
        return 1;
    }
    
    vector<pid_t> printerPids;
    
    while (true) {
        
        switch (getch()) {
                
            case '=': {
                
                //Signal for first printer
                if (printerPids.size() == 0) {
                    
                    sem_post(sem);
                    
                }
                
                pid_t pid = creatNewPrinter();
                printerPids.push_back(pid);
                
            } break;
                
            case '-': {
                
                if (printerPids.size()) {
                    
                    closePrinter(printerPids);
                    
                    //Reset semaphore in start state
                    if (printerPids.size() == 0) {
                        
                        sem_wait(sem);
                        
                    }
                }
                
            } break;
            
            case 'q': {
                
                if (printerPids.size()) {
                    
                    closeAllPrinters(printerPids);
                    
                    //Reset semaphore in start state
                    sem_wait(sem);
                    
                }
                
                sleep(1);
                closeServer();
                sem_close(sem);
                
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
