#include "KVRBTree.hpp"
#include <string>
#include <cstdlib>

#include <algorithm>
#include <vector>
#include <ctime>

int myrandom(int i) { return std::rand()%i; }

int main(int argc, char** argv){
    // Instantiate an associative array
    KVRBTree<std::string, int> scoreBoard; // Person's name and score

    auto printScore = [](const std::string &key, int &val){
        std::cout << key << ": " << val << std::endl; 
    };
    
    int personNum = 10;
    std::vector<int> nums(personNum);
    for (int i = 0; i < personNum; ++i){
        nums[i] = i;
    }
//    std::srand(std::time(NULL));
    std::random_shuffle( nums.begin(), nums.end(), myrandom );

    for (auto num : nums){
        int score = rand() % 100;
        std::string name = "P" + std::to_string(num);
        scoreBoard.insert(name, score);
    }
    scoreBoard.printTree();

    //  KVRBTree<std::string, int> scoreBoardCopy;
    //     scoreBoardCopy = scoreBoard;

    //    for (int i = 0; i < 10; i++){
    //        scoreBoard.del("Person" + std::to_string(i));
    //    }
    //    scoreBoard.insert("Person11", 100);
    //
    //    auto scoreBoard3 = scoreBoardCopy;
    //    scoreBoardCopy.del("Person31");
    //
    //    std::cout << "Scoreboard:" << std::endl;
    //    scoreBoard.forEach(printScore);
    //
    //    std::cout << "\nScoreboardCopy:" << std::endl;
    //    scoreBoardCopy.forEach(printScore);
    //
    //    std::cout << "\nScoreboard3:" << std::endl;
    //    scoreBoard3.forEach(printScore);

    return 0;
}
