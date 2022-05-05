# include <stdlib.h>
# include <iostream>
# include <signal.h>
# include <string>

// Funcao kill
// https://www.ibm.com/docs/en/i/7.2?topic=ssw_ibm_i_72/apis/sigkill.htms
// lista de signals
// https://www.tutorialspoint.com/unix/unix-signals-traps.htm

using namespace std;

int main(int argc, char const *argv[]) {
    // Server sends a signal to the client

    if (argc < 3) {
        cout << "Por favor, informe o número do processo e o sinal" << endl;
        exit(1);
    }
    
    int PID = stoi(argv[1]);
    int signal = stoi(argv[2]);
    
    string signalName = (signal == 5)? "SIGTRAP" : (signal == 14)? "SIGALRM" : "SIGFPE";

    cout << "Processo selecionado: " << PID << endl;
    cout << "Sinal selecionado: " << signal << " (" << signalName << ")" << endl;

    // Função kill recebe o número do processo e o código do sinal
    // 0 verifica se o processo existe
    int process_exists = kill(PID, 0);

    if (process_exists != 0) {
        cout << "Processo não existe" << endl;
        exit(1);
    } else {
        cout << "Processo existe" << endl;
        kill(PID, signal);
        cout << "Sinal enviado" << endl;
    }

    return 0;
}