#ifndef TRANSFORM_NONE_HPP
#define TRANSFORM_NONE_HPP

#include "Transformer.hpp"

class TransformNone : public Transformer 
{
public:
    TransformNone(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
                       Fifo<std::shared_ptr<cv::Mat> > &outFifo,
                       std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
                       uint32_t frameCount);
    
private:
    virtual std::shared_ptr<cv::Mat> transform(
        std::shared_ptr<const cv::Mat> pInFrame) override;
};

#endif /* TRANSFORM_NONE_HPP */