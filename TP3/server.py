import socket
import threading
from queue import Queue


constants = {
    'HOST': 'localhost', # Endereço IP do servidor
    'PORT': 8000, # Porta que o servidor escuta
    'F': 1024, # Tamanho de bytes a serem enviados
    'N': 256, # Número máximo de conexões aceitas
}


# Lista que armazena os clientes conectados
clients = Queue()

lock = threading.Lock() # Lock que controla o acesso à região crítica


def main():


    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Inicia o servidor TCP
    try:
        server.bind((constants['HOST'], constants['PORT']))
        server.listen(constants['N'])

        print("servidor iniciado...\n")
        print("Aguardando conexões\n")

    except Exception as e:
        print("Erro ao iniciar ao servidor", e)
        return

    while True:
        client = server.accept()

        # Cria uma thread que irá coordenar o acesso à região crítica
        thread_coordinator = threading.Thread(target=_coordinate_access, args=(client,))

        thread_coordinator.start()




def _coordinate_access(client):
    # Captura e coordena solicitações de acesso a região crítica
    while True:
        try:
            message = client.recv(constants['F']).decode('utf-8').split('|')
            
            if len(message) < 3:
                print("Encerrando conexão")
                client.close()
                return
            
            pid = message[1]

            if message[0] == '1':
                print(f"Processo {pid} solicitou acesso à região crítica")
                              
                clients.put((client, pid))

                if lock.locked():
                    client.send('Aguarde...'.encode('utf-8'))                    
                    
                else: 
                    next_client = clients.get()

                    _grant_access(next_client[0], next_client[1])



            elif message[0] == '3':
                print(f"Processo {pid} saiu da região crítica")
                lock.release()

                if not clients.empty():
                    next_client = clients.get(0)

                    _grant_access(next_client[0], next_client[1])

            elif message[0] == '4':
                print(f"Processo {pid} - {message[2]}")


        except Exception as e:
            print(f"Erro ao receber mensagens: {str(e)}")
            client.close()
            break


def _grant_access(client, pid):
    try:
        lock.acquire()
        client.send('2'.encode('utf-8'))
        print(f"Acesso concedido ao processo {pid}")
    except Exception as e:
        print("Erro grant: ", str(e))
        client.close()
        lock.release()    

if __name__ == '__main__':
    main()