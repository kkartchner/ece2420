#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include "MathObj.hpp"

/* Solves and prints the answer */
std::string solve(Problem *prob, Answer *ans);

/* Prints the problem (i.e. 1 + 2) */
std::string problem_str(Problem *prob);

#endif /* HELPERS_H */