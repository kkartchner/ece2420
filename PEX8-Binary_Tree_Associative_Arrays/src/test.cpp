#include "KVBinaryTree.hpp"
#include <string>
#include <cstdlib>


int main(int argc, char** argv){
    // Instantiate an associative array
    KVBinaryTree<std::string, int> scoreBoard; // Person's name and score

    auto printScore = [](const std::string &key, int &val){
        std::cout << key << ": " << val << std::endl;
    };

    srand(time(NULL));
    scoreBoard.insert("Person2", 100); 
    for (int i = 0; i < 6; i++){
        int score = rand() % 100;
        std::string name = "Person" + std::to_string(i);
        scoreBoard.insert(name, score);
    }

    KVBinaryTree<std::string, int> scoreBoardCopy(scoreBoard);

    scoreBoard.insert("Person11", 100);

    auto scoreBoard3 = scoreBoardCopy;
    scoreBoardCopy.del("Person31");

    scoreBoard3.del("Person5");
    scoreBoard3.del("Person2");
    scoreBoard3.insert("Person1", 40);

    std::cout << "Scoreboard:" << std::endl;
    scoreBoard.forEach(printScore);

    std::cout << "\nScoreboardCopy:" << std::endl;
    scoreBoardCopy.forEach(printScore);

    std::cout << "\nScoreboard3:" << std::endl;
    scoreBoard3.forEach(printScore);

    return 0;
}
