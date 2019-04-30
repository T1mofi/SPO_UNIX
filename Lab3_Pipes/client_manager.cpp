#include <iostream>

using namespace std;

int main(int argc, const char * argv[]) {
    
    cout << "Client_Manager : input '+' or '-'" << endl;
    
    while (true) {
        switch (getchar()) {
            case '+':
                //create new printer
                break;
                
            case '-':
                //remove one printer
                break;
                
            case 'q':
                //close printers
                //close server
                return 0;
                break;
                
            default:
                break;
        }
        cin.ignore(1);
    }
    
    return 0;
}
