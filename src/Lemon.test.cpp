#include <catch2/catch_all.hpp>

#include <iostream>

#include <lemon/list_graph.h>

TEST_CASE("Lemon example", "[Lemon]") {
    lemon::ListDigraph g;
    lemon::ListDigraph::Node u = g.addNode();
    lemon::ListDigraph::Node v = g.addNode();
    /*lemon::ListDigraph::Arc a = */g.addArc(u, v);
    CHECK(lemon::countNodes(g) == 2);
    CHECK(lemon::countArcs(g) == 1);
}
