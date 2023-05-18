#include "greedy_solver.h"

GreedySolver::GreedySolver() {}

GreedySolver::GreedySolver(TaxiAssignmentInstance &instance) {
    this->_instance = instance;
    this->_objective_value = 0;
    this->_solution_status = 0;
    this->_solution_time = 0;
}

void GreedySolver::setInstance(TaxiAssignmentInstance &instance) {
    this->_instance = instance;
}

void GreedySolver::solve() {
    
    // this->_instance, es la instancia de tipo TaxiAssignmentInstance
    // Al pasajero 0 le asigno el taxi mas cercano
    // Al pasajero 1 le asigno el taxi mas cercano de los restantes

    // Time solution
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

        double min_di = this->_instance.dist[0][p];

        for (int t = 0; t < this->_instance.n; t++){
            if (this->_instance.dist[t][p] < min_di && !this->_solution.isTaxiAssigned(t)){
                min_tax = t;
                min_di = this->_instance.dist[t][p];
            }
        }

        // if (min_tax == -1){
        //     //std::cout << "No se encontro taxi para el pasajero " << p << std::endl;
        //     continue;
        // }
        
        this->_solution.assign(min_tax, p);

        //std::cout << "Taxi " << min_tax << " asignado al pasajero " << p << " con distancia " << min_di << std::endl;

        // if (this->_instance.pax_trip_dist[p] == 0){
        //     continue;
        // }

        //double fare_per_km = this->_instance.pax_tot_fare[p] / this->_instance.pax_trip_dist[p];
        // El valor objetivo tiene en cuenta, el precio que cuesta que el taxi vaya a buscar al pasajero
        // Si no hay que tener esto en cuenta, se puede no multiplicar por el precio por km y solo sumar la distancia
        this->_objective_value += this->_instance.dist[min_tax][p];//* fare_per_km;

    }

    // Time solution
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
    return this->_solution_status;
}

double GreedySolver::getSolutionTime() const {
    return this->_solution_time;
}
