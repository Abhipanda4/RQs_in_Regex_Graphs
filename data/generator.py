import numpy as np
from faker import Faker
fake = Faker()

# generate nodes
def generate_nodes(N, M):
    '''
    Each node has 4 attributes:
        -> Year of Birth(Integer)
        -> Name(String)
        -> Gender(M/F)
        -> Unique ID(String)
    '''
    birth_years = [i for i in range(1960, 2002)]
    genders = ["M", "F"]
    colors = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j']
    graph_nodes = []
    graph_edges = []
    for i in range(N):
        year_of_birth = np.random.choice(birth_years)
        gender = np.random.choice(genders)
        if gender == "M":
            name = fake.first_name_male()
        else:
            name = fake.first_name_female()
        new_node = (year_of_birth, name, gender, i + 1)
        graph_nodes.append(new_node)

    for i in range(M):
        node = np.random.randint(1, N + 1, 2)
        color = np.random.choice(colors)
        new_edge = (node[0], node[1], color)
        graph_edges.append(new_edge)

    return graph_nodes, graph_edges

def main():
    N = 1000
    M = 20000
    V, E = generate_nodes(N, M)
    print(N, M)
    for i in range(len(V)):
        print(V[i][0], V[i][1], V[i][2], V[i][3])
    for i in range(len(E)):
        print(E[i][0], E[i][1], E[i][2])

if __name__ == "__main__":
    main()
