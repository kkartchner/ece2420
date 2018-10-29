#include "TransformNone.hpp"
#include "TransformBW.hpp"
#include "TransformCE.hpp"

#include <string>

std::mutex mtx;

void displayFrame(std::shared_ptr<cv::Mat> frame, const char* window_name){
    std::unique_lock<std::mutex> mlck(mtx);
    //cv::namedWindow(window_name, CV_WINDOW_AUTOSIZE);   
    cv::imshow(window_name, *frame);        
    cv::waitKey(1); 
    
    std::cout << window_name << " called on frame " << frame << std::endl;
}
void displayFrame_TNone(std::shared_ptr<cv::Mat> frame){
    displayFrame(frame, "TNone window");
}
void displayFrame_TBW(std::shared_ptr<cv::Mat> frame){
    displayFrame(frame, "TBW window");
}
void displayFrame_TCE(std::shared_ptr<cv::Mat> frame){
    displayFrame(frame, "TCE window");
}



bool loadVideoFrames(Fifo< std::shared_ptr<cv::Mat> > &fifo, std::string videoName, uint32_t frameCount){
    cv::VideoCapture video;
    if(video.open(videoName)){
        std::cout << "Video opened successfully!" << std::endl;
        for (int i = 0; i < frameCount; i++){
            std::shared_ptr<cv::Mat> pframe(new cv::Mat);
            std::shared_ptr<cv::Mat> newFrame(new cv::Mat);
            video >> *pframe;
            pframe->copyTo(*newFrame);
            fifo.addItem(newFrame);
        }
        return true;
    } else {
        std::cout << "Error opening video file. Is the name correct?" << std::endl;
        return false;
    }
}

int main(int argc, char** argv){
    std::string videoName;
    uint32_t frameCount;
    if (argc != 3){
       std::cout << "Incorrect number of argument:\n"
                 << "Entered " << argc << "; Requires 3" << std::endl;
       return -1;
    }

    videoName = argv[1];
    sscanf(argv[2], "%d", &frameCount);
    
    std::cout << "videoName: " << videoName << "\n"
              << "frameCount: " << frameCount << std::endl;

    // Declare 3 Fifos
    Fifo< std::shared_ptr<cv::Mat> > f1, f2, f3;
    
    // Declare 3 transformers
    auto t_none = new TransformNone(f1, f2, displayFrame_TNone, frameCount);
    auto t_BW = new TransformBW(f2, f3, displayFrame_TBW, frameCount);
    auto t_CE = new TransformCE(f3, displayFrame_TCE, frameCount);
  
    if (!loadVideoFrames(f1, videoName, frameCount)){
        return -2;
    }
    
    t_none->wait();
    t_BW->wait();
    t_CE->wait();
   
    std::cout << "Done." << std::endl;
    return 0;
}