#include "Head.hpp"
#include "DriveHead.hpp"

#include <iostream>

Head::Head()
{
  //std::cout << "Head constructor called." << std::endl;
}

std::vector<std::shared_ptr<Head> > Head::makePlatter(int numHead){
  DriveHead *driveHeadArray;
  std::vector<std::shared_ptr<Head> > platter(numHead);
  
  if (numHead > 1){
    for (int i=0; i < numHead; i++){
      auto newHead = new DriveHead(driveHeadArray, numHead, i);
      platter[i].reset(newHead); //, mtx);
    }
  }
  
  driveHeadArray = (DriveHead*) platter.front().get();
   
  return platter;
}