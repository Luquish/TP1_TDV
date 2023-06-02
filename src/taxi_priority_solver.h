#ifndef TAXI_PRIORITY_SOLVER_H
#define TAXI_PRIORITY_SOLVER_H

#include <cstdint>
#include <vector>
#include <iostream>
#include "taxi_assignment_instance.h"
#include "taxi_assignment_solution.h"
#include "ortools/graph/min_cost_flow.h"


class TaxiPrioritySolver
{
	public:
        TaxiPrioritySolver();
        TaxiPrioritySolver(TaxiAssignmentInstance &instance);

        void setInstance(TaxiAssignmentInstance &instance);
        void solve();

        double getObjectiveValue() const;
        double getTaxistObjectiveValue() const;
        TaxiAssignmentSolution getSolution() const;
        int getSolutionStatus() const;
        double getSolutionTime() const;
        
	private:

        operations_research::SimpleMinCostFlow _min_cost_flow;
        void _createMinCostFlowNetwork();

        // Instance, problem and results attributes
        TaxiAssignmentInstance _instance;
        TaxiAssignmentSolution _solution;
        double _objective_value;
        double _taxist_objective_value;
        int _solution_status;
        double _solution_time;

};

#endif
