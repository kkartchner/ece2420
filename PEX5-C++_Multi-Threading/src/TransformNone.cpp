#include "TransformNone.hpp"

TransformNone::TransformNone(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
              Fifo<std::shared_ptr<cv::Mat> > &outFifo,
              std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
              uint32_t frameCount)
: Transformer(inFifo, outFifo, displayFunc, frameCount)
{
    std::cout << "TransformNone constructor called." << std::endl;    
}

std::shared_ptr<cv::Mat> TransformNone::transform(
    std::shared_ptr<const cv::Mat> pInFrame)
{
    std::shared_ptr<cv::Mat> pout(new cv::Mat());
    std::cout << "TransformNone transform called." << std::endl;
    pInFrame->copyTo(*pout);
    return pout;
}
