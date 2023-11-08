#include <boost/container_hash/hash_fwd.hpp>
#include <catch2/catch_all.hpp>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/utility.hpp> // boost::tie

#include <algorithm>
#include <array>
#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <utility>
#include <vector>

TEST_CASE("Constructing a Graph", "[BoostGraphLibrary]") {
    // create a typedef for the Graph type
    using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS>;

    // Make convenient labels for the vertices
    enum {
        A,
        B,
        C,
        D,
        E,
        N
    };
    const int num_vertices = N;

    // writing out the edges in the graph
    using Edge = std::pair<int, int>;
    Edge edge_array[] = {Edge(A, B), Edge(A, D), Edge(C, A), Edge(D, C), Edge(C, E), Edge(B, D), Edge(D, E)};
    const int num_edges = sizeof(edge_array) / sizeof(edge_array[0]);

    // declare a graph object
    Graph g(num_vertices);

    // add the edges to the graph object
    for (int i = 0; i < num_edges; ++i) {
        boost::add_edge(edge_array[i].first, edge_array[i].second, g);
    }

    using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

    // get the property map for vertex indices
    using IndexMap = boost::property_map<Graph, boost::vertex_index_t>::type;
    IndexMap index = boost::get(boost::vertex_index, g);

    std::cout << "vertices(g) = ";
    using vertex_iter = boost::graph_traits<Graph>::vertex_iterator;
    std::pair<vertex_iter, vertex_iter> vp;
    for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
        Vertex v = *vp.first;
        std::cout << index[v] << " ";
    }
    std::cout << std::endl;

    std::cout << "edges(g) = ";
    boost::graph_traits<Graph>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
        std::cout << "(" << index[boost::source(*ei, g)]
                  << "," << index[boost::target(*ei, g)] << ") ";
    std::cout << std::endl;

    return;
}

TEST_CASE("Adding Some Color to your Graph", "[BoostGraphLibrary]") {
    using Graph = boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_weight_t, int>>;
    using Vertex = boost::graph_traits<Graph>::vertex_descriptor;
    using E = std::pair<int, int>;

    const int num_nodes = 5;
    auto edges = std::array{
        E(0, 2),
        E(1, 1),
        E(1, 3),
        E(1, 4),
        E(2, 1),
        E(2, 3),
        E(3, 4),
        E(4, 0),
        E(4, 1),
    };
    auto weights = std::array{1, 2, 1, 2, 7, 3, 1, 1, 1};

    Graph G(edges.begin(), edges.end(), weights.begin(), num_nodes);

    using IndexMap = boost::property_map<Graph, boost::vertex_index_t>::type;
    IndexMap index = boost::get(boost::vertex_index, G);

    // vector for storing distance property
    std::vector<int> d(num_vertices(G));

    // get the first vertex
    Vertex s = *(vertices(G).first);
    // invoke variant 2 of Dijkstra's algorithm
    dijkstra_shortest_paths(G, s, boost::distance_map(&d[0]));

    std::cout << "distances from start vertex:" << std::endl;
    boost::graph_traits<Graph>::vertex_iterator vi;
    for (vi = vertices(G).first; vi != vertices(G).second; ++vi)
        std::cout << "distance(" << index(*vi) << ") = "
                  << d[*vi] << std::endl;
    std::cout << std::endl;
    return;
}

TEST_CASE("create my use case", "[BoostGraphLibrary]") {
    using Graph = boost::adjacency_list<boost::listS, boost::listS, boost::bidirectionalS>;
    using Vertex = boost::graph_traits<Graph>::vertex_descriptor;
    using Edge = boost::graph_traits<Graph>::edge_descriptor;
    using EdgeIndicesPair = std::pair<int, int>;
    struct EdgeIndicesPairHash {
        std::size_t operator()(const EdgeIndicesPair & p) const {
            auto seed = std::size_t(31);
            boost::hash_combine(seed, p.first);
            boost::hash_combine(seed, p.second);
            return seed;
        }
    };

    auto vertices = std::vector<int>{96, 44, 0, 91, 93, 58, 67, 8, 76, 23};
    auto edges = std::vector<EdgeIndicesPair>{{96, 91}, {93, 76}, {44, 0}, {44, 58}, {67, 0}, {91, 58}, {96, 0}, {67, 44}, {44, 76}, {8, 76}};

    Graph g;
    auto vertex_map = std::unordered_map<int, Vertex>{};
    auto edge_map = std::unordered_map<EdgeIndicesPair, Edge, EdgeIndicesPairHash>{};
    for (const auto & v : vertices) {
        vertex_map[v] = boost::add_vertex(g);
    }
    for (const auto & e : edges) {
        if (vertex_map.find(e.first) == vertex_map.end() || vertex_map.find(e.second) == vertex_map.end()) {
            throw std::runtime_error("vertex " + std::to_string(e.first) + " or " + std::to_string(e.second) + "not found");
        }
        auto ret = boost::add_edge(vertex_map[e.first], vertex_map[e.second], g);
        if (ret.second) {
            edge_map[e] = ret.first;
        }
    }
}
