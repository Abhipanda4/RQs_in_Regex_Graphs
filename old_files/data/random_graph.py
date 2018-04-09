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
        -> Num Posts(int)
        -> Num Friends(int)
        -> Unique ID(int)
    '''
    birth_years = [i for i in range(1980, 2015)]
    genders = ["M", "F"]
    colors = ['a', 'b', 'c', 'd', 'e', 'f', 'g']
    num_posts = list(range(30, 400))
    num_friends = list(range(50, 1000))
    graph_nodes = []
    graph_edges = []
    for i in range(N):
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

    present_nodes = []
    i = 0
    while i < M:
        node1 = np.random.randint(1, N + 1)
        node2 = np.random.randint(1, N + 1)
        if (node1, node2) in present_nodes:
            continue
        if node1 == node2:
            continue
        color = np.random.choice(colors)
        present_nodes.append((node1, node2))
        new_edge = (node1, node2, color)
        graph_edges.append(new_edge)
        i += 1

    return len(colors), graph_nodes, graph_edges

def main():
    N = 8000
    M = 40000
    C, V, E = generate_nodes(N, M)
    print(N, M, C)
    for i in range(len(V)):
        print(V[i][0], V[i][1], V[i][2], V[i][3], V[i][4], V[i][5])
    for i in range(len(E)):
        print(E[i][0], E[i][1], E[i][2])

if __name__ == "__main__":
    main()
