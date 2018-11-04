#include "DriveHead.hpp"

#include <iostream>

#define DEBUG 1

void action(){
  for (int i = 0; i < 5; i++){
    std::cout << "Action performed." << std::endl; 
  }
}

int main (int argc, char** argv){
  int headNum;
  
  if (argc > 1) {
    sscanf(argv[1], "%d", &headNum);
    if (headNum < 0){
      std::cout << "Invalid number of heads specified. Number must be positive." << std::endl;
      return -1;
    }
  } else {
    headNum = 8;
  }
  if (DEBUG) { 
    std::cout << "Main started \nheadNum: " << headNum << "\n" << std::endl;  
  }
  
  auto platter = Head::makePlatter(headNum);
  
  std::shared_ptr<std::thread> threads[headNum];
  for (int i = 0; i < headNum; i++)
  {
    threads[i] = std::make_shared<std::thread>(&Head::execute, platter[i], action);
  }
 /* std::cout << "headPlatter size: " << headPlatter.size() 
            << "\nheadPlatter[0]: " << headPlatter.front()
            << "\nheadPlater[last]: " << headPlatter.back() << std::endl;
  */
  
  for (auto t : threads){
    t->join();
  }
  
  return 0;
}