#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <map>
#include <string>
#include <list>

class Graph  
{
    public:
        void addEdge(const std::string &from, const std::string &to,
                const uint32_t &dist);
        void printShortestPath(const std::string &srcCity, 
                const std::string &destCity);
    private:
        std::map<std::string, std::map<std::string, uint32_t> > m_adjList;
        std::map<std::string, std::string> m_predecessor; 

        std::string getMinUnvisited(std::list<std::string> &unvisited,
                std::map<std::string, uint32_t> &distFromSrc);
};

#endif /* GRAPH_HPP */
