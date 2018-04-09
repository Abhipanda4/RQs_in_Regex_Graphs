import numpy as np
import matplotlib.pyplot as plt

file1 = open("out1.txt")
file2 = open("out2.txt")

def build_freq_table(fp):
    freq = dict()
    for line in fp:
        nodes = line.split(",")
        u = int(nodes[0][1:])
        v = int(nodes[1][1:-2])
        if u in freq.keys():
            freq[u] += 1
        else:
            freq[u] = 0
    return freq

t1 = build_freq_table(file1)
t2 = build_freq_table(file2)

for i in t1.keys():
    if t1[i] != t2[i]:
        print(str(i) + ": " + str(t1[i]) + " *** " + str(t2[i]))
