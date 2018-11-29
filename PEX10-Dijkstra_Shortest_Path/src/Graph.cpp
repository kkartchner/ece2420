#include <iostream>
#include <stack>
#include <algorithm>

#include "Graph.hpp"

void Graph::addEdge(const std::string &from, const std::string &to,
        const uint32_t &dist)
{
    m_adjList[from][to] = dist;
    m_adjList[to];
}

void Graph::printShortestPath(const std::string &srcCity, 
        const std::string &destCity)
{
    std::list<std::string> unvisited;
    std::map<std::string, uint32_t> distFromSrc;

    for (auto city : m_adjList){
        distFromSrc[city.first] = UINT32_MAX; // Set distance from source to "infinity"
        unvisited.push_back(city.first); // Add each city to the unvisited list
    }

    distFromSrc[srcCity] = 0;
#if 0
    std::cout << "Distances from  " << srcCity << std::endl;
    for (auto dist : distFromSrc){
        std::cout << dist.first << ": " << dist.second << std::endl;
    }


    std::cout << "\nUnvisited list: " << std::endl;
    for (auto city : unvisited){
        std::cout << city << std::endl;
    }
#endif

    std::string minUnvisited;

    while (!unvisited.empty()){
#if 0
        for (auto city : unvisited){
            std::cout << city << std::endl;
        }
#endif
        minUnvisited = getMinUnvisited(unvisited, distFromSrc);
        if (minUnvisited == "none"){
            std::cout << destCity << " not reachable from " << srcCity << std::endl;
            return;
        }
        unvisited.remove(minUnvisited);

        for (auto neighbor : m_adjList[minUnvisited]){
            if(std::count(unvisited.begin(), unvisited.end(), neighbor.first) > 0){
    //            std::cout << "visiting " << neighbor.first;
                // distance of minUnvisited from src + distance of neighbor from minUnvisited
                uint32_t newDist = distFromSrc[minUnvisited] + neighbor.second;

                if (newDist < distFromSrc[neighbor.first]){
                    distFromSrc[neighbor.first] = newDist;
                    m_predecessor[neighbor.first] = minUnvisited;
                }
            }
        } 

    }


    std::stack<std::string> predStack; 
    std::string curCity = destCity;
    while (1){
        predStack.push(curCity); // Add current city to stack
        try { // Set current city to its predecessor (if it has one)
            curCity = m_predecessor.at(curCity); 
        } catch (const std::out_of_range &oor) {
            break;
        }
    }

#if 0 
    std::cout << "Distances from  " << srcCity << std::endl;
    for (auto dist : distFromSrc){
        std::cout << dist.first << ": " << dist.second << std::endl;
    }
#endif
#if 0
    std::cout << "\nUnvisited list: " << std::endl;
    for (auto city : unvisited){
        std::cout << city << std::endl;
    }
#endif
    while (!predStack.empty()){
        std::cout << predStack.top() << std::endl;
        predStack.pop();
    }

    std::cout << "Total Distance: " << distFromSrc[destCity] << std::endl;
#if 0
    std::cout << "Predecessors: "  << std::endl;
    for (auto city : m_predecessor){
        std::cout << city.first << "->" << city.second << std::endl;
    }
#endif
}

std::string Graph::getMinUnvisited(std::list<std::string> &unvisited,
        std::map<std::string, uint32_t> &distFromSrc)
{
    std::string minUnvisited = unvisited.front();
#if 0
    std::cout << "\nminimum unvisited city(before getMin): " << minUnvisited << std::endl;
#endif
    for (auto vertex : unvisited){
        if (distFromSrc[vertex] < distFromSrc[minUnvisited]){
            minUnvisited = vertex;
        }
    }
#if 0
    std::cout << "minimum unvisited city (after getMin): " << minUnvisited << "\n" << std::endl;
#endif

    if (distFromSrc[minUnvisited] == UINT32_MAX){
        minUnvisited = "none";
    }
    return minUnvisited;
}
