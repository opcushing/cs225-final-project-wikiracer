#include "wikigraph.hpp"

#include <fstream>
#include <queue>

#include "utilities.hpp"

// TODO: Construct from File
WikiGraph::WikiGraph(const std::string& file_name) {
  // Reads in file stream.
  // Populates map.

  // open file
  // get a line; decode the first item, decode the second item, add the second to adjlist of firsts
  std::string file = file_to_string(file_name);
  std::vector<std::string> lines;
  SplitString(file, '\n', lines);
  // our dataset has a line of empty space that we can discard
  if (lines.back().empty())
    lines.pop_back(); 

  std::map<std::string, std::string> decoded; // memoize decoding
  for (auto& line : lines) {
    std::vector<std::string> pages;
    SplitString(line, '\t', pages);

    // decode if necessary
    if (decoded.find(pages[0]) == decoded.end()) {
      decoded[pages[0]] = DecodeURL(pages[0]);
    }
    if (decoded.find(pages[1]) == decoded.end()) {
      decoded[pages[1]] = DecodeURL(pages[1]);
    }

    auto src = decoded[pages[0]];
    auto dest = decoded[pages[1]];

    article_map[src].push_back(dest);
    if (article_map.find(dest) == article_map.end()) {
      // create a vertex for the destination (constructs an empty list)
      article_map[dest];
    }
  }
}

// --------- Algorithms --------------
// DONE: getPathBFS()
std::vector<std::string> WikiGraph::getPathBFS(
    const std::string& start_page, const std::string& end_page) const {

  if (!validStartAndEnd(start_page, end_page)) {
    throw std::invalid_argument("These pages are not in the graph");
  }

  std::vector<std::string> page_path;

  std::map<std::string, std::string> page_tree;
  std::queue<std::string> to_visit;

  std::string curr_page;
  to_visit.push(start_page);

  while (!to_visit.empty() && curr_page != end_page) {
    curr_page = to_visit.front();
    to_visit.pop();
    if (article_map.find(curr_page) != article_map.end()) {
      std::vector<std::string> adjacent_pages = article_map.at(curr_page);
      for (const auto& adj_page : adjacent_pages) {
        // if page hasn't been visited.
        if (page_tree.count(adj_page) != 1) {
          to_visit.push(adj_page);
          page_tree[adj_page] = curr_page;
        }
        if (adj_page == end_page) {
          break; // path to page was found.
        }
      }
    }
  }

  // Path between pages was not found...
  if (page_tree.count(end_page) == 0) {
    throw std::runtime_error("Path to page not found");
  }

  std::string path_page = end_page;
  while (path_page != start_page) {
    page_path.push_back(path_page);
    path_page = page_tree.at(path_page);
  }
  page_path.push_back(start_page);

  return {page_path.rbegin(), page_path.rend()};
}

// DONE: getPathDijkstras()
/**
 * @deprecated
*/
std::vector<std::string> WikiGraph::getPathDijkstras(const std::string& start_page, const std::string& end_page) const {

  // require start and end exist
  if (!validStartAndEnd(start_page, end_page)) {
    throw std::invalid_argument("One or more of these pages are not in the graph");
  }

  auto pages = getPages();

 // init distance and predecessor
 std::map<std::string, int> distance;
 std::map<std::string, std::string> predecessor;
 for (const auto& page: pages) {
  distance[page] = INT_MAX;
  predecessor[page] = "";
 }
 distance[start_page] = 0;

 // priority is minimum distance, represented by this lambda expression
 auto priority = [&distance](std::string a, std::string b) {
    return distance.at(a) > distance.at(b);
  };

 // construct the priority queue based on the pages (keys in article_map)
 std::priority_queue<std::string, std::vector<std::string>, decltype(priority)> q{priority};
 q.push(start_page);

  // go through the edges
  while (!q.empty()) {
    std::string curr = q.top();
    q.pop();
    for (const auto& neighbor : article_map.at(curr)) {
      if (1 + distance[curr] < distance[neighbor]) {
        distance[neighbor] = 1 + distance[curr];
        predecessor[neighbor] = curr;
        q.push(neighbor);
      }
    }
  }

  // construct the path using the predecessor map
  std::vector<std::string> path;
  path.push_back(end_page);

  while (path.back() != start_page && predecessor[path.back()] != "") {
    path.push_back(predecessor[path.back()]);
  }

  // there is no path from the start to the end
  if (std::find(path.begin(), path.end(), start_page) == path.end()) {
    throw std::runtime_error("There is no path between these articles");
  }
  
  // return the reversed version (start -> end)
  return {path.rbegin(), path.rend()};
}

// TODO: rankPages()
std::vector<WikiGraph::RankedPage> WikiGraph::rankPages() const {
  std::vector<RankedPage> ranked_pages;
  // ...
  return ranked_pages;
}

// TODO: getBetweenCentrality()
double WikiGraph::getBetweenCentrality(const std::string& page) const {
  // Note: will possibly be memoized, either from this function or the other.
  std::map<std::string, double> centralityMap = getCentralityMap();
  if (centralityMap.find(page) != centralityMap.end()) {
    return centralityMap.at(page);
  }
  return -1.0; // INVALID VALUE.
}

// TODO: getCentralityMap()
std::map<std::string, double> WikiGraph::getCentralityMap() const {
  /* PSEUDOCODE as taken from Ulrik Brandes original paper:
  https://snap.stanford.edu/class/cs224w-readings/brandes01centrality.pdf#page=10
  
  C_B[v] = 0, for v in V
  for s in V do
    S: empty stack
    P[w] = empty list, for w in V
    sigma[t] = 0, t in V; sigma[s] = 1;
    d[t] = -1, for t in V; d[s] = 0;
    Q: empty queue
    Q enqueue s
    while Q is not empty:
      dequeue v from Q;
      push v on S
      for adjacent_page w of v:
        if d[w] < 0 then:
          enqueue w on Q
          d[w] = d[v] + 1;
        if d[w] == d[v] + 1
          sigma[w] = sigma[w] + sigma[v];
          P[w] append v
    delta[v] = 0, for v in V
    // S return vertices in order of non-increasing distance from s
    while S is not empty:
      w = S.pop();
      for v in P[w]:
        delta[v] += ( sigma[v] / sigma[w] ) * (1 + delta[w]);
      if w != s:
        C_B[w] = C_B[w] + delta[W];
  */
  return std::map<std::string, double>(); // Default value for now
}

//---------- Helper Methods ----------

bool WikiGraph::validStartAndEnd(const std::string& start_page, const std::string& end_page) const {
  return (article_map.find(start_page) != article_map.end()) && (article_map.find(end_page) != article_map.end());
}

std::vector<std::string> WikiGraph::getPages() const {
  // extract keys from map
  std::vector<std::string> pages;
  std::transform(article_map.begin(), article_map.end(), std::back_inserter(pages),
      [](decltype(article_map)::value_type const &kv) {
          return kv.first;
      });

  return pages;
}