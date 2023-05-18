#include "tests.h"

void TaxiAssignmentTests::testGreedySolverSmall1Assignments()
{
    std::string filename = "input/small_1.csv";
    TaxiAssignmentInstance instance(filename);
    GreedySolver solver(instance);
    solver.solve();

    TaxiAssignmentSolution solution = solver.getSolution();

    assert(solution.getAssignedPax(0) == 7);
    assert(solution.getAssignedPax(1) == 5);
    assert(solution.getAssignedPax(2) == 2);
    assert(solution.getAssignedPax(3) == 6);
    assert(solution.getAssignedPax(4) == 8);
    assert(solution.getAssignedPax(5) == 9);
    assert(solution.getAssignedPax(6) == 1);
    assert(solution.getAssignedPax(7) == 4);
    assert(solution.getAssignedPax(8) == 3);
    assert(solution.getAssignedPax(9) == 0);
}

void TaxiAssignmentTests::testGreedySolverSmall1Cost()
{
    std::string filename = "input/small_1.csv";
    TaxiAssignmentInstance instance(filename);
    GreedySolver solver(instance);
    solver.solve();

    TaxiAssignmentSolution solution = solver.getSolution();

    assert(solver.getObjectiveValue() == 168.086);
}
