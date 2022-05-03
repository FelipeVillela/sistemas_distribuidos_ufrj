# include <stdlib.h>
# include <iostream>
# include <signal.h>
# include <string>
# include <csignal>
#include <unistd.h>



# define PORT 8080
# define SIZE 255

using namespace std;

void signalHandler( int signum ) {
   cout << "Interrupt signal (" << signum << ") received.\n";

   // cleanup and close up stuff here  
   // terminate program  

   exit(signum);  
}

int main () {

    int wait_type;

   // register signal SIGINT and signal handler  
   signal(SIGINT, signalHandler);  

   cout << "Informe o numero da operação" << endl;
   cout << "0 - Busy Wait" << endl;
   cout << "1 - Blocking Wait" << endl;

   cin >> wait_type;


    if (wait_type == 0) {
        while (1) { // busy wait
            cout << "Aguardando sinal..." << endl;
            sleep(1);
        } 
    } else if  (wait_type == 1) {
        while(1) { pause(); } // blocking wait
    } else {
        cout << "Operação inválida" << endl;
    }

   return 0;
}