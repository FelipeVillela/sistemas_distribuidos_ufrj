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


    switch (signum) {
    case SIGFPE:
      cout << "O sinal SIGFPE foi recebido com sucesso!" << endl;
      break;
    case SIGALRM:
      cout << "O sinal SIGALRM foi recebido com sucesso!" << endl;
      break;
    case SIGTRAP:
      cout << "O sinal SIGTRAP foi recebido com sucesso!" << endl;
      break;
    default:
        cout << "O sinal recebido não foi identificado. Tente Novamente" << endl;
    }

    // Termina o processo
    cout << "Interrompendo sinal (" << signum << ") recebido.\n";

    exit(signum);  
}

int main () {

    int wait_type;

    cout << "Informe o número da operação:" << endl;
    cout << "0 - Busy Wait" << endl;
    cout << "1 - Blocking Wait" << endl;

    cin >> wait_type;

    signal(SIGFPE, signalHandler);  
    signal(SIGALRM, signalHandler);
    signal(SIGTRAP, signalHandler);


    if (wait_type == 0) {
        while (true) { // busy wait
            cout << "Aguardando sinal..." << endl;
            sleep(1);
        } 
    } else if  (wait_type == 1) {
        while(true) { // blocking wait
            cout << "Aguardando sinal..." << endl;
            pause();
        }
    } else {
        cout << "Operação inválida" << endl;
    }

   return 0;
}