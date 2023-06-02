#include "checker.h"
#include <limits>

TaxiAssignmentChecker::TaxiAssignmentChecker() :_feasibility_status(std::vector<bool>(3,false)) {}

bool TaxiAssignmentChecker::checkFeasibility(const TaxiAssignmentInstance &instance, const TaxiAssignmentSolution &solution) {
    /*
    *   Devuelve true si la solución es factible, false en caso contrario
    *   La solución es factible si cumple con las siguientes restricciones:
    *      - Todos los valores de la solución están en el rango [0,n], donde n es la cantidad de taxis/pasajeros
    *      - Cada pasajero es asignado a un único taxi
    *      - Cada taxi es asignado a un único pasajero
    *      - Todos los taxis/pasajeros son asignados
    *    En caso de que la solución no sea factible, se guarda en _feasibility_status el estado de cada restricción
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
    *   Devuelve el costo de la solución. (suma de las distancias de cada taxi a su pasajero asignado)
    *   Si la solución no es factible, devuelve -1.0
    *   Si la solución es factible, devuelve el costo de la solución
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

double TaxiAssignmentChecker::getMeanTaxiPriorities(const TaxiAssignmentInstance &instance, const TaxiAssignmentSolution &solution){
    /*
    *  Dadas una solución para una instancia de TaxiAssignment, calcula el promedio del costo de prioridad de los taxis.
    *  El Costo Unitario es la razón entre la distancia entre un taxista y su pasajero y la distancia del viaje del pasajero.
    *  El Costo se multiplica por 100 para trabajar con enteros, pero sin perder precisión.
    *  Ademas permite tratar al costo unitario o ratio como un porcentaje.
    *  El Costo de Taxistas es la suma de los Costos Unitarios de cada asignación.
    *  Cuando el viaje del pasajero es de 0, el Costo Unitario de la asignación no se considera para el Costo de Taxistas total.
    *  A la hora de comparar este costo a través de las soluciones, todos las soluciones van a haber tenido los mismos pasajeros con los mismos viajes.
    */
    
    double avg_taxi_cost_ratio = 0;

    for (int taxi = 0; taxi < instance.n; taxi++){

        int pax = solution.getAssignedPax(taxi);
        double trip_dist = instance.pax_trip_dist[pax];
        double search_dist = instance.dist[taxi][pax];
        
        double ratio;
        if (trip_dist == 0){
            ratio = 0;   
        }
        else{
            ratio = 100 * (search_dist / trip_dist);
        }
        
        avg_taxi_cost_ratio += ratio / instance.n;
    }

    return avg_taxi_cost_ratio;
}

bool TaxiAssignmentChecker::_checkValuesInRange(const TaxiAssignmentInstance &instance, const TaxiAssignmentSolution &solution){
    /*
    *   Devuelve true si todos los valores de la solución están en rango (0 <= taxi < n y 0 <= pax < n)
    *   Devuelve false si algún valor esta fuera de rango
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
    *   Devuelve true si todos los pasajeros están asignados a un solo taxi
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
    *   Devuelve false si algún taxi no tiene pasajero asignado o si mas de un pasajero están asignados al mismo taxi
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
