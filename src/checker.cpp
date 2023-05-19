#include "checker.h"

TaxiAssignmentChecker::TaxiAssignmentChecker() :_feasibility_status(std::vector<bool>(3,false)) {}

bool TaxiAssignmentChecker::checkFeasibility(const TaxiAssignmentInstance &instance, const TaxiAssignmentSolution &solution) {
    /*
    *   Devuelve true si la solucion es factible, false en caso contrario
    *   La solucion es factible si cumple con las siguientes restricciones:
    *      - Todos los valores de la solucion estan en el rango [0,n], donde n es la cantidad de taxis/pasajeros
    *      - Cada pasajero es asignado a un unico taxi
    *      - Cada taxi es asignado a un unico pasajero
    *      - Todos los taxis/pasajeros son asignados
    *    En caso de que la solucion no sea factible, se guarda en _feasibility_status el estado de cada restriccion
    */

    bool ret = true;

    std::vector<std::string> checks = {"VALUES_IN_RANGE", "PAX_UNIQUE", "TAXI_UNIQUE"};

    this->_feasibility_status[0] = _checkValuesInRange(instance, solution);
    this->_feasibility_status[1] = _checkPaxUnique(instance, solution);
    this->_feasibility_status[2] = _checkTaxiUnique(instance, solution);

    for (int i = 0; i < this->n_constraints; i++) {
        //std::cout << checks[i] << ": ";
        // if (this->_feasibility_status[i]) {
        //     std::cout << "OK" << std::endl;
        // } else {
        //     std::cout << "FAIL" << std::endl;
        // }
        ret = ret && this->_feasibility_status[i];
    }

    return ret;
}

double TaxiAssignmentChecker::getSolutionCost(const TaxiAssignmentInstance &instance, const TaxiAssignmentSolution &solution) {
    /*
    *   Devuelve el costo de la solucion. (suma de las distancias de cada taxi a su pasajero asignado)
    *   Si la solucion no es factible, devuelve -1.0
    *   Si la solucion es factible, devuelve el costo de la solucion
    */
    
    double ret = 0.0;
    
    int n = solution.getN();

    //std::cout << "Feasibility Check in getSolutionCost: " << std::endl;
    if (!checkFeasibility(instance, solution)) {
        return -1.0;
    }

    for (int taxi = 0; taxi < n; taxi++) {
        int pax = solution.getAssignedPax(taxi);
        ret += instance.dist[taxi][pax];
    }

    return ret;
}

bool TaxiAssignmentChecker::_checkValuesInRange(const TaxiAssignmentInstance &instance, const TaxiAssignmentSolution &solution){
    /*
    *   Devuelve true si todos los valores de la solucion estan en rango (0 <= taxi < n y 0 <= pax < n)
    *   Devuelve false si algun valor esta fuera de rango
    */

    bool ret = true;

    int max = solution.getN();

    for (int i = 0; i < max; i++) {
        int taxi = i;
        int pax = solution.getAssignedPax(taxi);
        //std::cout << "taxi: " << taxi << " pax: " << pax << std::endl;
        if (pax < 0) {
            //std::cout << "Unassigned taxi: " << std::endl;
            ret = false;
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
    /*
    *   Devuelve true si todos los pasajeros estan asignados a un solo taxi
    *   Devuelve false si algun pasajero no esta asignado o esta asignado a mas de un taxi
    */

    bool ret = true;

    int n = solution.getN();

    std::vector<bool> assigned_pax(n, false);

    for (int i = 0; i < n; i++) {
        int taxi = i;
        int pax = solution.getAssignedPax(taxi);
        
        if (pax < 0 || pax >= n) {
            ret = false;
        }

        if (assigned_pax[pax] == false) {
            assigned_pax[pax] = true;
        } else {
            //std::cout << "taxi: " << taxi << std::endl;
            //std::cout << "pax: " << pax << std::endl;
            ret = false;
        }
    }

    for (int i = 0; i < n; i++) {
        int pax = i;
        if (assigned_pax[pax] == false) {

            //std::cout << "Unassigned pax: " << pax << std::endl;
            ret = false;
        }
    }

    return ret;
}

bool TaxiAssignmentChecker::_checkTaxiUnique(const TaxiAssignmentInstance &instance, const TaxiAssignmentSolution &solution){
    /*
    *   Devuelve true si todos los taxis tienen un solo pasajero asignado
    *   Devuelve false si algun taxi no tiene pasajero asignado o si mas de un pasajero estan asignados al mismo taxi
    */

    bool ret = true;

    int n = solution.getN();

    std::vector<bool> assigned_taxi(n, false);

    for (int i = 0; i < n; i++) {
        int pax = i;
        int taxi = solution.getAssignedTaxi(pax);
        
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
