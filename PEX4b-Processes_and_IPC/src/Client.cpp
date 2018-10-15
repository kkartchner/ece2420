#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <map>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <iostream>

#include "MathObj.hpp"
#include "Helpers.hpp"

int main(int argc, char** argv)
{
    int loopNum = 1;                            // Set number of loop iterations (from commandline if available)
    if (argc > 1){
        sscanf(argv[argc-1], "%d", &loopNum); 
    }
   
    /**** Shared Memory ****/
    int fd_prob = shm_open(problemMem.c_str(), O_RDWR, 0666); // Open shared memory locations
    int fd_ans = shm_open(answerMem.c_str(), O_RDWR, 0666);   
    
    int probMemSize = ARRAY_SIZE *sizeof(Problem);            // Declare shared memory size
    int ansMemSize = ARRAY_SIZE *sizeof(Answer);
   
    ftruncate(fd_prob, probMemSize);
    ftruncate(fd_ans, ansMemSize);
    
    void *probList = mmap(NULL, probMemSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd_prob, 0);
    void *ansList = mmap(NULL, ansMemSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd_ans, 0);
    
     /**** Semaphores *******/
    sem_t *probListHasRoom = sem_open(problemNotFullSem.c_str(), O_CREAT | O_RDWR, 0666, ARRAY_SIZE);
    sem_t *probListHasContent = sem_open(problemNotEmptySem.c_str(), O_CREAT | O_RDWR, 0666, 0);
    
    sem_t *ansListHasRoom = sem_open(answerNotFullSem.c_str(), O_CREAT | O_RDWR, 0666, ARRAY_SIZE);
    sem_t *ansListHasContent = sem_open(answerNotEmptySem.c_str(), O_CREAT | O_RDWR, 0666, 0);
     /***********************/
    
    srand(time(0));
    
    Problem *prob = (Problem *) probList;
    Answer *ans = (Answer *) ansList;
 
    int probHead = 0, ansHead = 0;
    int sent = 0, received = 0;
    
    for (int j = 0; j < loopNum; j++){
        for (int i = 0; i < ARRAY_SIZE; i++){
            // Create and send problem:
            sem_wait(probListHasRoom);
            
            prob[probHead].m_opcode = (Operation) (rand() % 3);
            prob[probHead].m_op1 = (rand() % 11) + 1;
            prob[probHead].m_op2 = (rand() % 11) + 1;
                      
            std::cout << "Problem sent to problemMem: " << problem_str(&prob[probHead]) << std::endl;
            sent++;
            
            probHead = (probHead + 1) % ARRAY_SIZE;
            sem_post(probListHasContent); 

            // Receive answer:
            sem_wait(ansListHasContent);
            
            std::cout << "Answer received from answerMem: " << ans[ansHead].m_answer << "\n" << std::endl;
            received++;           
            
            ansHead = (ansHead + 1) % ARRAY_SIZE;
            sem_post(ansListHasRoom);
        }
    }

    std::cout << "loopNum: " << loopNum << std::endl;
    std::cout << "Total messages sent: " << sent << std::endl;
    std::cout << "Total messages received: " << received << std::endl;
    
    return 0;
}
