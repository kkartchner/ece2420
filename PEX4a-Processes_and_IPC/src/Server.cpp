#include <mqueue.h>
#include <iostream>

#include "MathObj.hpp"
#include "Helpers.hpp"

int main(int argc, char** argv)
{
    int loopNum = 1;                            // Set number of loop iterations (from commandline if available)
    if (argc > 1){
        sscanf(argv[argc-1], "%d", &loopNum); 
    }  
    
    mq_unlink(problemQueueName.c_str());
    mq_unlink(answerQueueName.c_str());
    
    mqd_t problemQueue = mq_open(problemQueueName.c_str(), O_CREAT | O_RDWR, 0666, NULL); // Open message queues
    mqd_t answerQueue = mq_open(answerQueueName.c_str(), O_CREAT | O_RDWR, 0666, NULL);
   
    mq_attr problemAttr, answerAttr;
    
    mq_getattr(problemQueue, &problemAttr);         // Get attributes of queues
    mq_getattr(answerQueue, &answerAttr);
    
    unsigned int prio = 1;
    
    Problem *prob = new Problem();
    Answer *ans = new Answer();
    
    int sent = 0, received = 0;
    while (1){
            if(mq_receive(problemQueue, (char *) prob, problemAttr.mq_msgsize, &prio) + 1){
                std::cout << "Problem recieved from problemQueue: " << problem_str(prob) << std::endl;
                solve(prob, ans);
                received++;
                
                if (mq_send(answerQueue, (char *) ans, sizeof(Answer), 1) + 1){
                    std::cout << "Answer sent to answerQueue: " << ans->m_answer << "\n" << std::endl;      
                    sent++;
                } else {
                    std::cout << "Failed to send to answerQueue" << std::endl;
                    return -1;
                }
            } else {
                std::cout << "Failed to receive problem from problemQueue" << std::endl;
                return -1;
            }
    }
   
    std::cout << "loopNum: " << loopNum << std::endl; 
    std::cout << "Total messages received: " << received << std::endl;
    std::cout << "Total messages sent: " << sent << std::endl;
    
    return 0;
}