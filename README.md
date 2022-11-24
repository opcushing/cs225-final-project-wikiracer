## Leading Question 

####As in the popular webgame [Wikiracer](https://www.thewikigame.com/group), can we find the shortest way to get from one Wikipedia article just by links to other articles?

## Dataset Acquisition

We will be using the following dataset provided by Standford University:   
[wikispeedia_paths-and-graph.tar.gz](http://snap.stanford.edu/data/wikispeedia/wikispeedia_paths-and-graph.tar.gz)   
which was found from:  
[SNAP: Web data: Wikispeedia navigation paths](http://snap.stanford.edu/data/wikispeedia.html)   

This folder of data contains different kinds of data collected, but what we are likely most interested in is the file titled `links.tsv`. This file contains a list of wikipedia article titles, and links to other articles that are referenced / linked in the body of that article.

This dataset was provided from the following sources:  
- Robert West and Jure Leskovec: Human Wayfinding in Information Networks. 21st International World Wide Web Conference (WWW), 2012.  
- Robert West, Joelle Pineau, and Doina Precup: Wikispeedia: An Online Game for Inferring Semantic Distances between Concepts. 21st International Joint Conference on Artificial Intelligence (IJCAI), 2009.  

## Data Format

The data is in tsv format. The first column of the table contains the list of article names, and the second column contains other articles that are linked in the body of the article.  
For example, the Wikipedia article "Agriculture" (in the first column) links to the article "Vegetable" (in the second column). 

## Data Correction

Some of the article titles are URL-encoded for some reason (while others aren't), so we will likely have to do some scripting to convert any URL-encoded article title into it's regular text format.

## Data Storage

We can represent this problem as a *directed* graph of Wikipedia article titles. Each node in our graph is an article, and a directed edge connecting, say, A to B would be mean that the article A has a link to article B contained in it (You can get to article B using a link from A). 
We will store this graph as a `std::map<std::string, std::vector<std::string>>`, with the key being the article title (`std::string`), and the value being a vector of article titles that can be accessed from that given article.

To generate the graph, we will simply have to iterate over the file, parsing the pairs of these article titles (as described above), and link them using our map. Let's call this map `wiki_map`. Using the example from before, we would say that "Agriculture" links to "Vegetable", which would be assigned like: `wiki_map["Agriculture"].push_back("Vegetable");`

## Algorithm

BFS
- Function efficiency: The worst BFS will require going through every link, so the runtime will be O(E) Where E is the number of links in the dataset. Holding every point in queue/set for this worst traversal will also require O(V) space, where V is the number of pages. 
- Function input: (Assume this function has access to the graph data). The function will take in a start page's title and destination page's title as strings. 
- Function output: The function will output a vector of strings where the first element is the start, the final element is the destination, and the interior are the titles of the pages used to get from the start to the end.

Dijkstra's
- Function efficiency: It was shown in class that the runtime of Dijkstra's algorithm is $O(E+V\log{V})$, where E is the number of eges (links in the dataset) and V is the number of vertices (pages in the dataset). 
- Function input: The function that implements Dijkstra's (function A) would take in the graph of pages and a starting vertex. The user would interact with another function (function B) which would take in a start and end page, and calls function A using the already generated graph and the start page.
- Function output: Function A would return a vector of distances from the start to each node and a vector which determines the immediate predecessor of each vertex. Function B would return a vector generated by finding the path in the predecessor vector from function A which goes from the start to end pages.

Page Rank
- Function efficiency: The page rank algorithm runs in $O(V + E)$ time, where E is the number of eges (links in the dataset) and V is the number of vertices (pages in the dataset). The memory needed to perform the algorithm is at least O(V^2), since we need to represent the data as a graph adjacency matrix to perform the neccesary calculations.
- Function input: This function just needs the entire graph data to perform the algorithm.
- Function output: This function will output the relative strength / popularity of every page, in descending order of popularity. This will likely be a vector of pairs of strings and doubles, the string being the article title, and the double being the article's popularity (0.0 to 1.0).

## Timeline

- Initialize Project
- Be able to load data in. Store it as graph
- Create our first algorithm
- Develop the other two algorithms
- Develop Presentation
