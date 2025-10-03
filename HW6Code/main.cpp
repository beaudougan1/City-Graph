#include "CityGraph.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <climits>

using namespace std;

// Custom priority queue implemented as a min-heap
class PriorityQueue {
    vector<pair<int, int> > heap; // Pair of (city ID, distance)

    // Helper function to maintain heap property after insertion
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index].second < heap[parent].second) {
                swap(heap[index], heap[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    // Helper function to maintain heap property after removal
    void heapifyDown(int index) {
        int size = heap.size();
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < size && heap[left].second < heap[smallest].second) {
                smallest = left;
            }
            if (right < size && heap[right].second < heap[smallest].second) {
                smallest = right;
            }

            if (smallest != index) {
                swap(heap[index], heap[smallest]);
                index = smallest;
            } else {
                break;
            }
        }
    }

public:
    // Check if the priority queue is empty
    bool isEmpty() { return heap.empty(); }

    // Add an element to the priority queue
    void push(pair<int, int> element) {
        heap.push_back(element);
        heapifyUp(heap.size() - 1);
    }

    // Remove and return the element with the smallest distance
    pair<int, int> pop() {
        pair<int, int> top = heap.front();
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
        return top;
    }
};

// Function to perform Dijkstra's algorithm to find the shortest path
vector<int> dijkstra(CityGraph& graph, int start, int end, int& shortestDistance) {
    unordered_map<int, int> distances; // Map of city ID to shortest distance
    unordered_map<int, int> previous; // Map of city ID to previous city in the path

    // Initialize distances to infinity and previous to -1
    for (int i = 0; i < 20; ++i) {
        distances[i] = INT_MAX;
        previous[i] = -1;
    }

    distances[start] = 0;

    // Custom priority queue to track the shortest known distance
    PriorityQueue pq;
    pq.push(make_pair(start, 0));

    // Perform Dijkstra's algorithm
    while (!pq.isEmpty()) {
        // Retrieve the city with the smallest known distance
        pair<int, int> currentPair = pq.pop();
        int current = currentPair.first;

        // If we've reached the destination, stop the algorithm
        if (current == end) break;

        // Iterate over all neighbors (roads) of the current city
        for (const auto& road : graph.getNeighbors(current)) {
            int newDist = distances[current] + road.distance; // Calculate new distance
            if (newDist < distances[road.toCity]) {  // Update the distance if shorter
                distances[road.toCity] = newDist;
                previous[road.toCity] = current;
                pq.push(make_pair(road.toCity, newDist)); // Add the city to the priority queue
            }
        }
    }

    shortestDistance = distances[end];
    vector<int> path;

    // Reconstruct the path if a valid route was found
    if (shortestDistance == INT_MAX) return path; // No route found

    for (int at = end; at != -1; at = previous[at]) {
        path.insert(path.begin(), at); // Insert each city in the path
    }
    return path;
}

int main(int argc, char* argv[]) {
    // Ensure there are exactly 3 arguments: program name, From_City_Code, and To_City_Code
    if (argc != 3) {
        cerr << "Usage: ./sssp <From_City_Code> <To_City_Code>" << endl;
        return 1;
    }

    // Read command-line arguments
    string fromCode = argv[1]; // First argument: From_City
    string toCode = argv[2];   // Second argument: To_City

    //Header information
    cout << "Author: Beau Dougan, Landon Graham\n";
    cout << "Date: 11/24/2024\n";
    cout << "Course: CS311 (Data Structures and Algorithms)\n";
    cout << "Description: Program to find the shortest route between cities\n";
    cout << "----------------------------------------------------------------\n";

    CityGraph graph;

    //Load city and road data from files
    graph.loadCities("city.txt");
    graph.loadRoads("roads.txt");
    graph.buildGraph();

    //Validate the city codes provided from the command line arguments
    if (!graph.realCity(fromCode)) {
        cerr << "Invalid city code: " << fromCode << endl;
        return 1;
    }
    if (!graph.realCity(toCode)) {
        cerr << "Invalid city code: " << toCode << endl;
        return 1;
    }

    //Print information about the cities
    cout << "From City: ";
    graph.printCityInfo(fromCode);
    cout << "To City: ";
    graph.printCityInfo(toCode);

    //Get the city IDs based on their codes
    int fromID = graph.getCity(fromCode).id;
    int toID = graph.getCity(toCode).id;

    //Variable to store the shortest distance
    int shortestDistance;

    //Find the shortest path using Dijkstra's algorithm
    vector<int> path = dijkstra(graph, fromID, toID, shortestDistance);

    //Output the results
    if (path.empty()) {
        cout << "No route from " << graph.getCity(fromCode).name << " to " << graph.getCity(toCode).name << endl;
    } else {
        cout << "The shortest distance from " << graph.getCity(fromCode).name << " to " << graph.getCity(toCode).name << " is " << shortestDistance << endl;
        cout << "Through the route: ";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << graph.getCityByID(path[i]).name; //Print the city name in the path
            if (i != path.size() - 1) cout << " -> "; //add arrows between cities
        }
        cout << endl;
    }

    return 0;
}
