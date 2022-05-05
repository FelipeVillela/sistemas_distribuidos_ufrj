#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <math.h>

using namespace std;

#define BUFFER 24

int setDelta() {
  return rand() % 100 + 1;
}

bool isPrime(int num){
  for (int i=2; i <= num/2; i++) {
    if (num%i == 0) {
      return false;
    }    
  }
  return true;
}

void Producer(int PIPE[], int n) {
  char pipe_msg[BUFFER];
  int n0 = 1;
  int pipeNumber = 1;
  
  while (pipeNumber <= n) {
    sprintf(pipe_msg, "%d", n0);
    write(PIPE[1], &pipe_msg, sizeof(char)*BUFFER);
    cout << "PROD." << pipeNumber << " - Produto [" << n0 << "] escrito com sucesso!" << endl;
    n0 += setDelta();

    pipeNumber++;
  }

  // Sinalizar que finalizou enviando 0
  sprintf(pipe_msg, "%d", 0);
  write(PIPE[1], &pipe_msg, sizeof(char)*BUFFER);
  cout << endl;
}

int main(int argc, char const *argv[]) {

  if (argc != 2) {
    cout << "Por favor, informe o número de números que deseja que o produtor gere." << endl;
    exit(EXIT_FAILURE);
  }

  int number = atoi(argv[1]);

  // usa o clock interno do computador para controlar a escolha do seed continuamente.
  srand(time(NULL)); 

  int PIPE[2];

  // Processo pai
  Producer(PIPE,number);

  return 0;
}