#include "greedy_solver.h"

GreedySolver::GreedySolver() {
    /*
    *   Crea una instancia de la clase GreedySolver.
    */
    this->_objective_value = 0;
    this->_solution_status = 0;
    this->_solution_time = 0;
    this->_is_instance_set = false;
}

GreedySolver::GreedySolver(TaxiAssignmentInstance &instance) {
    /*
    *   Crea una instancia de la clase GreedySolver para resolver la instancia dada.
    *   La resolución corresponde con la estrategia FCFS:
    *       - En orden ascendente de pasajeros, 
    *       - a cada pasajero asignarle el taxi mas cercano que no haya sido asignado a otro pasajero
    */

    this->_instance = instance;
    this->_is_instance_set = true;

    this->_objective_value = 0;
    /* 
    *   NOT SOLVED = 0
    *   OPTIMAL = 1
    *   FEASIBLE = 2
    *   INFEASIBLE = 3
    *   UNBALANCED = 4
    *   BAD_RESULT = 5
    *   BAD COST RANGE = 6 
    */
    this->_solution_status = 0;
    this->_solution_time = 0;
}

void GreedySolver::setInstance(TaxiAssignmentInstance &instance) {
    this->_instance = instance;
    this->_is_instance_set = true;
}

void GreedySolver::solve() {
    /*
    *   Resuelve la instancia recibida por parametro al constructor de forma greedy.
    *   Guarda la solucion en el atributo _solution.
    */
    
    // this->_instance, es la instancia de tipo TaxiAssignmentInstance
    // Al pasajero 0 le asigno el taxi mas cercano
    // Al pasajero 1 le asigno el taxi mas cercano de los restantes

    // Si no existe la instancia, no se puede resolver
    
    if (!this->_is_instance_set) {
        this->_solution_status = 0;
        return;
    }

    // Time solution
    //std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    auto start = std::chrono::steady_clock::now();
    
    this->_solution = TaxiAssignmentSolution(this->_instance.n);

    for (int p = 0; p < this->_instance.n; p++){
        
        int min_tax = -1;

        while (min_tax == -1){
            for (int t = 0; t < this->_instance.n; t++){
                if (!this->_solution.isTaxiAssigned(t)){
                    min_tax = t;
                    break;
                }
            }
        }

        double min_di = this->_instance.dist[min_tax][p];

        for (int t = 0; t < this->_instance.n; t++){

            if (this->_instance.dist[t][p] < min_di && !this->_solution.isTaxiAssigned(t)){
                min_tax = t;
                min_di = this->_instance.dist[t][p];
            }
        }

        if (min_tax == -1){
            this->_solution_status = 4;
            continue;
        }

        this->_solution.assign(min_tax, p);

        // El valor objetivo tiene en cuenta la distancia recorrida por el taxi para ir a buscar al pasajero
        // Esta operación se podría hacer despues de asignar todos los pasajeros, y luego de timear el algoritmo
        this->_objective_value += this->_instance.dist[min_tax][p];//* fare_per_km;

        // Solution Status
        //TaxiAssignmentChecker checker = TaxiAssignmentChecker();
        
        //this->_solution_status = checker.checkFeasibility(this->_instance, this->_solution) ? 2 : 3;
        this->_solution_status = 2;
    }

    // Time solution
    //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();
    this->_solution_time = std::chrono::duration<double, std::milli>(end - start).count();

}

double GreedySolver::getObjectiveValue() const {
    return this->_objective_value;
}

TaxiAssignmentSolution GreedySolver::getSolution() const {
    return this->_solution;
}

int GreedySolver::getSolutionStatus() const {
    /*
    *   NOT SOLVED = 0
    *   OPTIMAL = 1
    *   FEASIBLE = 2
    *   INFEASIBLE = 3
    *   UNBALANCED = 4
    *   BAD_RESULT = 5
    *   BAD COST RANGE = 6
    */
    return this->_solution_status;
}

double GreedySolver::getSolutionTime() const {
    return this->_solution_time;
}
