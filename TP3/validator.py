import sys

params = sys.argv[1:]

if len(params) < 1:
    print("Informa a quantidade de logs a serem testados")
    sys.exit(1)




def validate(index):
    f = open(f"log_{index}.txt", "r")
    lines = f.readlines()
    requests = []
    grants = []
    releases = []

    for line in lines:
        if ("[R] Request" in line):
            requests.append(int(line.split("-")[1]))
            continue
        if ("[S] Grant" in line):
            if (len(grants) != len(releases)):
                print(line)
                print(len(grants))
                print(len(releases))
                raise Exception("Invalid log file: invalid grants and releases sequence")
            grants.append(int(line.split("-")[1]))
            continue
        if ("[R] Release" in line):
            if (len(releases) != len(grants) - 1):
                raise Exception("Invalid log file: invalid grants and releases sequence")
            releases.append(int(line.split("-")[1]))
            continue

    for i in range(len(requests)):
        if (requests[i] != grants[i] or grants[i] != releases[i]):
            raise Exception("Invalid log file: invalid grants and releases sequence")

    print("Log file was successfully validated")

if __name__ == "__main__":
    print(params)
    for i in range(int(params[0])):
        validate(i)