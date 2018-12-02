# Fix number of node predicates at 1 out of 6
# this ensures queries with larger space of possible nodes
# The number of colors in a query is varied from 1 to 5

import argparse
import numpy as np
from graph_gen import birth_years, genders, num_posts, num_friends

possibilities = [1, 3, 4, 5]
# do not consider equaliy as it will narrow down
# extreme node sets too much
op1 = ["<=", "<", ">", ">="]
op2 = ["==", "!="]
def construct_predicate():
    # attribute 2 & 6 should never be considered
    orig_query = ["__"] * 6
    # num_preds = np.random.choice([0, 1, 2, 3])
    num_preds = 3
    selected_predicates = np.random.choice(possibilities, num_preds, replace=False)
    for predicate in selected_predicates:
        query = ""
        if predicate == 1:
            # birth_year
            op_value = np.random.choice(birth_years)
            op = np.random.choice(op1)
            query += str(op)
            query += str(op_value)

        elif predicate == 3:
            # sex
            op_value = np.random.choice(genders)
            op = np.random.choice(op2)
            query += str(op)
            query += str(op_value)

        elif predicate == 4:
            # posts
            op_value = np.random.choice(num_posts)
            op = np.random.choice(op1)
            query += str(op)
            query += str(op_value)

        elif predicate == 5:
            # friends
            op_value = np.random.choice(num_friends)
            op = np.random.choice(op1)
            query += str(op)
            query += str(op_value)

        orig_query[predicate - 1] = query

    return "".join(orig_query)


def construct_regex():
    colors = ['a', 'b', 'c', 'd', 'e', 'f']
    # some parameters to fix for queries
    max_colors = 4
    max_len = 8
    ops = ["", "<="]
    # reg_len = np.random.randint(1, max_colors + 1)
    reg_len = 3
    np.random.shuffle(colors)
    regex = ""
    for color in colors[:reg_len]:
        regex += color
        op = np.random.choice(ops)
        if op != "<=":
            regex += op
        else:
            regex += op
            regex += str(np.random.randint(2, max_len + 1))

    return regex

def construct_query():
    pred1 = construct_predicate()
    pred2 = construct_predicate()
    regex = construct_regex()
    return pred1 + " " + pred2 + " " + regex

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--Q", type=int, default=10)
    args = parser.parse_args()

    print(args.Q)
    for i in range(args.Q):
        print(construct_query())

if __name__ == "__main__":
    main()
