#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

# define PORT 8080
# define SIZE 255

using namespace std;

int isPrime(int num){
  if (num == 1) {
    return 0;
  }
  for (int i=2; i <= num/2; i++) {
    if (num%i == 0) {
      return 0;
    }    
  }
  return 1;
}


int main() {

    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        cout << "Erro na criação do socket" << endl;
        exit(EXIT_FAILURE);
    }
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
 
    // Converte ipv4 em binario
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
        <= 0) {
            cout << "Endereço inválido ou não suportado" << endl;
        exit(EXIT_FAILURE);
    }
 
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "Falha ao conectar com o servidor" << endl;
        exit(EXIT_FAILURE);
    } else {

        while(true) {
            char buffer[SIZE];
            int qt_nums, valread, valwrite;
            int is_prime;

            bzero(buffer, sizeof(buffer));

            valread = read(sock, buffer, sizeof(buffer));
            if (valread < 0) {
                cout << "Falha ao ler no socket" << endl;
                exit(EXIT_FAILURE);
            }

            qt_nums = atoi(buffer);
            cout << "Quantidade de números: " << qt_nums << endl;
            if(qt_nums == 0) {
                break;
            }
            else {

                for(int i = 2; i < qt_nums; i++) { 
                    if (qt_nums % i == 0) {
                        is_prime=0;
                        break;
                    }
                    else if (i == qt_nums - 1)
                        is_prime=1;
                    
                }
                    
                // cout << buffer << is_prime << endl;
                sprintf (buffer, "%d", is_prime);
                valwrite = write(sock, buffer, sizeof(buffer));
                if (valwrite < 0) {
                    cout << "Falha ao escrever no socket" << endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
        return 0;
    }
}