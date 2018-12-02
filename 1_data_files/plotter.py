import csv
import matplotlib.pyplot as plt
import numpy as np

def MC_size_dependence():
    f = open("./size_dep.csv", 'r')
    x = csv.reader(f)
    size_MC = []
    times = []
    for i in x:
        size_MC.append(int(i[0]))
        times.append(int(i[1]))
    fig = plt.figure()
    plt.plot(size_MC, times)
    plt.xlabel("Number of pairs stored")
    plt.ylabel("Average query time in ms")
    fig.savefig("../figures/MC_size_dependence.pdf")

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
    fp = csv.reader(open("./hits_and_miss.csv", 'r'))
    g_hits = []
    g_miss = []
    nodes = []
    hits = []
    misses = []
    for i, line in enumerate(fp):
        hits.append(int(line[2]))
        misses.append(int(line[3]))
        if (i + 1) % 5 == 0:
            nodes.append(int(line[0]))
            g_hits.append(np.mean(hits)/1000)
            g_miss.append(np.mean(misses)/1000)
            hits = []
            misses = []
    fig = plt.figure()
    plt.plot(nodes, g_hits)
    plt.xlabel("Number of pairs stored")
    plt.ylabel("Number of hits(in thousands)")
    plt.show()
    fig.savefig("../figures/hits_vs_misses.pdf")


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
hits_vs_miss()
# MC_size_dependence()
# edge_size()
