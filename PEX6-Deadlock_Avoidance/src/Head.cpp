#include "Head.hpp"
#include "DriveHead.hpp"

#include <iostream>

Head::Head()
{
  std::cout << "Head constructor called." << std::endl;
}

/** 
* "This static method will create a number of head objects, connect
* each of the heads in a way that they can lock out their right/left
* neighbor, and return shared pointers to each created head in a vector."
*/
std::vector<std::shared_ptr<Head> > Head::makePlatter(int numHead){
  // Create a vector for storing shared pointers to the DriveHeads:
  std::vector<std::shared_ptr<DriveHead> > platter;
   
  // Fill the vector with new DriveHead shared_ptrs:
  for (int i=0; i < numHead; i++){
    platter.push_back(std::make_shared<DriveHead>());
  }
  // Assign the DriveHead mutex shared_ptrs:
  for (int i=0; i < numHead; i++){  
    int i_rNeighbor = (i+1) % numHead;
    platter[i]->m_mtx2_sp = platter[i_rNeighbor]->m_mtx1_sp;    
  }
  // Swap the mtx shared pointers on the last head,  so the order they are 
  // grabbed is swapped, in order to prevent deadlock.
  platter[numHead-1]->swapMtxOrder();
  
  // Return a new vector of Head shared_ptrs created from platter.
  return std::vector<std::shared_ptr<Head> >(platter.begin(), platter.end()); 
}