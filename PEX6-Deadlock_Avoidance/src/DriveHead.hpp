#ifndef DRIVE_HEAD_HPP
#define DRIVE_HEAD_HPP

#include "Head.hpp" 

#include <mutex>

class DriveHead : public Head
{
public:
/*** Constructor **/
  /**
  * @brief Default constructor
  * 
  * Constructs the DriveHead including the creation of the mutex that 
  * belongs to this DriveHead (the object that m_mtx1_sp points to).
  */
  DriveHead();
  
/*** Methods **/
  /**
  * @brief Override the base class execute function
  *
  * Executes the provided action in a thread safe matter. (No two
  * adjacent head can be executing at the same time).
  *
  * @param action The action to perform
  */
  virtual void execute(std::function<void()> action) override;  
  
  /**
  * @brief Swap the mutex grab order
  * 
  * Swaps the mutex pointers, so that the order they are grabbed in
  * the execute function is swapped as well.
  */
  void swapMtxOrder();
  
/*** Data Members ****/
  /**
  * @brief First mutex to grab. 
  *
  * Typically the head's own mutex. But it will be the right neighbor's for 
  * the last head in order to prevent deadlock.
  */  
  std::shared_ptr<std::mutex> m_mtx1_sp;
  /**
  * @brief Second mutex to grab. 
  *
  * Typically a pointer to the right neighbor's mutex. But it will
  * the head's own mutex if it is the last head, again in order to
  * prevent deadlock
  */  
  std::shared_ptr<std::mutex> m_mtx2_sp;
  
};

#endif /* DRIVE_HEAD_HPP */