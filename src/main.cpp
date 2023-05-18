#include "taxi_assignment_instance.h"
#include "taxi_assignment_solution.h"
#include "checker.h"
#include "greedy_solver.h"
#include "min_cost_flow_solver.h"

void check_small1(){
    std::string filename = "input/small_1.csv";

    TaxiAssignmentInstance instance(filename);
    std::cout << filename << std::endl;

    // Greedy Solver

    GreedySolver solver(instance);

    solver.solve();

    TaxiAssignmentSolution greedy_solution = solver.getSolution();

    std::cout << "Greedy Solution: " << std::endl;
    std::cout << greedy_solution << std::endl;

    std::cout << "Objective Value: " << solver.getObjectiveValue() << std::endl;
    std::cout << "Solution Time: " << solver.getSolutionTime() << std::endl;


    // Min-Cost-Flow Solver

    MinCostFlowSolver min_cost_flow_solver(instance);

    min_cost_flow_solver.solve();

    TaxiAssignmentSolution min_cost_flow_solution = min_cost_flow_solver.getSolution();

    std::cout << "Min Cost Flow Solution: " << std::endl;
    std::cout << min_cost_flow_solution << std::endl;

    std::cout << "Objective Value: " << min_cost_flow_solver.getObjectiveValue() << std::endl;
    std::cout << "Solution Time: " << min_cost_flow_solver.getSolutionTime() << std::endl;

    // Using a TaxiAssignmentChecker to check the solution

    TaxiAssignmentChecker checker = TaxiAssignmentChecker();

    double greedy_cost = checker.getSolutionCost(instance, greedy_solution);
    double min_cost_flow_cost = checker.getSolutionCost(instance, min_cost_flow_solution);

    std::cout << "Greedy Solution Cost: " << greedy_cost << std::endl;
    std::cout << "Min Cost Flow Solution Cost: " << min_cost_flow_cost << std::endl;

}

void batch_check(int n = 10) {
    std::vector<std::string> sizes = {"small", "medium", "large", "xl"};

    TaxiAssignmentChecker checker = TaxiAssignmentChecker();

    for (auto size : sizes) {
        std::string filename_start = "input/" + size;

        for (int i = 0; i < n; i++){
            std::string filename = filename_start + "_" + std::to_string(i) + ".csv";

            TaxiAssignmentInstance instance(filename);
            
            std::cout << filename << std::endl;

            // Greedy Solver
            
            GreedySolver solver(instance);

            solver.solve();

            TaxiAssignmentSolution greedy_solution = solver.getSolution();

            //Min-Cost-Flow Solver

            MinCostFlowSolver min_cost_flow_solver(instance);

            min_cost_flow_solver.solve();

            TaxiAssignmentSolution min_cost_flow_solution = min_cost_flow_solver.getSolution();

            // Using a TaxiAssignmentChecker to check the solution
            // Feasibility Check and Cost Check

            std::cout << "Greedy Solution Feasible: " << checker.checkFeasibility(instance, greedy_solution) << std::endl;
            double greedy_cost = checker.getSolutionCost(instance, greedy_solution);
            std::cout << "Greedy Objective Value: " << solver.getObjectiveValue() << std::endl;
            std::cout << "Greedy Solution Cost: " << greedy_cost << std::endl;
            
            std::cout << std::endl;

            std::cout << "Min Cost Flow Solution Feasible: " << checker.checkFeasibility(instance, min_cost_flow_solution) << std::endl;
            double min_cost_flow_cost = checker.getSolutionCost(instance, min_cost_flow_solution);
            std::cout << "Min Cost Flow Objective Value: " << min_cost_flow_solver.getObjectiveValue() << std::endl;
            std::cout << "Min Cost Flow Solution Cost: " << min_cost_flow_cost << std::endl;

            std::cout << std::endl;

        }
    }
}


int main(int argc, char** argv) {
    //check_small1();

    batch_check();

    // std::string filename = "input/medium_0.csv";

    // TaxiAssignmentInstance instance(filename);
    // std::cout << filename << std::endl;

    // std::cout << "Number of Taxis: " << instance.n << std::endl;

    // // Greedy Solver

    // GreedySolver solver(instance);

    // solver.solve();

    // TaxiAssignmentSolution greedy_solution = solver.getSolution();

    // std::cout << "Greedy Solution: " << std::endl;
    // std::cout << greedy_solution << std::endl;

    // std::cout << "Objective Value: " << solver.getObjectiveValue() << std::endl;
    // std::cout << "Solution Time: " << solver.getSolutionTime() << std::endl;

    // TaxiAssignmentChecker greedy_checker = TaxiAssignmentChecker();

    // std::cout << greedy_checker.checkFeasibility(instance, greedy_solution) << std::endl;

    //double greedy_cost = greedy_checker.getSolutionCost(instance, greedy_solution);

    //std::cout << "Greedy Solution Cost: " << greedy_cost << std::endl;


    // MinCostFlowSolver min_cost_flow_solver(instance);

    // min_cost_flow_solver.solve();

    // TaxiAssignmentSolution min_cost_flow_solution = min_cost_flow_solver.getSolution();

    // std::cout << "Min Cost Flow Solution: " << std::endl;
    // std::cout << min_cost_flow_solution << std::endl;

    // std::cout << "Objective Value: " << min_cost_flow_solver.getObjectiveValue() << std::endl;
    // std::cout << "Solution Time: " << min_cost_flow_solver.getSolutionTime() << std::endl;

    // TaxiAssignmentChecker checker = TaxiAssignmentChecker();

    // std::cout << checker.checkFeasibility(instance, min_cost_flow_solution) << std::endl;

    // double min_cost_flow_cost = checker.getSolutionCost(instance, min_cost_flow_solution);

    // std::cout << "Min Cost Flow Solution Cost: " << min_cost_flow_cost << std::endl;

    return 0;
}
