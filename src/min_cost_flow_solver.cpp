#include "min_cost_flow_solver.h"


MinCostFlowSolver::MinCostFlowSolver() {}

MinCostFlowSolver::MinCostFlowSolver(TaxiAssignmentInstance &instance) {
    /*
    *   Crea una instancia del solver de Min-Cost Flow.
    */

    this->_instance = instance;
    this->_objective_value = 0;
    this->_cost_value = 0;
    this->_solution_status = 0;
    this->_solution_time = 0;
}

void MinCostFlowSolver::setInstance(TaxiAssignmentInstance &instance) {
    this->_instance = instance;
}

void MinCostFlowSolver::solve() {
    /*
    *   Resuelve el problema de asignación de taxis utilizando un grafo de flujo de costo mínimo.
    *   Guarda la solución en el atributo _solution.
    */

    // Time solution
    auto start = std::chrono::steady_clock::now();

    // Record the solution.
    this->_solution = TaxiAssignmentSolution(this->_instance.n);

    // Create the min cost network flow instance.
    this->_createMinCostFlowNetwork();

    // Obtain the solve the problem.
    int status = this->_min_cost_flow.Solve();

    // Obtain the solution, construct the corresponding object and record de desired parameters.
    if (status == operations_research::MinCostFlow::OPTIMAL) {
        //std::cout << "Minimum cost flow: " << this->_min_cost_flow.OptimalCost() << std::endl;
        //std::cout << "";
        //std::cout << " Arc   Flow / Capacity  Cost" << std::endl;
        for (std::size_t i = 0; i < this->_min_cost_flow.NumArcs(); ++i) {

            //std::cout << "Arc: " << i << std::endl;


            int64_t flow = this->_min_cost_flow.Flow(i);
            int64_t cost = flow * this->_min_cost_flow.UnitCost(i);

            if (flow == 0) continue;
            //if (cost == 0) continue;
            
            int taxi = this->_min_cost_flow.Tail(i);
            int pax = this->_min_cost_flow.Head(i) - this->_instance.n;
            

            // std::cout << p << " -> " << t
            //             << "  " << flow << "  / "
            //             << this->_min_cost_flow.Capacity(i) << "       " << cost << std::endl; 

            this->_solution.assign(taxi, pax);
            
            //this->_objective_value += this->_instance.dist[t][p];

            this->_objective_value += this->_min_cost_flow.UnitCost(i) / 10.0;
            int64_t trunk_cost = this->_instance.dist[taxi][pax] * 10;
            this->_cost_value += trunk_cost / 10.0;

            //std::cout << p << " -> " << t << std::endl;
            //std::cout << "Trunk Cost: " << trunk_cost / 10.0 << " - Dist: " << this->_instance.dist[t][p] << std::endl;
            //std::cout << this->_instance.dist[t][p] << "\t" << this->_min_cost_flow.UnitCost(i) / 10.0 << std::endl;
            // Compare UnitCost with dist
            
            //std::cout << "Unit Cost: " << this->_min_cost_flow.UnitCost(i) << " - Dist: " << this->_instance.dist[p][t] * 10 << std::endl;

            //this->_objective_value += cost / 10;

            //std::cout << "Objective Value: " << this->_objective_value << std::endl;
           
        }
    } else {
        std::cout << "Solving the min cost flow problem failed. Solver status: "
                << status << std::endl;
    }

    // Record the solution status.
    this->_solution_status = status;

    // Record the solution time.
    auto end = std::chrono::steady_clock::now();
    this->_solution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

}

double MinCostFlowSolver::getObjectiveValue() const {
    return this->_objective_value;
}

TaxiAssignmentSolution MinCostFlowSolver::getSolution() const {
    return this->_solution;
}

int MinCostFlowSolver::getSolutionStatus() const {
    return this->_solution_status;
}

double MinCostFlowSolver::getSolutionTime() const {
    return this->_solution_time;
}

// Private methods

void MinCostFlowSolver::_createMinCostFlowNetwork() {

    // Initialize graph structures.
    int n = this->_instance.n;
    std::vector<int64_t> start_nodes(n*n, -1);
    std::vector<int64_t> end_nodes(n*n, -1);
    std::vector<int64_t> capacities(n*n, 1);
    std::vector<int64_t> unit_costs(n*n, -1);

    // Complete the graph structures. 
    // Origin vertices (taxis) indexed from 0...n-1. 
    // Destination vertices (paxs) indexed from n...2n-1
    // unit_cost of (i,j) = dist[i][j]
    int cnt = 0;
    for (int i = 0; i < this->_instance.n; i++) {
        for (int j = this->_instance.n; j < 2*this->_instance.n; j++) {
            // capacities are always 1, defined when initialized.
            start_nodes[cnt] = i;
            end_nodes[cnt] = j;
            unit_costs[cnt] = 10*this->_instance.dist[i][j - n];
            cnt++;
        }
    }

    // Create the supplies.
    // supplies[i] = 1 for taxis, i = 0,...,n-1.
    // supplies[i] = -1 for paxs, i = n,...,2n-1.
    std::vector<int64_t> supplies(2*n, 0);
    for (int i = 0; i < this->_instance.n; i++) {
        supplies[i] = 1;
        supplies[n + i] = -1;
    }

    // Create the digraph
    // Add each arc.
    for (int i = 0; i < start_nodes.size(); ++i) {
        int arc = this->_min_cost_flow.AddArcWithCapacityAndUnitCost(start_nodes[i], end_nodes[i], capacities[i], unit_costs[i]);
        
        if (arc != i) LOG(FATAL) << "Internal error";
    }

    // Add node supplies.
    for (int i = 0; i < supplies.size(); ++i) {
        this->_min_cost_flow.SetNodeSupply(i, supplies[i]);
    }

    /* for (int i = 0; i < n*n; i++) {
        std::cout << unit_costs[i] << " ";
    } */
    std::cout << std::endl;

}

void MinCostFlowSolver::_createSolutionInfo() {

}
