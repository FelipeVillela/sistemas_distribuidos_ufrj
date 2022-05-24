import subprocess

def makefile():
  subprocess.run('g++ -pthread -o adder.out adder.cpp'.split())

# Definindo n e k
n = [10**7, 10**8, 10**9]
k = [1, 2, 4, 8, 16, 32, 64, 128, 256]

def main():
  file = open('results.csv', 'w')

  for i in n:
    for j in k:
      details = ['./adder.out', str(i), str(j)]
      output = subprocess.run(details, stdout = file)

  file.close()

if (__name__ == '__main__'):
  makefile()
  main()

