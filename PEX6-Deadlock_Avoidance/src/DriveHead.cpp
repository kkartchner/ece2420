#include "DriveHead.hpp"

#include <iostream>

/**
* Constructs the DriveHead including the creation of the mutex that 
* belongs to this DriveHead (the object that m_mtx1_sp points to).
*/
DriveHead::DriveHead()
  : Head()
{
    m_mtx1_sp = std::make_shared<std::mutex>();
  
    std::cout << "DriveHead default constructor called on DriveHead " << this << std::endl;
}

/** 
* Executes the provided action in a thread safe matter. (No two
* adjacent head can be executing at the same time).
*/
void DriveHead::execute(std::function<void()> action){  
  // Grab (lock) mutexes in order, then release (unlock) after action is called.
  m_mtx1_sp->lock();
  m_mtx2_sp->lock();
  
  action();
  
  m_mtx1_sp->unlock();
  m_mtx2_sp->unlock();
}

/**
* Swaps the mutex pointers, so that the order they are grabbed in
* the execute function is swapped as well. 
*/
void DriveHead::swapMtxOrder(){
  auto temp = m_mtx2_sp;
  m_mtx2_sp = m_mtx1_sp;
  m_mtx1_sp = temp;
}

