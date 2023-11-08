#include <catch2/catch_all.hpp>

#include <random>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>
#include <unordered_map>

#include "boost/container_hash/hash.hpp"

#include "lemon/core.h"
#include "lemon/list_graph.h"

struct RandomGenerator {
    static constexpr int RANDOM_SEED = 1234;
    std::default_random_engine random_engine;

    RandomGenerator()
        : random_engine(RANDOM_SEED) {}

    auto generate_vertices(const int & low, const int & high, const std::size_t & count) -> std::vector<int> {
        std::uniform_int_distribution<int> uniform_distribution(low, high);
        auto vertices = std::set<int>{};
        if (high <= low) {
            throw std::runtime_error("invalid range for high and low");
        }
        if (static_cast<std::size_t>(high - low) < count) {
            throw std::runtime_error("too many vertices to generate");
        }
        while (vertices.size() < count) {
            vertices.insert(uniform_distribution(random_engine));
        }
        return std::vector<int>(vertices.begin(), vertices.end());
    }

    auto generate_edges(const std::vector<int> & vertices, const std::size_t & count) -> std::set<std::pair<int, int>> {
        std::uniform_int_distribution<int> uniform_distribution(0, vertices.size() - 1);
        auto edges = std::set<std::pair<int, int>>{};
        if (vertices.size() * vertices.size() < count) {
            throw std::runtime_error("too many edges to generate");
        }
        while (edges.size() < count) {
            auto first = uniform_distribution(random_engine);
            auto second = first;
            while (first == second) {
                second = uniform_distribution(random_engine);
            }
            edges.insert({vertices[first], vertices[second]});
        }
        return edges;
    }
};

using EdgeIndicesPair = std::pair<int, int>;
struct EdgeIndicesPairHash {
    std::size_t operator()(const EdgeIndicesPair & p) const {
        auto seed = std::size_t(31);
        boost::hash_combine(seed, p.first);
        boost::hash_combine(seed, p.second);
        return seed;
    }
};

TEST_CASE("Lemon example", "[Lemon]") {
    // vertices and edges
    auto random_generator = RandomGenerator();
    const int ROUNDS = 3;
    const auto vertices_range = std::vector<std::pair<int, int>>{
        {0, 10000},
        {10001, 20000},
        {20001, 30000},
    };
    const auto vertices_size = std::vector<int>{
        1000,
        1000,
        1000,
    };
    const auto edges_size = std::vector<int>{
        1000,
        1000,
        1000,
    };
    auto vertices = std::vector<std::vector<int>>();
    auto edges = std::vector<std::set<std::pair<int, int>>>();
    for (int i = 0; i < ROUNDS; ++i) {
        vertices.push_back(
            random_generator.generate_vertices(
                vertices_range[i].first,
                vertices_range[i].second,
                vertices_size[i]));
        edges.push_back(
            random_generator.generate_edges(
                vertices[i],
                edges_size[i]));
    }

    BENCHMARK("make graph") {
        lemon::ListGraph g;
        auto vertex_map = std::unordered_map<int, lemon::ListGraph::Node>{};
        auto edge_map = std::unordered_map<EdgeIndicesPair, lemon::ListGraph::Edge, EdgeIndicesPairHash>{};
        for (int i = 0; i < ROUNDS; ++i) {
            //// add vertices - guaranteed to be unique
            for (const auto & v : vertices[i]) {
                vertex_map[v] = g.addNode();
            }
            //// add edges - guaranteed to be unique and valid
            for (const auto & e : edges[i]) {
                edge_map[e] = g.addEdge(vertex_map[e.first], vertex_map[e.second]);
            }
            // remove all vertices
            for (const auto & v : vertices[i]) {
                g.erase(vertex_map[v]);
            }
        }
    };
}
