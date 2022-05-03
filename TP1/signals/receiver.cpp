# include <stdlib.h>
# include <iostream>
# include <signal.h>
# include <string>


# define PORT 8080
# define SIZE 255

using namespace std;

int main() {

    // Server receives a signal from the client
    // and prints the message
    // signal(SIGUSR1, [](int sig) {
    //     cout << "Received signal " << sig << endl;
    // });

    int i;

    cout << "Digite um numero: \n" << endl;

    cin >> i;

    return 0;
}