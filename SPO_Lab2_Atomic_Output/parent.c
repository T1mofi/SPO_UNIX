#include <iostream>
#include <signal.h>
//#include <curses.h>
#include <time.h>
#include <unistd.h>

#include <vector>


using namespace std;

void creatNewPrinter();
void closePrinter();
void closeAllPrinters();

int nobodyPrint = 1;
vector<pid_t> printers;
int workPrinterIndex = 0;

struct sigaction printSignal;
void setPrint(int sign) {
    if (printers.size()) {
        if (++workPrinterIndex >= printers.size()) {
            workPrinterIndex = 0;
        }
        sleep(1);
        kill(printers[workPrinterIndex], SIGUSR2);
        
    } else {
        nobodyPrint = 1;
    }
    return;
}

int main(int argc, const char * argv[]) {
    
    printSignal.sa_handler = setPrint;
    printSignal.sa_flags = SA_RESTART;
    sigaction(SIGUSR1,&printSignal,NULL);
    
    cout << "Main control process : input '+' or '-'" << endl;
    
    while (true) {
        switch (getchar()) {
            case '=':
                creatNewPrinter();
                if (nobodyPrint == 1) {
                    sleep(1);
                    kill(printers.back(), SIGUSR2);
                    nobodyPrint = 0;
                }
                break;
                
            case '-':
                if (printers.size()) {
                    closePrinter();
                }
                break;
                
            case 'q':
                closeAllPrinters();
                return 0;
                break;
                
            default:
                break;
        }
        cin.ignore(1);
    }
    
    return 0;
}

void creatNewPrinter() {
    pid_t pid = fork();
    
    switch (pid)
    {
        case -1:
            cout << "FORK ERROR" << endl;
            break;
        case 0: {
            char* const argin[] = {"Printer", NULL};
            execvp("./Printer", argin);
            cout << "cannot run child pr";
            break;
        }
        default:
            printers.push_back(pid);
            break;
    }
}

void closePrinter() {
    pid_t closePID = printers.back();
    printers.pop_back();
    
    if (closePID == printers[workPrinterIndex]) {
        kill(getpid(),SIGUSR1);
    } else {
        kill(closePID, SIGUSR1);
        kill(closePID, SIGUSR2);
    }
    
    
}

void closeAllPrinters() {
    for(pid_t &closePID : printers) {
        kill(closePID, SIGUSR1);
        kill(closePID, SIGUSR2);
    }
    printers.clear();
}
