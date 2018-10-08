#include "Helpers.hpp"

std::string solve(Problem *prob, Answer *ans){
    switch (prob->m_opcode){
        case Operation::eADD:
            ans->m_answer = prob->m_op1 + prob->m_op2;
            break;
        case Operation::eSUB:
            ans->m_answer = prob->m_op1 - prob->m_op2;
            break;
        case Operation::eMUL:
            ans->m_answer = prob->m_op1 * prob->m_op2;
            break;
        case Operation::eDIV:
            ans->m_answer = prob->m_op1 / prob->m_op2;
            break;
    }
    
    return problem_str(prob) + " = " + std::to_string((int)ans->m_answer);
}

std::string problem_str(Problem *prob){
    std::string op_str;
    switch (prob->m_opcode){
        case Operation::eADD:   op_str += " + ";   break;
        case Operation::eSUB:   op_str += " - ";   break;
        case Operation::eMUL:   op_str += " x ";   break;
        case Operation::eDIV:   op_str += " / ";   break;
    }
    
    return std::to_string((int)prob->m_op1) + op_str + std::to_string((int)prob->m_op2);
}