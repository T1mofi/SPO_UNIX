#include <iostream>

#include <string>
#include "Tstr.cpp"

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>

#include <errno.h>


using namespace std;

//make handler for terminate signal


void makePipe(string pipeName);
int  openPipe(string pipeName);
string readFromPipe(int pipeDescriptor);


const int MSG_SIZE = 31;
const string pipeName = "named_pipe";


int main(int argc, const char * argv[]) {
    
    //run client_manager process
    
    makePipe(pipeName);
    
    int pipeDescriptor = openPipe(pipeName);
    
    while (true) {
        
        string message = readFromPipe(pipeDescriptor);
        cout << message << endl;
        
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
        
        cout << "Server: Read from pipe error." << endl;
        exit(-1);
        
    }
        
    string message = cmessage;
    
    return message;
}