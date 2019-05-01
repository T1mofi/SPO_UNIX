#include <iostream>

#include <string>
#include "Tstr.cpp"

#include <fcntl.h>
#include <unistd.h>

#include <signal.h>

#include <semaphore.h>


using namespace std;


int closeFlag = 0;

void closeProcess(int sigNum) {
    
    closeFlag = 1;
    
}


int openPipe(string pipeName);
void writeToPipe(string message, int pipeDescriptor);


const int MSG_SIZE = 31;
const string pipeName = "named_pipe";
#define SEMAPHORE_NAME "/my_nam"


int main(int argc, const char * argv[]) {
    
    struct sigaction closePr;
    closePr.sa_handler = closeProcess;
    sigaction(SIGUSR1, &closePr, NULL);
    
    int  pipeDescriptor = openPipe(pipeName);
    
    sem_t *sem;
    
    if ( (sem = sem_open(SEMAPHORE_NAME, 0)) == SEM_FAILED ) {
        return 1;
    }
    
    while (true) {
        
        if (sem_wait(sem) == 0)
        {
            
            cout << endl << endl << "Client: wait work fine" << endl;
            writeToPipe(to_string(getpid()), pipeDescriptor);
            sem_post(sem);
            
        }
        
        if (closeFlag == 1) {
            
            sem_close(sem);
            close(pipeDescriptor);
            exit(0);
            
        }
        
    }
    
    return 0;
}


int openPipe(string pipeName) {
    
    int  pipeDescriptor;
    
    if ((pipeDescriptor = open(str_to_cstr(pipeName), O_WRONLY ) ) < 0) {
        
        cout << "Client: open pipe error" << endl;
        exit(-1);
        
    }
    
    return pipeDescriptor;
}

void writeToPipe(string message, int pipeDescriptor) {
    
    for (int i = 0; i < 3; ++i) {
        
        if (write(pipeDescriptor, str_to_cstr(message), MSG_SIZE + 1) < 0) {
            
            cout << "Write error" << endl;
            exit(-1);
            
        }
        sleep(1);
        
    }
}
