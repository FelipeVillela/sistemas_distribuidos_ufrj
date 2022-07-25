import socket
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
    n = int(params[3]) # Número de processos


    # Cria um socket TCP (SOCK_STREAM) IPv4 (AF_INET)
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Tenta conectar ao servidor
        client.connect((HOST, PORT))
    except:
        print("Erro ao conectar ao servidor")
        return


    pid = os.getpid()

    cont = 0
    
    _request_access(client, pid, test_number, n)
    while True:
        try:
            message = client.recv(F).decode('utf-8')

            if message == '2':
                print("Acesso concedido")
                _write(client, pid, test_number, n)
        
                # Solicita a saída da região crítica
                client.send(f'3|{pid}|{test_number}'.encode('utf-8')) 
                cont += 1

                # Sai do loop após alcançar o número máximo de repetições
                if cont == reps: break

                # Dorme k segundos e solicita acesso novamente
                print(f"Dormindo...{reps}")
                time.sleep(k)
                _request_access(client, pid, test_number, n)
                
        except Exception as e:
            print("Erro ao receber mensagens\n", e)
            break

    print("Conexão encerrada")
    client.close()
    return



def _write(client, pid, test_number, n):
    try:
        with open(f'resultado_{test_number}_{n}.txt', 'a') as f:
            now = dt.datetime.now()
            f.write(f'{pid} {now}\n')


    except Exception as e:
        print("Erro ao escrever arquivo\n", e)
        client.close()

def _request_access(client, pid, test_number, n):
    try:
        client.send(f'1|{pid}|{test_number}'.encode('utf-8'))
    except Exception as e:
        print(f"Erro ao enviar mensagens - {str(e)}")
        client.close()


if __name__ == '__main__':
    main()