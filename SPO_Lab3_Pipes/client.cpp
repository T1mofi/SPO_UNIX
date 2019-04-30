#include <iostream>

#include <string>
#include "Tstr.cpp"

#include <fcntl.h>
#include <unistd.h>


using namespace std;


//make handler for terminate signal


int openPipe(string pipeName);
void writeToPipe(string message, int pipeDescriptor);


const int MSG_SIZE = 31;
const string pipeName = "named_pipe";


int main(int argc, const char * argv[]) {
    
    int  pipeDescriptor = openPipe(pipeName);
    
    for (int i = 0; i < 3; i++) {
        
        writeToPipe(to_string(getpid()), pipeDescriptor);
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

void writeToPipe(string message, int pipeDescriptor) {
    
    if (write(pipeDescriptor, str_to_cstr(message), MSG_SIZE + 1) < 0) {
        
        cout << "Write error" << endl;
        exit(-1);
        
    }
    
}
