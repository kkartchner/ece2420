#ifndef TRANSFORM_BW_HPP
#define TRANSFORM_BW_HPP

#include "Transformer.hpp"

class TransformBW : public Transformer 
{
public:
    TransformBW(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
                Fifo<std::shared_ptr<cv::Mat> > &outFifo,
                std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
                uint32_t frameCount);
    
private:
    virtual std::shared_ptr<cv::Mat> transform(
        std::shared_ptr<const cv::Mat> pInFrame) override;
};

#endif /* TRANSFORM_BW_HPP */ 
