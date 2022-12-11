#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif

#include "catch.hpp"
#include <string>
#include <map>
#include <vector>
#include <wikigraph.hpp>
#include <time.h>
#include <iostream>

#include "utilities.hpp"

TEST_CASE("Intake test", "[constructor]") {
  // python >:)
  std::map<std::string, std::vector<std::string>> expected = 
  {
    {"A", {"B", "C"}},
    {"B", {}},
    {"C", {"D"}},
    {"D", {}},
    {"this has spaces", {"B"}},
    {"\"article\"", {"D"}},
  };

  WikiGraph w("./datasets/input_test.tsv");
  REQUIRE(expected == w.getMap());
}

// ------------- BFS Test Cases -------------

TEST_CASE("BFS simple", "[BFS]") {
  std::vector<std::string> expectedAE = {
    "A", "B", "C", "D", "E"
  };
  std::vector<std::string> expectedCD = {
    "C", "D"
  };
  WikiGraph w("./datasets/line_graph.tsv");
  REQUIRE(expectedAE == w.getPathBFS("A", "E"));
  REQUIRE(expectedCD == w.getPathBFS("C", "D"));
}

TEST_CASE("BFS picks the best path", "BFS") {
  std::vector<std::string> expectedAE = {
    "A", "E"
  };
  std::vector<std::string> expectedAD = {
    "A", "B", "D"
  };
  WikiGraph w("./datasets/diff_paths.tsv");
  REQUIRE(expectedAE == w.getPathBFS("A", "E"));
  REQUIRE(expectedAD == w.getPathBFS("A", "D"));
}

TEST_CASE("BFS handles cycles", "[BFS]") {
  std::vector<std::string> expectedAE = {
    "A", "E"
  };
  std::vector<std::string> expectedAD = {
    "A", "B", "D"
  };
  WikiGraph w("./datasets/cycle.tsv");
  REQUIRE(expectedAE == w.getPathBFS("A", "E"));
  REQUIRE(expectedAD == w.getPathBFS("A", "D"));
}

// ------------- Brandes Test Cases -------------

TEST_CASE("Page Centrality for two middle Nodes", "[Brandes]") {
  /* A -> B
     |    |
     v    v
     C -> D
  */
  WikiGraph w{"./datasets/centrality/basic_two_cent_nodes.tsv"};
  REQUIRE(w.getBetweenCentrality("A") == 0.0);
  REQUIRE(w.getBetweenCentrality("D") == 0.0);
  REQUIRE(w.getBetweenCentrality("B") == w.getBetweenCentrality("C"));
  REQUIRE(w.getBetweenCentrality("B") > 0.0); // isn't the only shortest path!
  REQUIRE(w.getBetweenCentrality("B") < 1.0); // isn't the only shortest path!
  REQUIRE(w.getBetweenCentrality("C") > 0.0); // isn't the only shortest path!
  REQUIRE(w.getBetweenCentrality("C") < 1.0); // isn't the only shortest path!
}

TEST_CASE("Page centrality no central nodes", "[Brandes]") {
  WikiGraph w{"./datasets/centrality/no_central_node.tsv"}; 
  REQUIRE(w.getBetweenCentrality("A") == 0.0);
  REQUIRE(w.getBetweenCentrality("B") == 0.0);
  REQUIRE(w.getBetweenCentrality("C") == 0.0);
  REQUIRE(w.getBetweenCentrality("D") == 0.0);
}

TEST_CASE("Page centrality single central nodes", "[Brandes]") {
  WikiGraph w{"./datasets/centrality/single_central_node.tsv"}; 
  REQUIRE(w.getBetweenCentrality("A") == 0.0);
  REQUIRE(w.getBetweenCentrality("B") == 0.0);
  REQUIRE(w.getBetweenCentrality("C") == 0.0);
  REQUIRE(w.getBetweenCentrality("D") > 1.0); // is central for many paths.
  REQUIRE(w.getBetweenCentrality("E") == 0.0);
  REQUIRE(w.getBetweenCentrality("F") == 0.0);
  REQUIRE(w.getBetweenCentrality("G") == 0.0);
}

TEST_CASE("Page centrality two central equal central nodes", "[Brandes]") {
  WikiGraph w{"./datasets/centrality/two_eq_cent_nodes.tsv"}; 
  REQUIRE(w.getBetweenCentrality("A") == 0.0);
  REQUIRE(w.getBetweenCentrality("B") == 0.0);
  REQUIRE(w.getBetweenCentrality("C") == 0.0);
  REQUIRE(w.getBetweenCentrality("D") > 1.0); // is central for many paths.
  REQUIRE(w.getBetweenCentrality("E") == 0.0);
  REQUIRE(w.getBetweenCentrality("F") == 0.0);
  REQUIRE(w.getBetweenCentrality("G") == 0.0);
  REQUIRE(w.getBetweenCentrality("H") > 1.0); // is central for many paths.
  REQUIRE(w.getBetweenCentrality("D") == w.getBetweenCentrality("H"));
}

TEST_CASE("Pagerank simple", "prk") {
  WikiGraph w("datasets/pagerank/easypagerank.tsv");
  auto ranked = w.rankPages();
  for (const auto& rank : ranked) {
    std::cout << rank.title << "    " << rank.rank << std::endl;
  }
}

// ------------- Page Rank Test Cases -------------

