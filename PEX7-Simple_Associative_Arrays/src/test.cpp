#include "KVList.hpp"
#include <string>

int main(int argc, char** argv){
    // Instantiate an associative array
    KVList<std::string, int> scoreBoard; // Person's name and score

    auto printScore = [](const std::string &key, int &val){
        std::cout << key << ": " << val << std::endl;
    };
    
    for (int i = 0; i < 5; i++){
        int score = (i * 5) + (11 - i);
        std::string name = "Person" + std::to_string(i + score);
        scoreBoard.insert(name, score);
    }
    
    KVList<std::string, int> scoreBoardCopy(scoreBoard);
 //   scoreBoardCopy = scoreBoard;
   
    scoreBoard.insert("Person10", 100);

    auto scoreBoard3 = scoreBoardCopy;
    scoreBoardCopy.del("Person31");
    
    std::cout << "\nScoreboard:" << std::endl;
    scoreBoard.forEach(printScore);
   
    std::cout << "\nScoreboardCopy:" << std::endl;
    scoreBoardCopy.forEach(printScore);
     
    std::cout << "\nScoreboard3:" << std::endl;
    scoreBoard3.forEach(printScore);
    
    return 0;
}
