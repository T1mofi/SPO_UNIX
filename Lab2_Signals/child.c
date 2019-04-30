#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

using namespace std;


void closeP(int i){
    exit(0);
}

void playMusic() {
    int sig;
    sigset_t   set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR2);
    sigprocmask(SIG_BLOCK, &set, NULL);

    string uniqueStr = "Process";

    while (true) {
        sigwait(&set, &sig);
        
        cout << "-----------------" << endl;
        for (int i = 0; i < uniqueStr.size(); ++i) {
            cout << uniqueStr[i] << endl;
            usleep(200000);
        }
        cout << "PID " << getpid() << endl;
        cout << "-----------------" << endl;
        
        kill(getppid(),SIGUSR1);
    }
}

int main(int argc, const char * argv[]) {
    struct sigaction closeProg;
    closeProg.sa_handler = closeP;
    sigaction(SIGUSR1,&closeProg,NULL);
    
    playMusic();

    return 0;
}
