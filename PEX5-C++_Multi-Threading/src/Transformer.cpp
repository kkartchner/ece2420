#include "Transformer.hpp"

Transformer::Transformer(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
              Fifo<std::shared_ptr<cv::Mat> > &outFifo,
              std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
              uint32_t frameCount)
: m_inFifo(inFifo), m_outFifo(outFifo)
{
    std::cout << "Transformer constructor 1 called" << std::endl;
    m_displayFunc = displayFunc;
    m_last = false;
    
    m_pProcessThread = std::make_shared<std::thread>(&Transformer::process, this, frameCount);
}

Transformer::Transformer(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
              std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
              uint32_t frameCount)
: m_inFifo(inFifo), m_outFifo(inFifo)
{ 
    std::cout << "Transformer constructor 2 called." << std::endl;
    m_displayFunc = displayFunc;
    m_last = true;
    
    m_pProcessThread = std::make_shared<std::thread>(&Transformer::process, this, frameCount);
}

void Transformer::wait(){
    m_pProcessThread->join();
}
void Transformer::process(uint32_t frameCount){
    std::cout << "Process began" << std::endl;
    while (frameCount > 0){
      std::shared_ptr<cv::Mat> frameIn;  // Read in frame from input fifo
      std::shared_ptr<cv::Mat> frameOut;
      m_inFifo.popItem(frameIn);         //
        
      frameOut = transform(frameIn);        // Transform the frame (should call derived transform function)
      if (!m_last){                    // Add the frame to the output fifo if this isn't the last transformer
          m_outFifo.addItem(frameOut);    
      }   
      m_displayFunc(frameOut);            // Display the transformed frame
      frameCount--;
    }
}