# Script to generate a regex graph froma normal
# directed graph generated using PaRMAT
import argparse
import numpy as np
from faker import Faker

def read_lines(filename):
    '''
    Given a tab delimited file which represents the nodes
    which are connected, this function processes each line
    and adds to the list of edges.
    Each element in the list is a tuple of the form (u, v)
    '''
    graph_edges = []
    try:
        fp = open(filename, 'r')
        for line in fp:
            nodes = line.split(",")
            graph_edges.append((int(nodes[0]), int(nodes[1])))
    finally:
        fp.close()

    return graph_edges


def color_distribution(n_edges, n_colors):
    '''
    Decide the color for each edge
    '''
    color_distribution = []
    for i in range(n_edges):
        color_distribution.append(np.random.randint(0, n_colors))
    return color_distribution


def create_nodes(n_vertices):
    '''
    Generate node predicates
    Currently, there are 6 predicates
        -> Year of Birth(Integer)
        -> Gender(M/F)
        -> Name(String)
        -> Num Posts(int)
        -> Num Friends(int)
        -> Unique ID(int)
    '''
    fake = Faker()

    birth_years = [i for i in range(1980, 2015)]
    genders = ["M", "F"]
    num_posts = list(range(30, 400))
    num_friends = list(range(50, 1000))

    graph_nodes = []

    for i in range(n_vertices):
        year_of_birth = np.random.choice(birth_years)
        gender = np.random.choice(genders)
        if gender == "M":
            name = fake.first_name_male()
        else:
            name = fake.first_name_female()
        posts = np.random.choice(num_posts)
        friends = np.random.choice(num_friends)

        new_node = (year_of_birth, name, gender, posts, friends, i + 1)
        graph_nodes.append(new_node)

    return graph_nodes

def create_edges(filename, n_colors):
    edges = read_lines(filename)
    n_edges = len(edges)
    edge_colors = color_distribution(n_edges, n_colors)
    graph_edges = []
    for edge, color in zip(edges, edge_colors):
        graph_edges.append((edge[0], edge[1], color))
    return graph_edges

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--inpfile", type=str)
    parser.add_argument("--nVertices", type=int)
    parser.add_argument("--nColors", type=str, default=5)

    args = parser.parse_args()
    graph_nodes = create_nodes(args.nVertices)
    graph_edges = create_edges(args.inpfile, args.nColors)

    # print the nodes and edges
    print(len(graph_nodes), len(graph_edges), args.nColors)
    for node in graph_nodes:
        for feature in node:
            print(feature, " ", end="")
        print()
    for edge in graph_edges:
        print(edge[0], edge[1], chr(ord('a') + edge[2]))

if __name__ == "__main__":
    main()

