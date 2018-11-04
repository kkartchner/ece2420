#include "DriveHead.hpp"

#include <iostream>
#include <mutex>
#include <condition_variable>

std::mutex g_mtx;
std::condition_variable g_cv;

DriveHead::DriveHead()
  : Head()
{
  std::cout << "DriveHead default constructor called on DriveHead " << this << std::endl;
}

DriveHead::DriveHead(DriveHead *driveHeadArray, int arraySize, int platterIndex)
: m_platterIndex(platterIndex), m_enabled(false)
{
  std::cout << "Drive head " << platterIndex << " (" << this 
            << ") created." << std::endl; 
  
   int i_last = arraySize-1; // Index of last head in platter vector
   
  // Determine indices of left and right neighbor (circularly):  
  int i_left = (platterIndex-1 >= 0) ? platterIndex-1 : i_last;
  int i_right = (platterIndex + 1) % arraySize;            
  
  // Assign neighbor pointers to platter heads at left and right index
  m_lNeighbor_ptr = &driveHeadArray[i_left];
  m_rNeighbor_ptr = &driveHeadArray[i_right];
  
  std::cout << "left neighbor: DriveHead " << m_lNeighbor_ptr
            << "\nright neighbor: DriveHead  " << m_rNeighbor_ptr << "\n" << std::endl;  
}

/*void DriveHead::genNeighborSharedPtrs() {
  int i_last = m_platter_ref.size()-1; // Index of last head in platter vector
   
  // Determine indices of left and right neighbor (circularly):  
  int i_left = (m_platterIndex-1 >= 0) ? m_platterIndex-1 : i_last;
  int i_right = (m_platterIndex + 1) % m_platter_ref.size();            
  
  // Assign neighbor pointers to platter heads at left and right index
  m_lNeighbor_ptr = m_platter_ref[i_left].get();
  m_rNeighbor_ptr = (DriveHead*) m_platter_ref[i_right].get();
  
  std::cout << "left neighbor: DriveHead " << m_lNeighbor_ptr->m_platterIndex
            << "\nright neighbor: DriveHead  " << m_rNeighbor_ptr->m_platterIndex << "\n" << std::endl;  
}*/

void DriveHead::execute(std::function<void()> action){  
  if (!(m_lNeighbor_ptr->m_enabled && m_rNeighbor_ptr->m_enabled)){
    g_mtx.lock();
    std::cout << "Head " << m_platterIndex << " executing." << std::endl;
    m_enabled = true;
    m_lNeighbor_ptr->disable();
    m_rNeighbor_ptr->disable();
    g_mtx.unlock();
    
    action();
  }
  
  // Ensure that both of the the adjacent heads are not currenty active.
  
  
  // Execute the function object 

  
  // Release any resources preventing the adjacent heads from acting.
   
  /* This function MUST be thread safe.  Any number of threads may call this 
     function any number of times and behavior must be correct. */  
}