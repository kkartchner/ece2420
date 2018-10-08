#include <mqueue.h>
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <cstdlib>

#include "MathObj.hpp"
#include "Helpers.hpp"

int main(int argc, char** argv)
{
    int loopNum = 1;                            // Set number of loop iterations (from commandline if available)
    if (argc > 1){
        sscanf(argv[argc-1], "%d", &loopNum); 
    }

    mqd_t problemQueue = mq_open(problemQueueName.c_str(), O_RDWR, 0666, NULL); // Open message queues
    mqd_t answerQueue = mq_open(answerQueueName.c_str(), O_RDWR, 0666, NULL);
   
    mq_attr problemAttr, answerAttr;
    
    mq_getattr(problemQueue, &problemAttr);         // Get attributes of queues
    mq_getattr(answerQueue, &answerAttr);
    
    unsigned int prio = 1;
  
    srand(time(0));
    
    Problem *prob = new Problem();
    Answer *ans = new Answer();

    int sent = 0, received = 0;
    for (int j = 0; j < loopNum; j++){
        for (int i = 0; i < ARRAY_SIZE; i++){
            prob->m_opcode = (Operation) (rand() % 3);
            prob->m_op1 = (rand() % 11) + 1;
            prob->m_op2 = (rand() % 11) + 1;
            
            if(mq_send(problemQueue, (char *) prob, sizeof(Problem), 1) + 1){     // Yields true if mq_send returns success value of 0. False if it returns -1 on failure.
                std::cout << "Problem sent to problemQueue: " << problem_str(prob) << std::endl;
                sent++;
                
                if(mq_receive(answerQueue, (char *) ans, answerAttr.mq_msgsize, &prio) + 1){  // Yields true if mq_receive returns success value of 0. False if it returns -1 on failure.
                    std::cout << "Answer received back from answerQueue: " << ans->m_answer << "\n" << std::endl;
                    received++;
                } else {
                    std::cout << "Failed to receive from answerQueue." << std::endl;
                    return -1;
                }
            } else {
                std::cout << "Failed to send to problemQueue." << std::endl;   
                return -1;
            }
        }
    }

    std::cout << "loopNum: " << loopNum << std::endl;
    std::cout << "Total messages sent: " << sent << std::endl;
    std::cout << "Total messages received: " << received << std::endl;
    
    return 0;
}