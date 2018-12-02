import csv
import matplotlib.pyplot as plt
import numpy as np

def index_sizes():
    fp = open("./index_size.csv")
    x = csv.reader(fp, delimiter='\t')
    sizes = []
    for line in x:
        size = float(line[0].strip()[:-1])
        sizes.append(size)
    temp = sorted(sizes[:-1])
    nodes = [i for i in range(100, 5001, 100)]
    f = plt.figure()
    plt.xlabel("Number of node-color pairs stored")
    plt.ylabel("Size of partial index(in MBs)")
    plt.plot(nodes, temp)
    plt.show()
    f.savefig("../figures/index_sizes.pdf", bbox_inches='tight')


def MC_size_dependence():
    fp = open("./MC_analysis.csv", 'r')
    x = csv.reader(fp)
    times = [int(i[1]) for i in x]

    p = []
    for i in range(0, len(times), 100):
        p.append(np.mean(times[i:i + 100]))

    parts = [i for i in range(10, 501, 10)]
    print(p)
    plt.plot(parts, p)
    plt.xlabel("Number of pairs stored")
    plt.ylabel("Average time in ms")
    plt.title("Variation of query times with size of index stored")
    plt.show()

def pre_process_time():
    fp1 = open("./TC_pre_pro.txt", 'r')
    fp2 = open("./MC_pre_pro.txt", 'r')

    nodes = [i for i in range(10, 21, 2)]
    f1 = csv.reader(fp1)
    f2 = csv.reader(fp2)

    p2 = [[]] * 5
    p1 = [int(i[0]) for i in f1]

    p2_temp = [int(i[0]) for i in f2]
    print(p2_temp)
    for i in range(5):
        p2[i] = [p2_temp[j] / 20 for j in range(i, len(p2_temp), 5)]

    f = plt.figure()
    plt.plot(nodes, p1, label="Complete transitive closure")
    for i in range(5):
        plt.plot(nodes, p2[i], label=str((i + 1) * 10) + "% pairs computed")
    plt.xlabel("|V|(in thousands)")
    plt.ylabel("Time taken to build table(in ms)")
    plt.title("Comparision of pre-processing times for Algorithm 1 & 3")
    plt.legend()
    plt.show()
    f.savefig("../figures/pre_pro_10iter_MC.pdf", bbox_inches='tight')

def hits_vs_miss():
    fp = csv.reader(open("./MC_size_dependence.csv", 'r'))
    tmp = list(fp)
    lines = [i for i in tmp if i[0].find("Time") == -1]
    T = []
    H = []
    M = []
    for i in range(0, len(lines), 5):
        times = [int(lines[i + j][1]) for j in range(5)]
        T.append(sum(times) / 5)
        hits = [int(lines[i + j][2]) for j in range(5)]
        H.append(sum(hits) / 5)
        misses = [int(lines[i + j][3]) for j in range(5)]
        M.append(sum(misses) / 5)

    # print(T)
    # print(H)
    # print(M)
    X = [6000, 12000, 18000, 24000, 30000]
    plt.subplot(1, 2, 1)
    plt.plot(X, H, label="Hits")
    plt.plot(X, M, label="Misses")
    plt.xlabel("Number of node-color pairs stored")
    plt.ylabel("Number of hits/misses")
    plt.legend()
    plt.subplot(1, 2, 2)
    plt.plot(X, T, label="Query Times")
    plt.xlabel("Number of node-color pairs stored")
    plt.ylabel("Time in ms")
    plt.title("Query times")
    plt.show()

def f(r):
    timings = []
    x = []
    count = 0
    for line in r:
        x.append(int(line[1]))
        count += 1
        if count == 20:
            timings.append(sum(x) / 20)
            x = []
            count = 0
    return timings



def edge_size():
    f1 = open("./data_algo1.csv", 'r')
    f2 = open("./data_algo2.csv", 'r')
    f3 = open("./data_algo3.csv", 'r')

    r1 = csv.reader(f1)
    r2 = csv.reader(f2)
    r3 = csv.reader(f3)

    x1 = f(r1)
    x2 = f(r2)
    x3 = f(r3)

    x1[3] = x1[3] / 100

    print(x1)
    print(x2)
    print(x3)

    x = [2, 3, 4, 5, 6]

    fig = plt.figure()
    plt.plot(x, x1, label="Full Transitive Closure")
    plt.plot(x, x2, label="Partial Transitive Closure")
    plt.plot(x[:3], x3[:3], label="BFS")
    plt.legend()
    plt.xlabel("Edge-Node Ratio")
    plt.ylabel("Average Time taken for query in milliseconds")
    plt.show()
    fig.savefig("../figures/edge_variation.pdf")





# pre_process_time()
# hits_vs_miss()
# MC_size_dependence()
# edge_size()
index_sizes()
