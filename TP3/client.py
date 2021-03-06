import socket
import threading
import os
import time
import datetime as dt
import sys



"""
1. REQUEST: Mensagem enviada por um processo para solicitar acesso à região crítica.
2. GRANT: Mensagem enviada pelo coordenador dando acesso à região crítica.
3. RELEASE: Mensagem enviada por um processo ao sair da região crítica.
"""

params = sys.argv[1:]

write_lock = threading.Lock()

def main():
    if len(params) < 3:
        print("Parâmetros insuficientes")
        sys.exit(1)
    
    HOST = 'localhost' # Endereço IP do servidor
    PORT = 8000 # Porta que o servidor escuta
    F = 1024 # Tamanho de bytes a serem enviados
    k = float(params[0]) # Segundos de intervalo entre uma requisição e outra
    reps = int(params[1]) # Quantidade de requisições de escrita a serem feitas
    test_number = int(params[2]) # Número do teste

    print(f'k: {k}')
    print(f'reps: {reps}')

    # Cria um socket TCP (SOCK_STREAM) IPv4 (AF_INET)
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Tenta conectar ao servidor
        client.connect((HOST, PORT))
    except:
        return "Erro ao conectar ao servidor"


    pid = os.getpid()
    # print("Você está conectado ao servidor")
    
    request_access(client, pid, test_number)
    while True:
        try:
            message = client.recv(F).decode('utf-8')

            if message == '2':
                print("Acesso concedido")
                _write("[S] Grant", client, pid, test_number)                
        
                # Solicita a saída da região crítica
                client.send(f'3|{pid}|{test_number}'.encode('utf-8'))
                reps -= 1

                # Sai do loop após alcançar o número máximo de repetições
                if reps < 0: break

                # Dorme k segundos e solicita acesso novamente
                print(f"Dormindo...{reps}")
                time.sleep(k)
                request_access(client, pid, test_number)
                
        except Exception as e:
            print("Erro ao receber mensagens\n", e)
            client.close()
            break

    print("Conexão encerrada")
    client.close()




def _write(operation_type, client, pid, test_number):
    # Captura as mensagens vindas do servidor
    try:
        while write_lock.locked():
            time.sleep(0.1)
        write_lock.acquire()
        with open(f'log_{test_number}.txt', 'a') as f:
            now = dt.datetime.now()
            f.write(f'{operation_type} {pid} {now}\n')
        write_lock.release()


    except Exception as e:
        print("Erro ao escrever arquivo\n", e)
        client.close()

def request_access(client, pid, test_number):
    try:
        client.send(f'1|{pid}'.encode('utf-8'))
        _write("[R] Request", client, pid, test_number)
    except:
        return "Erro ao enviar mensagens"



if __name__ == '__main__':
    main()