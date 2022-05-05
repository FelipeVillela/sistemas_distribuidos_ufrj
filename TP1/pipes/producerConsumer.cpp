#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <sys/wait.h>

using namespace std;

#define BUFFER 24

int setDelta() {
  return rand() % 100 + 1;
}

bool isPrime(int num){
  if (num == 1) {
    return false;
  }
  for (int i=2; i <= num/2; i++) {
    if (num%i == 0) {
      return false;
    }    
  }
  return true;
}

void Producer(int pipe[], int n) {
  int n0 = 1;
  int pipeNumber = 1;
  
  char pipeMessage[BUFFER];
  
  while (pipeNumber <= n) {
    
    // Gerando Ni com delta aleatório
    n0 += setDelta();

    sprintf(pipeMessage, "%d", n0);

    // Escrevendo os bytes na write end
    write(pipe[1], &pipeMessage, sizeof(char)*BUFFER);

    cout << "PROD. " << pipeNumber << " - Produto [" << n0 << "] escrito com sucesso!" << endl;
 
    pipeNumber++;
  }

  // Finalizando ao enviar o valor 0
  sprintf(pipeMessage, "%d", 0);
  write(pipe[1], &pipeMessage, sizeof(char)*BUFFER);

  // Finalizando escrita
  close(pipe[1]);

  // Aguardando o Processo filho para sair
	wait(NULL);
	exit(0);   

  cout << "Processo finalizado" << endl;
}

void Consumer(int pipe[]) {
  char pipeMessage[BUFFER];
  int receivedData;
  int counter = 1;

  while (true) {
    // Leitura dos dados que vêm pelo pipe
    read(pipe[0], &pipeMessage, sizeof(char)*BUFFER);
    receivedData = atoi(pipeMessage);

    if(receivedData == 0){
      return;
    } else if (isPrime(receivedData)){
      cout << "CONS. " << counter << "- " << receivedData << " é primo." << endl;
    } else {
      cout << "CONS. " << counter << "- " << receivedData << " não é primo." << endl;
    }

    counter++;
  }
}

int main(int argc, char const *argv[]) {

  if (argc != 2) {
    cout << "Por favor, informe o número de números que deseja que o produtor gere." << endl;
    exit(EXIT_FAILURE);
  }

  int number = atoi(argv[1]);

  // usa o clock interno do computador para controlar a escolha do seed continuamente.
  srand(time(NULL)); 

  int pi[2];

  // Se a função pipe() obtiver sucesso, retornará 0
  if (pipe(pi) != 0) {
    cout << "Erro na criação do pipe. Tente novamente." << endl;
    exit(EXIT_FAILURE);
  }

  // Fazendo um "fork" do processo
  pid_t child= fork();


  child == 0 ? Consumer(pi) : Producer(pi,number);

  return 0;
}