#include <iostream>
#include <string>
#include <vector>

#include <signal.h>

#include <unistd.h>
#include <fcntl.h>

#include "Tstr.cpp"
#include "Tgetch.cpp"

#include <pthread.h>


using namespace std;


int counter = 0;
int  pipeDescriptor;
pthread_mutex_t mp;
pthread_mutex_t counterMutex;
vector<pthread_t> printerIDs;
pthread_t printThreadID = 0;

void closeProcess(int sigNum) {
    
    if (printThreadID == pthread_self()) {
        pthread_mutex_unlock(&mp);
    }

    pthread_exit(NULL);
    
}

int openPipe(string pipeName);
void writeToPipe(int ID, int pipeDescriptor);


const int MSG_SIZE = 31;
const string PIPE_NAME = "named_pipe";


void* newthread(void* arg){
    
    pthread_mutex_lock(&counterMutex);
    int ID = ++counter;
    pthread_mutex_unlock(&counterMutex);
    
    struct sigaction closePr;
    closePr.sa_handler = closeProcess;
    sigaction(SIGUSR1, &closePr, NULL);
    
    while (true) {
        
        pthread_mutex_lock(&mp);
        
        printThreadID = pthread_self();
        writeToPipe(ID, pipeDescriptor);
        printThreadID = 0;
        
        pthread_mutex_unlock(&mp);
        sleep(1);
        
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

void writeToPipe(int ID, int pipeDescriptor) {
    
    if (write(pipeDescriptor, str_to_cstr(""), MSG_SIZE + 1) < 0) {
        
        cout << "Write error" << endl;
        exit(-1);
        
    }
    
    for (int i = 0; i < 3; ++i) {
        
        if (write(pipeDescriptor, int_to_cstr(ID), MSG_SIZE + 1) < 0) {
            
            cout << "Write error" << endl;
            exit(-1);
            
        }
        sleep(1);
        
    }
}

////////////////////////////////////////////////////////////////////////

pthread_t creatNewPrinter();
void  closePrinter();
void  closeAllPrinters();
void  closeServer();


int main(int argc, const char * argv[]) {

    pipeDescriptor = openPipe(PIPE_NAME);
    
    if(pthread_mutex_init(&mp, NULL)!=0){
        cout << "Mutex create error" << endl;
    }
    
    if(pthread_mutex_init(&counterMutex, NULL)!=0){
        cout << "Mutex create error" << endl;
    }
    
    
    cout << "Client_Manager : input '+' or '-'" << endl;
    
    while (true) {
        
        switch (getch()) {
                
            case '=': {
                
                pthread_t threadID = creatNewPrinter();
                printerIDs.push_back(threadID);
                
            } break;
                
            case '-': {
                
                    closePrinter();
                
            } break;
            
            case 'q': {
  
                closeAllPrinters();
                close(pipeDescriptor);
                
                sleep(1);
                closeServer();
                
                return 0;
            } break;
                
            default:
                break;
                
        }
        
    }
    
    return 0;
}


pthread_t creatNewPrinter() {
    
    pthread_t threadID;
    
    if(pthread_create(&threadID,NULL, newthread, NULL)!=0){
        cout << "Thread create error" << endl;
    }
    
    return threadID;
}

void closePrinter() {
    
    if (printerIDs.size()) {
    
        pthread_t closedPrinerID = printerIDs.back();
        printerIDs.pop_back();
        
        pthread_kill(closedPrinerID, SIGUSR1);
        
    } else {
        
        cout << "Printers are not exist" << endl;
        
    }
    
}

void closeAllPrinters() {
    
    if (printerIDs.size()) {
        
        for (pthread_t threadID : printerIDs) {
            
            pthread_kill(threadID, SIGUSR1);
            
        }
        
        printerIDs.clear();
        
    }
    
}

void closeServer() {
    
    kill(getppid(), SIGUSR1);
    
}
