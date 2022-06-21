import random
import string
import os
import subprocess

def random_file(N):
    text = ''.join(random.choices(string.ascii_uppercase + string.ascii_lowercase, k=N))
    with open("perf_src.txt", "w") as f:
        f.write(text)



def bench(maxN, step=20):
    N = []
    keys = []
    enc_seq = []
    enc_par = []
    dec_seq = []
    dec_par = []
    for file_char_count in range(0, maxN+1, step):
        random_file(file_char_count)
        print(file_char_count)
        stdout = subprocess.run(["NTRU.exe"], capture_output=True, text=True).stdout.strip()
        values = stdout.split("\n")
        print(values)
        N.append(file_char_count)
        keys.append(values[0])
        enc_seq.append(values[1])
        enc_par.append(values[2])
        dec_seq.append(values[3])
        dec_par.append(values[4])

    with open("../results/N.txt", "w") as file:
        print(*N, file=file, sep="\n")
    with open("../results/keys.txt", "w") as file:
        print(*keys, file=file, sep="\n")
    with open("../results/enc_seq.txt", "w") as file:
        print(*enc_seq, file=file, sep="\n")
    with open("../results/enc_par.txt", "w") as file:
        print(*enc_par, file=file, sep="\n")
    with open("../results/dec_seq.txt", "w") as file:
        print(*dec_seq, file=file, sep="\n")
    with open("../results/dec_par.txt", "w") as file:
        print(*dec_par, file=file, sep="\n")

if __name__ == "__main__":
    bench(5000, step=50)