#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

# define PORT 8080
# define SIZE 255

using namespace std;


int getRandomNumber() {
  return rand() % 1000 + 1;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        cout << "Por favor, informe o número de números a sortear" << endl;
        exit(EXIT_FAILURE);
    }
    int qt_nums = stoi(argv[1]);
    int new_socket, server_fd, opt, valwrite, valread;
    struct sockaddr_in address;
    socklen_t address_len = sizeof(address);
    char buffer[SIZE];

    cout << "Inicializando socket..." << endl;

    // Cria o file descriptor do socket
    // socket(domain, type, protocol)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == 0) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }
     // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
 
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    cout << "Server online na porta " << PORT << endl;
    cout << "Aguardando conexão do client" << endl;

    new_socket = accept(server_fd, (struct sockaddr *) &address, &address_len);
    if (new_socket < 0) {
        perror("Erro ao aceitar conexão");
        exit(EXIT_FAILURE);
    } else {
        cout << "Conexão aceita" << endl;


        int is_prime;
        int random_number = 1;
       
        while(true){
            
            bzero(buffer, sizeof(buffer));

            random_number += getRandomNumber();

            if (qt_nums <= 0) {
                cout << "Finalizando conexão" << endl;
                break;
            }

            sprintf (buffer, "%d", random_number);
            // cout << buffer << random_number << endl;
            
            valwrite = write(new_socket, buffer, sizeof(buffer));
            if (valwrite < 0) {
                cout << "Falha ao escrever no socket" << endl;
                exit(EXIT_FAILURE);
            }

            valread = read(new_socket, buffer, sizeof(buffer));
            if (valread < 0) {
                cout << "Falha ao ler no socket" << endl;
                exit(EXIT_FAILURE);
            }

            is_prime = atoi(buffer);            
            if (is_prime == 1) { cout << "O número sorteado " << random_number << " é primo" << endl; }   
            else { cout << "O número sorteado " << random_number <<  " não é primo" << endl; }
            
            qt_nums--;
            }
            
        }
    
    close(new_socket);
    return 0;
}