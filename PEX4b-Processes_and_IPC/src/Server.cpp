#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <map>

#include "MathObj.hpp"
#include "Helpers.hpp"

int main(int argc, char** argv)
{
    int loopNum = 1;                        // Set number of loop iterations (from commandline if available)
    if (argc > 1){
        sscanf(argv[argc-1], "%d", &loopNum); 
    }  
    
    shm_unlink(problemMem.c_str());         // Unlink shared memory
    shm_unlink(answerMem.c_str());
    
    sem_unlink(problemNotFullSem.c_str());  // Unlink semaphores
    sem_unlink(problemNotEmptySem.c_str());
    sem_unlink(answerNotFullSem.c_str());
    sem_unlink(answerNotEmptySem.c_str());
    
    /*** Shared memory items: ****/   
    int fd_prob = shm_open(problemMem.c_str(), O_CREAT | O_RDWR, 0666); // Open shared memory 
    int fd_ans = shm_open(answerMem.c_str(), O_CREAT | O_RDWR, 0666);
   
    int probMemSize = ARRAY_SIZE *sizeof(Problem);                  // Set shared memory size
    int ansMemSize = ARRAY_SIZE *sizeof(Answer);
      
    ftruncate(fd_prob, probMemSize);
    ftruncate(fd_ans, ansMemSize);
    
    void *probList = mmap(NULL, probMemSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd_prob, 0);
    void *ansList = mmap(NULL, ansMemSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd_ans, 0);
    
    /**** Semaphores ******/
    sem_t *probListHasRoom = sem_open(problemNotFullSem.c_str(), O_CREAT | O_RDWR, 0666, ARRAY_SIZE);
    sem_t *probListHasContent = sem_open(problemNotEmptySem.c_str(), O_CREAT | O_RDWR, 0666, 0);
    
    sem_t *ansListHasRoom = sem_open(answerNotFullSem.c_str(), O_CREAT | O_RDWR, 0666, ARRAY_SIZE);
    sem_t *ansListHasContent = sem_open(answerNotEmptySem.c_str(), O_CREAT | O_RDWR, 0666, 0);
    /**********************/
   
    // Problem and answer handling:
    Problem *prob = (Problem *) probList;
    Answer *ans = (Answer *) ansList;
    
    Answer tempAns; // Used so answerMem isn't affected until after it has room (i.e. sem_wait(ansListHasRoom))   
    int probHead = 0, ansHead = 0;
    int sent = 0, received = 0;
    
    while (1){
        // Receive and solve problem:
        sem_wait(probListHasContent);
        
        std::cout << "Problem received from problemMem: " << problem_str(&prob[probHead]) << std::endl; 
        solve(&prob[probHead], &tempAns); // Solve problem and store answer into temporary Answer object
        received++;
        
        probHead = (probHead + 1) % ARRAY_SIZE;
        sem_post(probListHasRoom);
        
        // Send answer:
        sem_wait(ansListHasRoom);
        
        ans[ansHead].m_answer = tempAns.m_answer;
        std::cout << "Answer sent to answerMem: " << ans[ansHead].m_answer << "\n" << std::endl;      
        sent++;
       
        ansHead = (ansHead + 1) % ARRAY_SIZE;
        sem_post(ansListHasContent);        
    }
    
    std::cout << "loopNum: " << loopNum << std::endl; 
    std::cout << "Total messages received: " << received << std::endl;
    std::cout << "Total messages sent: " << sent << std::endl;
    
    return 0;
}