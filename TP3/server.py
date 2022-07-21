import socket
import threading
from queue import Queue

constants = {
    'HOST': 'localhost', # Endereço IP do servidor
    'PORT': 8000, # Porta que o servidor escuta
    'F': 1024, # Tamanho de bytes a serem enviados
    'N': 10, # Número máximo de conexões aceitas
}


# Lista que armazena os clientes conectados
clients = []

lock = threading.Lock()


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
        client, address = server.accept()

        # Cria uma thread que irá coordenar o acesso à região crítica
        thread_coordinator = threading.Thread(target=coordinate_access, args=(client,))

        thread_coordinator.start()




def coordinate_access(client):
    # Captura e coordena solicitações de acesso a região crítica
    while True:
        try:
            message = client.recv(constants['F']).decode('utf-8').split('|')

            if message[0] == '1':
                print(f"Processo {message[1]} solicitou acesso à região crítica")
                
                # Adiciona o cliente à fila de clientes
                clients.append(client)

                if lock.locked():
                    client.send('Aguarde...'.encode('utf-8'))                    
                    
                else:
                    # lock.acquire()
                    # print("Acesso concedido")
                    # clients[0].send('2'.encode('utf-8'))
                    _grant_access(client)



            elif message[0] == '3':
                print(f"Processo {message[1]} saiu da região crítica")
                lock.release()

                clients.pop(0)
                if len(clients) > 0:
                    # lock.acquire()
                    # clients[0].send('2'.encode('utf-8'))
                    _grant_access(clients[0])


        except Exception as e:
            print("Erro ao receber mensagens", str(e))
            client.close()
            break


def _grant_access(client):
    try:
        lock.acquire()
        client.send('2'.encode('utf-8'))
    except Exception as e:
        print("Erro: ", str(e))
        client.close()
        clients.remove(client)
        lock.release()

def broadcast(message, client):
    # Recebe a mensagem do cliente e repassa a todos os outros
    for c in clients:
        if c != client:
            try:
                c.send(message.encode('utf-8'))
            except:
                print("Erro ao enviar mensagens")
                # c.close()
                clients.remove(c)

main()