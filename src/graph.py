import itertools
import random

import networkx as nx
import matplotlib.pyplot as plt

def get_nodes(low, high, count = 10):
    return list(random.choices(range(low, high), k=count))

def get_edges(nodes, count = 10):
    all_possible_edges = list(itertools.product(nodes, repeat=2))
    all_valid_edges = list(filter(lambda e: e[0] != e[1], all_possible_edges))
    selected_edges = list(set(random.choices(all_valid_edges, k=count)))
    return selected_edges

if __name__ == "__main__":
    print("")
    g = nx.Graph()
    nodes = []

    random.seed(1234)

    nodes = get_nodes(0, 100)
    edges = get_edges(nodes)
    print(f"set of nodes to add: " + f"{nodes}".replace("[", "{").replace("]", "}"))
    print(f"set of edges to add: " + f"{edges}".replace("[", "{").replace("]", "}").replace("(", "{").replace(")", "}"))
    print("")
    g.add_nodes_from(nodes)
    g.add_edges_from(edges)

    plt.subplot(111)
    nx.draw_shell(g, with_labels=True)
    plt.savefig("random_graph_1.png")
    plt.close()

    prev_nodes = nodes
    nodes = get_nodes(100, 200)
    edges = get_edges(nodes + prev_nodes)
    print(f"set of nodes to add: " + f"{nodes}".replace("[", "{").replace("]", "}"))
    print(f"set of edges to add: " + f"{edges}".replace("[", "{").replace("]", "}").replace("(", "{").replace(")", "}"))
    print("")
    g.add_nodes_from(nodes)
    g.add_edges_from(edges)

    plt.subplot(111)
    nx.draw_shell(g, with_labels=True)
    plt.savefig("random_graph_2.png")
    plt.close()


