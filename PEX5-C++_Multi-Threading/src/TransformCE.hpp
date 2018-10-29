#ifndef TRANSFORM_CE_HPP
#define TRANSFORM_CE_HPP

#include "Transformer.hpp"

class TransformCE : public Transformer 
{
public:
    TransformCE(Fifo<std::shared_ptr<cv::Mat> > &inFifo,
                  std::function<void(std::shared_ptr<cv::Mat>)> displayFunc,
                  uint32_t frameCount);
    
private:
    virtual std::shared_ptr<cv::Mat> transform(
        std::shared_ptr<const cv::Mat> pInFrame) override;
};

#endif /* TRANSFORM_CE_HPP */ 
 
