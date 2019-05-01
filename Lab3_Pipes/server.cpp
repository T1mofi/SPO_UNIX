#include <iostream>

#include <string>
#include "Tstr.cpp"

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>

#include <errno.h>

#include <signal.h>


using namespace std;


int closeFlag = 0;

void closeProcess(int sigNum) {
    
    closeFlag = 1;
    
}


void makePipe(string pipeName);
int  openPipe(string pipeName);
string readFromPipe(int pipeDescriptor);
void runManagerProcess();


const int MSG_SIZE = 31;
const string pipeName = "named_pipe";


int main(int argc, const char * argv[]) {
    
    struct sigaction close;
    close.sa_handler = closeProcess;
    sigaction(SIGUSR1, &close, NULL);
    
    runManagerProcess();
    
    makePipe(pipeName);

    int pipeDescriptor = openPipe(pipeName);
    
    while (true) {
        
        string message = readFromPipe(pipeDescriptor);
        
        if (closeFlag == 1) {

            exit(0);
            
        } else {
            
            cout << message << endl;
            
        }
        
    }

    return 0;
}


void makePipe(string pipeName) {
    
    if (mkfifo(str_to_cstr(pipeName), 0666) < 0) {
        
        if (errno != EEXIST) {
            
            cout << "Server: make pipe error." << endl;
            exit(-1);
            
        }
        
    }
}

int openPipe(string pipeName) {
    
    int  pipeDescriptor;
    
    if ((pipeDescriptor = open(str_to_cstr(pipeName), O_RDWR)) < 0) {
        
        cout << "Srver: open pipe error." << endl;
        exit(-1);
        
    }
    
    return pipeDescriptor;
}

string readFromPipe(int pipeDescriptor) {
    
    char cmessage[MSG_SIZE];
    
    if (read(pipeDescriptor, cmessage, MSG_SIZE + 1) < 1) {
        
//        cout << "Server: Read from pipe error.." << endl;
        return "";
        
    }
        
    string message = cmessage;
    
    return message;
}

void runManagerProcess() {
    
    pid_t pid = fork();
    
    switch (pid)
    {
        case -1: {
            
            cout << "FORK ERROR" << endl;
            
        } break;
            
        case  0: {
            
            char* const argin[] = {"Manager", NULL};
            execvp("./Manager", argin);
            cout << "cannot run child pr" << endl;
            exit(-1);
            
        } break;
            
        default:
            break;
            
    }
    
}
