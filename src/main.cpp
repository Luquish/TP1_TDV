#include "taxi_assignment_instance.h"
#include "taxi_assignment_solution.h"
#include "checker.h"
#include "greedy_solver.h"


int main(int argc, char** argv) {
    std::string filename = "input/small_1.csv";

    TaxiAssignmentInstance instance(filename);
    std::cout << filename << std::endl;

    TaxiAssignmentSolution solution(instance.n);

    GreedySolver solver(instance);

    solver.solve();

    TaxiAssignmentSolution greedy_solution = solver.getSolution();

    std::cout << "Greedy Solution: " << std::endl;

    for (int i = 0; i < instance.n; i++){
        std::cout << "Taxi " << i << " assigned to passenger " << greedy_solution.getAssignedPax(i) << std::endl;
    }

    std::cout << "Objective Value: " << solver.getObjectiveValue() << std::endl;
    std::cout << "Solution Time: " << solver.getSolutionTime() << std::endl;

    return 0;
}
