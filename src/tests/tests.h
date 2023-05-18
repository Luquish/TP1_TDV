#include <cstdint>
#include <vector>
#include <iostream>
#include <cassert>

#include "taxi_assignment_instance.h"
#include "taxi_assignment_solution.h"
#include "checker.h"
#include "greedy_solver.h"

class TaxiAssignmentTests 
{
    public:
        void testGreedySolverSmall1Assignments();
        void testGreedySolverSmall1Cost();
        
        void testGreedySolverSmall2();
        void testGreedySolverSmall3();

    private:
        void runTests();
};
