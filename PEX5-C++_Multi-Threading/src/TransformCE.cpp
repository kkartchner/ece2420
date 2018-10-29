#include "TransformCE.hpp"

TransformCE::TransformCE(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
             std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
             uint32_t frameCount)
: Transformer(inFifo, displayFunc, frameCount)
{
    std::cout << "TransformCE constructor called." << std::endl;    
}

std::shared_ptr<cv::Mat> TransformCE::transform(
    std::shared_ptr<const cv::Mat> pInFrame)
{
    std::shared_ptr<cv::Mat> pout(new cv::Mat());
    cv::Canny(*pInFrame, *pout, 0, 30, 3);     
    
    std::cout << "TransformCE transform called." << std::endl;
    
    return pout;
}
 
