
#ifndef CITYGRAPH_H
#define CITYGRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <climits>

using namespace std;
//struct for a city which has an id code name population and elevation
struct City{
  int id;
  string code;
  string name;
  int population;
  int elevation;
    //default constructor for city
    City() : id(0), population(0), elevation(0) {}
    //constructor for city
    City(int id, string code, string name, int population, int elevation)
            : id(id), code(code), name(name), population(population), elevation(elevation) {}
  };
//struct for roads between each city with distance weights the from city and the to city
struct Road {
  int fromCity;
  int toCity;
  int distance;
    //default road constructor
    Road() : fromCity(0), toCity(0), distance(0) {}
    //constructor for road
    Road(int fromCity, int toCity, int distance)
            : fromCity(fromCity), toCity(toCity), distance(distance) {}
  };

class CityGraph{
  private:
    //Adjacency list for graph using the key as the node selected and value as a list of neighbors
    unordered_map<int, vector<Road> > adjacencyList;

    //vector to store roads as edges
    vector<Road> roads;
    //take in city code and bring it to city details
    unordered_map<string, City> cities;

  public:
    //constructor and destructor
    CityGraph();
    ~CityGraph();;

    bool realCity(string& cityCode){
      return cities.find(cityCode) != cities.end();
    }

    void printCityInfo(string& cityCode){
      //check if city exists
      if(realCity(cityCode)){
        //set city to the City value for the city code in the map
        City& city = cities.find(cityCode)->second;
        //print the information for the city
        cout <<city.name << ", population " << city.population << ", elevation " << city.elevation << endl;
      }
      else{
        //if city does not exist print that it was not found with the code
        cout << "City not found: " << cityCode << endl;
      }
      }
	//load the cities from the file
    void loadCities(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening city file" << endl;
            exit(1);
        }
        string line;
        while (getline(file, line)) {
            // Skip empty lines
            if (line.empty()) continue;

            stringstream ss(line);
            string id, code, name, population, elevation;
            // Parse the line as space-separated values
            ss >> id >> code >> name >> population >> elevation;

            // Validate parsing
            if (id.empty() || code.empty() || name.empty() || population.empty() || elevation.empty()) {
                cerr << "Malformed line in city file: " << line << endl;
                continue;
            }

            try {
                // Convert to appropriate types and store in the map
                cities[code] = City(stoi(id), code, name, stoi(population), stoi(elevation));
            } catch (const invalid_argument& e) {
                cerr << "Invalid numeric value in city file: " << line << endl;
            } catch (const out_of_range& e) {
                cerr << "Numeric value out of range in city file: " << line << endl;
            }
        }
        file.close();
    }



   void loadRoads(const string& filename) {
       ifstream file(filename);
       if (!file.is_open()) {
           cerr << "Error opening road file" << endl;
           exit(1);
       }
       string line;
       while (getline(file, line)) {
           // Skip empty lines
           if (line.empty()) continue;

           stringstream ss(line);
           string fromCity, toCity, distance;
           // Parse the line as space-separated values
           ss >> fromCity >> toCity >> distance;

           // Validate parsing
           if (fromCity.empty() || toCity.empty() || distance.empty()) {
               cerr << "Malformed line in road file: " << line << endl;
               continue;
           }

           try {
               // Convert to appropriate types and store in the roads vector
               roads.push_back(Road(stoi(fromCity), stoi(toCity), stoi(distance)));
           } catch (const invalid_argument& e) {
               cerr << "Invalid numeric value in road file: " << line << endl;
           } catch (const out_of_range& e) {
               cerr << "Numeric value out of range in road file: " << line << endl;
           }
       }
       file.close();
   }


	//build the graph by storing all the roads into the adjacency list
    void buildGraph(){
      for(Road& road: roads){
          adjacencyList[road.fromCity].push_back(road);
        }
      }

    // Method to get all neighbors of a city
    vector<Road>& getNeighbors(int cityID) {
        return adjacencyList[cityID];
    }

    // Method to get all cities
    unordered_map<string, City>& getCities() {
        return cities;
    }

    // Method to get city by ID
    City& getCityByID(int cityID) {
        for (const auto& entry : cities) {
            if (entry.second.id == cityID) return cities[entry.first];
        }
        throw invalid_argument("City ID not found");
    }

    // Method to get city by code
    City& getCity(const string& cityCode) {
        return cities.at(cityCode);
    }
};
#endif //CITYGRAPH_H
