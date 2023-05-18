#include "checker.h"

TaxiAssignmentChecker::TaxiAssignmentChecker() :_feasibility_status(std::vector<bool>(3,false)) {}

bool TaxiAssignmentChecker::checkFeasibility(const TaxiAssignmentInstance &instance, const TaxiAssignmentSolution &solution) {

    bool ret = true;

    this->_feasibility_status[0] = _checkValuesInRange(instance, solution);
    this->_feasibility_status[1] = _checkPaxUnique(instance, solution);
    this->_feasibility_status[2] = _checkTaxiUnique(instance, solution);

    for (int i = 0; i < this->n_constraints; i++) {
        std::cout << "Constraint " << i << ": " << this->_feasibility_status[i] << std::endl;
        ret = ret && this->_feasibility_status[i];
    }

    return ret;
}

double TaxiAssignmentChecker::getSolutionCost(const TaxiAssignmentInstance &instance, const TaxiAssignmentSolution &solution) {
    double ret = 0.0;

    int n = solution.getN();

    if (!checkFeasibility(instance, solution)) {
        return -1.0;
    }

    for (int i = 0; i < n; i++) {
        int pax = solution.getAssignedPax(i);

        //std::cout << "p: " << pax << std::endl;
        //std::cout << "taxi: " << i << std::endl;
        //std::cout << "dist: " << instance.dist[i][pax] << std::endl;

        ret += instance.dist[i][pax];
    }

    return ret;
}

bool TaxiAssignmentChecker::_checkValuesInRange(const TaxiAssignmentInstance &instance, const TaxiAssignmentSolution &solution){

    bool ret = true;

    int max = solution.getN();

    for (int i = 0; i < max; i++) {
        int taxi = i;
        int pax = solution.getAssignedPax(taxi);

        if (pax < 0) {
            std::cout << "taxi: " << taxi << std::endl;
        }

        if (pax >= max) {
            ret = false;
        }
    }

    for (int i = 0; i < max; i++) {
        int pax = i;
        int taxi = solution.getAssignedTaxi(pax);

        if (taxi >= max) {
            ret = false;
        }
    }

    return ret;

}

bool TaxiAssignmentChecker::_checkPaxUnique(const TaxiAssignmentInstance &instance, const TaxiAssignmentSolution &solution){
    
    bool ret = true;

    int n = solution.getN();

    std::vector<bool> assigned_pax(n, false);

    for (int i = 0; i < n; i++) {
        int taxi = i;
        int pax = solution.getAssignedPax(taxi);
        
        // if (pax < 0 || pax >= n) {
        //     ret = false;
        // }

        if (assigned_pax[pax] == false) {
            assigned_pax[pax] = true;
        } else {

            std::cout << "taxi: " << taxi << std::endl;
            std::cout << "pax: " << pax << std::endl;
            ret = false;
        }
    }

    for (auto pax : assigned_pax) {
        if (pax == false) {
            ret = false;
        }
    }

    return ret;
}

bool TaxiAssignmentChecker::_checkTaxiUnique(const TaxiAssignmentInstance &instance, const TaxiAssignmentSolution &solution){
    
    bool ret = true;

    int n = solution.getN();

    std::vector<bool> assigned_taxi(n, false);

    for (int i = 0; i < n; i++) {
        int taxi = i;
        int pax = solution.getAssignedPax(taxi);
        
        // if (taxi < 0 || taxi >= n) {
        //     ret = false;
        // }

        if (assigned_taxi[taxi] == false) {
            assigned_taxi[taxi] = true;
        } else {
            ret = false;
        }
    }

    for (auto taxi : assigned_taxi) {
        if (taxi == false) {
            ret = false;
        }
    }

    return ret;

}
