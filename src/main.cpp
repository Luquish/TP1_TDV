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

//https://stackoverflow.com/a/253874
//https://books.google.ca/books?ei=KRkLSYLZEJ7ktQOX3-ChBQ&id=T89QAAAAMAAJ&dq=%22essentially+equal%22+%22approximately+equal%22+knuth&q=%22essentially+equal%22+%22approximately+equal%22+%22definitely+less+than%22&pgis=1&hl=en#search
bool approximatelyEqual(float a, float b, float epsilon)
{
    return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool essentiallyEqual(float a, float b, float epsilon)
{
    return fabs(a - b) <= ( (fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

void batch_check(int n = 10, int sizes_n = 0){
    std::vector<std::string> sizes = {"small", "medium", "large", "xl"};

    TaxiAssignmentChecker checker = TaxiAssignmentChecker();

    if (sizes_n == 0){
        sizes_n = sizes.size();
    }

    for (int size_i = 0; size_i < sizes_n; size_i++){
        std::string filename_start = "input/" + sizes[size_i];

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
            
            //assert(greedy_cost == solver.getObjectiveValue());
            // Convert to int64_t to avoid floating point errors in assertion

            /* int64_t greedy_cost_int = (int64_t) (greedy_cost * 10);
            int64_t solver_objective_value_int = (int64_t) (solver.getObjectiveValue() * 10);

            std::cout << solver_objective_value_int << std::endl;
            std::cout << greedy_cost_int << std::endl;

            assert(greedy_cost_int == solver_objective_value_int); */
            
            assert(approximatelyEqual(greedy_cost, solver.getObjectiveValue(), 1e-5));


            std::cout << std::endl;

            std::cout << "Min Cost Flow Solution Feasible: " << checker.checkFeasibility(instance, min_cost_flow_solution) << std::endl;
            double min_cost_flow_cost = checker.getSolutionCost(instance, min_cost_flow_solution);
            std::cout << "Min Cost Flow Objective Value: " << min_cost_flow_solver.getObjectiveValue() << std::endl;
            std::cout << "Min Cost Flow Solution Cost*: " << min_cost_flow_solver._cost_value << std::endl;
            std::cout << "Min Cost Flow Solution Cost: " << min_cost_flow_cost << std::endl;

            //assert(min_cost_flow_cost == min_cost_flow_solver.getObjectiveValue());

            /* int64_t min_cost_flow_cost_int = (int64_t) (min_cost_flow_cost * 10);
            int64_t min_cost_flow_solver_objective_value_int = (int64_t) (min_cost_flow_solver.getObjectiveValue() * 10);

            assert(min_cost_flow_cost_int == min_cost_flow_solver_objective_value_int); */

            assert(approximatelyEqual(min_cost_flow_cost, min_cost_flow_solver.getObjectiveValue(), 1e-5));

            std::cout << std::endl;

        }
    }
}

void write_solution_csv(std::string filename, TaxiAssignmentSolution &solution, TaxiAssignmentInstance &instance){
    std::ofstream file(filename);

    file << "taxi_id,pax_id,dist" << std::endl;
    //std::cout << solution << std::endl;
    for (int i = 0; i < instance.n; i++){
        int taxi = i;
        int pax = solution.getAssignedPax(i);

        double dist = instance.dist[taxi][pax];

        //std::cout << taxi << "," << pax << "," << dist << std::endl;

        file << taxi << "," << pax << "," << dist << std::endl;
    }

    file.close();
}


int main(int argc, char** argv) {
    //check_small1();

    batch_check();

    // std::string filename = "input/small_0.csv";

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

    // Write to CSV

    // std::string output_filename = "output/small_0_data.csv";

    // write_solution_csv(output_filename, min_cost_flow_solution, instance);

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
