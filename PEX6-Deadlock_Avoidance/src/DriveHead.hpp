#ifndef DRIVE_HEAD_HPP
#define DRIVE_HEAD_HPP

#include "Head.hpp" 

#include <thread>
#include <mutex>

class DriveHead : public Head
{
public:
  /**
  * @brief Construct a drive head that knows its right and left neighboring
  * heads.
  * 
  * @param lNeighbor The head's left neighboring head
  * @param rNeighbor The head's right neighboring head
  */
  /*DriveHead(std::vector<std::shared_ptr<Head> > &platter, 
            int indexInPlatter); // std::vector<std::mutex> &mtx);
  */
  DriveHead();
  
  DriveHead(DriveHead *driveHeadArray, int arraySize, int platterIndex);
 /* DriveHead(std::shared_ptr<DriveHead> lNeighbor,
            std::shared_ptr<DriveHead> rNeighbor,
            std::mutex &mtx
           );
  */
  
  
  /**
  * @brief Override the base class execute function
  * 
  * @param action The action to perform
  */
  virtual void execute(std::function<void()> action) override;  
  
  /**
  * @brief Enables the drive head
  */
  void enable(){ m_enabled = true; }
  /**
  * @brief Disables the drive head
  */
  void disable(){ m_enabled = false;}
  /**
  * @brief Return true if enabled, false if not    /**
  * @brief Assign m_lNeighbor and m_rNeighbor shared pointers based
  * based on platter location.
  */
  
  bool m_enabled;
  
  //bool isActive(){ return enabled; };
  
  std::shared_ptr<std::thread> m_thread_ptr;
  
private:
    /**
  * @brief Assign m_lNeighbor and m_rNeighbor shared pointers based
  * based on platter location.
  */
  void genNeighborSharedPtrs();
  
  int m_platterIndex;

  DriveHead *m_lNeighbor_ptr;
  DriveHead *m_rNeighbor_ptr;
};

#endif /* DRIVE_HEAD_HPP */