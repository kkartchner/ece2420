#include <mqueue.h>
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <sys/wait.h>

#include "MathObj.hpp"
#include "Helpers.hpp"

int main(int argc, char **argv)
{
    int loopNum = 1;                            // Set number of loop iterations (from commandline if available)
    if (argc > 1){
        sscanf(argv[argc-1], "%d", &loopNum); 
    }
    std::cout << "loopNum: " << loopNum << std::endl;
     
    mqd_t problemQueue = mq_open(problemQueueName.c_str(), O_RDWR, 0666, NULL); // Open message queues
    mqd_t answerQueue = mq_open(answerQueueName.c_str(), O_RDWR, 0666, NULL);
   
    mq_attr problemAttr, answerAttr;
    
    mq_getattr(problemQueue, &problemAttr);         // Get attributes of queues
    mq_getattr(answerQueue, &answerAttr);  
   
  //Fork the process so we have two processes running.  The child will create
  //problems and send them to the service.  The parent will wait for responses
  //and then wait for the child to die after collecting and evaluating all responses
  int pid = fork();
  if(pid == 0)
  {
    srand(time(0));
    Problem *prob = new Problem();
    int sent = 0;
    for(int i = 0; i < loopNum; i++)
    {
        for(int j = 0; j < ARRAY_SIZE; j++){
        //Create a problem to send to the server.
            prob->m_opcode = (Operation) (rand() % 3);
            prob->m_op1 = (rand() % 11) + 1;
            prob->m_op2 = (rand() % 11) + 1;
            
            if(mq_send(problemQueue, (char *) prob, sizeof(Problem), 1) + 1){     // Yields true if mq_send returns success value of 0. False if it returns -1 on failure.
                std::cout << "Problem sent to problemQueue: " << problem_str(prob) << std::endl;
                sent++;
            } else {
                std::cout << "Failed to send to problemQueue." << std::endl;   
                return -1;
            }
        }
    }
      std::cout << "sent: " << sent << std::endl;
  }
  else
  {
      unsigned int prio = 1;
      Answer *ans = new Answer();
      int received = 0;
      for(int i = 0; i < loopNum; i++)
      {
          for (int j = 0; j < ARRAY_SIZE; j++){
              //Retrieve an answer from the server.  
              if(mq_receive(answerQueue, (char *) ans, answerAttr.mq_msgsize, &prio) + 1){  // Yields true if mq_receive returns success value of 0. False if it returns -1 on failure.
                  std::cout << "Answer received back from answerQueue: " << ans->m_answer << std::endl;
                      received++;
              } else {
                  std::cout << "Failed to receive from answerQueue." << std::endl;
                  return -1;
              }
          }
    }

    //This call will force the parent process to wait for the child to die.  It may ensure
    //that all problems are processed.  Also ensure a clean teardown, (i.e. no zombie/orphaned 
    //processes)
      int status;
      waitpid(pid, &status, 0);
      
      std::cout << "loopNum: " << loopNum << std::endl;
      std::cout << "received: " << received << std::endl;
  }
}
