
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <thread> 
#include <mutex> 
#include <condition_variable>
#include <chrono>
#include <atomic>

#define M 100000 // Números processados pelo consumidor
#define RANGE_MAX 10000000 // Número máximo gerado

using namespace std;

// Memory Vectors
vector<int> shared_memory; // vetor compartilhado com N posições de inteiros
vector<int> consumer_memory;
// Semaphores Variables
mutex s_mutex; // mutex semaphore 
condition_variable s_full;  // semáforo para indicar que a memória compartilhada está cheia
condition_variable s_empty;  // semáforo para indicar que a memória compartilhada está vazia

int qt_numbers = M;
int N; // Número de espaço em memória para o vetor
int elements = 0;
atomic<int> p_working(0); //Produtores em execução
int produced = 0;
int consumed = 0;

bool isPrime(int n){
    int i;
    for (i=2; i<n; i++){
        if (n % i == 0){
            return false;
        }
    }
    return true;
}

int getRandomNumber(int n){
    srand(n+time(NULL));
    int r = rand() % RANGE_MAX + 1;
    return r;
}

int findEmptyPosition(vector<int> v){
	// return -1 se o vetor estiver cheio
	int pos = -1;
	int i = 0;
	while(i < v.size()){
		if(v[i] == 0){
			pos = i;
			break;
		}
		i++;
	}
	return pos;
}

int findProductPosition(vector<int> v){
	// return -1 caso o vetor esteja vazio
	int pos = -1;
	int i = 0;
	while(i < v.size()){
		if(v[i] != 0){
			pos = i;
			break;
		}
		i++;
	}
	return pos;
}

void produce(int p_id){
	unique_lock<mutex> lock(s_mutex);
	int positionToStore = findEmptyPosition(shared_memory); // -1 se o vetor estiver cheio
	if(s_full.wait_for(lock,chrono::milliseconds(200),[] {return elements != N;}) && positionToStore  != -1){
		int r = getRandomNumber(p_id); // p_id é usado para gerar um número aleatório
		shared_memory[positionToStore] = r; 
		elements += 1;
		produced +=1;
		s_empty.notify_all();
	}
}

void consume(){
	unique_lock<mutex> lock(s_mutex);
	int positionToGet = findProductPosition(shared_memory); // -1 iff the vector is empty
	if(s_empty.wait_for(lock,chrono::milliseconds(200),[] {return elements > 0;}) && positionToGet  != -1){
		isPrime(shared_memory[positionToGet]);
		// cout << shared_memory[positionToGet]<< " is prime? The answer is: "<< isPrime(shared_memory[positionToGet])<< endl;
		consumer_memory.push_back(shared_memory[positionToGet]); // saving the number in local memory
		shared_memory[positionToGet] = 0; // cleaning the position after read a number
		elements -= 1;
		consumed +=1;
		s_full.notify_all();
	}
}


void consumer(){
	while(p_working == 0){
		this_thread::yield();
	}
	while(consumed < qt_numbers){
		consume();
	}
}

void producer(){
	p_working += 1;
	while(produced < qt_numbers){
		produce(produced);
	}
	p_working -= 1;
}

int main(int argc, char* argv[]) {
	N = atoi(argv[1]); // Número de espaços em memória
	int nc = atoi(argv[2]);  // Número de threads consumidoras
	int np = atoi(argv[3]);  // Número de threads produtoras

	// Measure of time 
	chrono::time_point<std::chrono::system_clock> start, end;
	start = chrono::system_clock::now();


	int qt_threads = nc+np; //total de threads
	thread threads[qt_threads];

	// Preenche vetor com zeros
	for (int i = 0; i < N; ++i){
		shared_memory.push_back(0);
	}
	// Launch  Threads
	for(int i = 0; i < np; i++){
		threads[i] = thread(producer);
	}
	for(int i = np; i < qt_threads; i++){
		threads[i] = thread(consumer);
	}
	// Join Threads
	for (int i = 0; i < qt_threads; i++){ 
		threads[i].join();
	}

	end = chrono::system_clock::now();


	long exec_time = std::chrono::duration_cast<std::chrono::milliseconds> (end-start).count();
	cout << exec_time  << endl;
	return 0;
}