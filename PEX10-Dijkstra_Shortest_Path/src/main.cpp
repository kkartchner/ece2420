#include <iostream>
#include <fstream>
#include <cstdint>
#include "Graph.hpp"

int main (int argc, char** argv){
    if (argc != 2){
        std::cout << "Incorrect number of arguments. Need to provide name of input file.\n "
            "(i.e.\"./main input.txt\")" << std::endl;    
        return -1;
    }

    std::ifstream inputFile(argv[1]);

    Graph graph;

    std::string srcCity, destCity;

    if (inputFile.is_open()){
        inputFile >> srcCity >> destCity;

        std::string from, to; 
        uint32_t dist;

        while (inputFile >> from >> to >> dist){
            graph.addEdge(from, to, dist); 
        }
    }     

    graph.printShortestPath(srcCity, destCity);

    inputFile.close();

    return 0;
};

