#include "TransformBW.hpp"

TransformBW::TransformBW(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
             Fifo<std::shared_ptr<cv::Mat> > &outFifo,
             std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
             uint32_t frameCount)
: Transformer(inFifo, outFifo, displayFunc, frameCount)
{
    std::cout << "TransformBW constructor called." << std::endl;    
}

std::shared_ptr<cv::Mat> TransformBW::transform(
    std::shared_ptr<const cv::Mat> pInFrame)
{
    std::shared_ptr<cv::Mat> pout(new cv::Mat());
    cv::cvtColor(*pInFrame, *pout, cv::COLOR_RGB2GRAY);
    
    std::cout << "TransformBW transform called." << std::endl;
    return pout;
}
