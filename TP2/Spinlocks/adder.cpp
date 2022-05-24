#include <unistd.h>
#include <iostream>
#include <atomic>
#include <pthread.h>
#include <time.h>

using namespace std;

struct Lock {
  atomic_flag held = ATOMIC_FLAG_INIT;
};

// Estrutura que será recebida pela thread (contendo todos os parâmetros)
struct Thread {
  Lock *lock;
  char *vector; // Vetor com os números
  int *portionsQnt; // Quantidade de parcelas
  int *sum; // Soma
};

void acquire(Lock *lock) {
  // busy wait
  while(lock->held.test_and_set());
}

void release(Lock *lock) {
  lock->held.clear();
}

int setRandomNumber(int i, int j) {
  return i + rand()%(j-i + 1);
}

void *Adder(void *args){
  // Criando thread atual
  Thread *thisThread = (Thread *) args;

  //Definindo valores iniciais pra parcela e soma parcial
  int portion = *thisThread->portionsQnt;
  int partialSum = 0;

  // For loop para fazer a soma parcial da thread
  for (int i=0; i<portion; i++) {
    partialSum += (int)thisThread->vector[i];
  }

  acquire(thisThread->lock);

  *thisThread->sum += partialSum;

  release(thisThread->lock);

  return NULL;
}

int main(int argc, char const *argv[]){
  if (argc != 3) {
    cout << "Erro! É necessário que o tamanho do vetor n e o número de threads que serão utilizadas (k) sejam passadas." << endl;
    exit(EXIT_FAILURE);
  }
  srand(time(NULL));

  // Definindo o tamanho do vetor n e alocando 1 byte para cada valor
  int n = atoi(argv[1]);
  char *vector = new char[n];
  
  // Definindo número de threads e criando-as
  int k = atoi(argv[2]);
  pthread_t threads[k];
  Thread **params = new Thread *[k];

  int portion = n/k;
  int soma = 0;
  int i;
  Lock lock;
  
  double average = 0;
  int iterator = 10;
  clock_t initial, final;

  // Populando o vetor com valores escolhidos aleatoriamente
  for (i=0; i<n; i++) {
    vector[i] = (char)setRandomNumber(-100,100);
  }
  
  // Estruturando os parâmetros para cada thread
  for (i=0; i<k; i++){
    params[i] = new Thread();
    params[i]->lock = &lock;
    params[i]->vector = &vector[portion*i];
    params[i]->portionsQnt = &portion;
    params[i]->sum = &soma;
    
  }

  // Executando 10 vezes e obtendo a média de tempo.
  for (int z=0; z<iterator; z++) {
    
    // Criando as threads
    initial = clock();
    for (i=0; i<k; i++) {
      pthread_create(&threads[i], NULL, Adder, (void *) params[i]);
    }
    
    // Aguardando o fim das threads
    for (i=0; i<k; i++) {
      pthread_join(threads[i],NULL);
    }
    final = clock();

    // Calculando média e e resetando a soma
    average += (double)(final - initial)/CLOCKS_PER_SEC;
    soma = 0; 
  }

  cout << n << ", " << k << ", " << average/(double)iterator << endl;
  return 0;
}