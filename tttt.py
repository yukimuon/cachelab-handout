import numpy as np

ff = open("output", "r")

lines = ff.readlines()

M = int(lines[0].split("M=")[1].split(",")[0])
N = int(lines[0].split("N=")[1].split(",")[0])
P = int(lines[0].split("P=")[1].split(",")[0])

a = ""
for line in lines[3:(3+M)]:
    a += line
a = np.array([int(x) for x in a.split()]).reshape(M, P)

b = ""
for line in lines[(4+M):(4+M+P)]:
    b += line
b = np.array([int(x) for x in b.split()]).reshape(P, N)

c = ""
for line in lines[(5+M+P):]:
    c += line
c = np.array([int(x) for x in c.split()]).reshape(M, N)

print(np.dot(a, b) - c)

ff.close()