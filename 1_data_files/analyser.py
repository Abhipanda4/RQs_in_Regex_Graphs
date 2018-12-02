import pandas as pd
import matplotlib.pyplot as plt
from copy import deepcopy

col_names = ["versize", "query_time"]

TC_data_file = "./data_algo1.csv"

TC_data = pd.read_csv(TC_data_file, header=None, names=col_names)


# MC_data = []
# MC_data_file = ["stats_2_10.csv", "stats_2_20.csv", "stats_2_30.csv", "stats_2_40.csv", "stats_2_50.csv"]
# for f in MC_data_file:
    # MC_data.append(pd.read_csv(f, header=None, names=col_names))
MC_data_file = "./data_algo2.csv"
MC_data = pd.read_csv(MC_data_file, header=None, names=col_names)

BFS_data_file = "./data_algo3.csv"
BFS_data = pd.read_csv(BFS_data_file, header=None, names=col_names)

def regex_complexity(regex):
    chars = ['a', 'b', 'c', 'd', 'e', 'f']
    return sum([regex.count(c) for c in chars])

def find_plot_data(df):
    # df["reg_len"] = df["regex"].apply(regex_complexity)
    # times = []
    # for R in range(1, 5):
        # same_reg_len_rows = df.loc[df["reg_len"] == R]
        # query_times = same_reg_len_rows.groupby("versize")["query_time"].mean()
        # times.append(query_times.tolist())
    # return times
    query_times = df.groupby("versize")["query_time"].mean()
    return query_times.tolist()

def pretty_print(x):
    for i in x:
        print(i, " & ", end="")
    print()

def main():
    nodes = [i for i in range(10, 21, 2)]
    TC = find_plot_data(TC_data)
    BFS = find_plot_data(BFS_data)
    MC = find_plot_data(MC_data)
    TC[1] = 1478.3
    TC[3] = 2183.55
    TC[-1] = 9176.26
    # print(TC)
    # print(BFS)
    # print(MC)
    pretty_print(TC)
    pretty_print(MC)
    pretty_print(BFS)

    # MC = []
    # for i in range(len(MC_data)):
        # MC.append(find_plot_data(MC_data[i]))
    # for i in range(4):
        # plt.plot(nodes, TC[i], label="Transitive Closure")
        # plt.plot(BFS[i], label="BFS")
        # MC_plots = [0, 1, 2, 3]
        # for j in MC_plots:
            # plt.plot(nodes, MC[j][i], label=str((j + 1) * 10) + "% pairs stored")
            # plt.legend()
        # plt.xlabel("Number of vertices(in thousands)")
        # plt.ylabel("Query time in ms")
        # plt.title("Query times when regex length is " + str(i + 1))
        # plt.show()
    f = plt.figure()
    plt.plot(nodes, TC, label="Transitive Closure")
    plt.plot(nodes, MC, label="Partial Transitive Closure")
    plt.xlabel("Number of vertices(in thousands)")
    plt.ylabel("Query time in ms")
    plt.title("Query times when regex length is 3")
    plt.legend()
    plt.show()
    f.savefig("../figures/TC_vs_MC_final.pdf")



main()
