#include "taxi_priority_solver.h"
#include <limits>

TaxiPrioritySolver::TaxiPrioritySolver() {
    /*
    *   Crea una instancia del solver de Min-Cost Flow (TaxiPriority).
    */
    this->_objective_value = 0;
    this->_taxist_objective_value = 0;
    this->_solution_status = 0;
    this->_solution_time = 0;
}

TaxiPrioritySolver::TaxiPrioritySolver(TaxiAssignmentInstance &instance) {
    /*
    *   Crea una instancia del solver de Min-Cost Flow (TaxiPriority).
    *   Guarda la instancia en el atributo _instance.
    */
    this->_instance = instance;
    this->_objective_value = 0;
    this->_taxist_objective_value = 0;
    this->_solution_status = 0;
    this->_solution_time = 0;
}

void TaxiPrioritySolver::setInstance(TaxiAssignmentInstance &instance) {
    /*
    *   Guarda la instancia en el atributo _instance.
    */
    this->_instance = instance;
}

void TaxiPrioritySolver::solve() {
    /*
    *   Resuelve el problema de asignación de taxis utilizando un grafo de flujo de costo mínimo.
    *   Guarda la solución en el atributo _solution.
    */

    // Time solution
    auto start = std::chrono::steady_clock::now();

    // Record the solution.
    this->_solution = TaxiAssignmentSolution(this->_instance.n);

    // Crear la red de flujo de costo mínimo.
    this->_createMinCostFlowNetwork();

    // Resolver el problema. Guardar el status de la solución. 
    this->_solution_status = this->_min_cost_flow.Solve();

    // Para esta instancia del algoritmo, ya se podría interrumpir el timer
    // Pero consideramos que rellenar la solución compone terminar de resolver el problema de TaxiPriority.

    if (this->_solution_status == operations_research::MinCostFlow::OPTIMAL) {
        for (std::size_t i = 0; i < this->_min_cost_flow.NumArcs(); ++i) {
            int64_t flow = this->_min_cost_flow.Flow(i);
            int64_t cost = flow * this->_min_cost_flow.UnitCost(i);

            if (flow == 0) continue;

            // Obtener el taxi y el pasajero correspondiente al arco i.
            int pax = this->_min_cost_flow.Head(i) - this->_instance.n;
            int taxi = this->_min_cost_flow.Tail(i);

            // Agregar el arco a la solución.
            this->_solution.assign(taxi, pax);

            // Valor objetivo del problema de asignación de taxis.
            this->_objective_value += this->_instance.dist[taxi][pax];
            
            // Nuevo valor objetivo del problema de asignación de taxis centrado en el beneficio de los taxis.
            this->_taxist_objective_value += this->_min_cost_flow.UnitCost(i);
        }
    } else {
        std::cout << "Solving the taxi centered problem failed. Solver status: "
                << this->_solution_status << std::endl;
    }

    auto end = std::chrono::steady_clock::now();
    //this->_solution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    this->_solution_time = std::chrono::duration<double, std::milli>(end - start).count();
}

void TaxiPrioritySolver::_createMinCostFlowNetwork() {
    /*
    *   Crea la red de flujo de costo mínimo para el problema de asignación de taxis.
    *   Utiliza la instancia guardada en _instance.
    *   Guarda la red en el atributo _min_cost_flow.
    *   https://developers.google.com/optimization/flow/mincostflow
    */

    int n = this->_instance.n;
    std::vector<int64_t> start_nodes(n*n, -1);
    std::vector<int64_t> end_nodes(n*n, -1);
    std::vector<int64_t> capacities(n*n, 1);
    std::vector<int64_t> unit_costs(n*n, -1);

    /*
    * Rellenar start_nodes, end_nodes y unit_costs
    * Los vertices de la red se indexan de 0 a 2n-1
    * Los primeros n son los taxis [0, n-1]
    * Los ultimos n son los pasajeros [n, 2n-1]
    * Ademas, los costos entre taxis y pasajeros son 10 * distancia(taxi, pasajero)
    * 10 es un factor de escala para que los costos sean enteros y no decimales.
    */

    int cnt = 0;
    for (int taxi = 0; taxi < this->_instance.n; taxi++) {
        for (int j = this->_instance.n; j < 2*this->_instance.n; j++) {
            // capacities are always 1, defined when initialized.
            int pax = j - n;
            start_nodes[cnt] = taxi;
            end_nodes[cnt] = j;

            double trip_dist = this->_instance.pax_trip_dist[pax];
            double search_dist = this->_instance.dist[taxi][pax];


            if (trip_dist == 0) {
                // Make it very expensive
                unit_costs[cnt] = std::numeric_limits<int>::max();
            }
            else {
                //unit_costs[cnt] = 100 * (trip_dist / search_dist);
                unit_costs[cnt] = 100 * (search_dist / trip_dist);
            }
    
            cnt++;
        }
    }

    // Los supplies son 1 para los taxis y -1 para los pasajeros
    // 
    std::vector<int64_t> supplies(2*n, 0);
    for (int i = 0; i < this->_instance.n; i++) {
        supplies[i] = 1;
        supplies[n + i] = -1;
    }

    // Crea la red de flujo de costo mínimo.
    
    // Agrega los ejes entre cada taxi y cada pasajero, de costo 10 * distancia(taxi, pasajero) y capacidad 1. 
    for (int i = 0; i < start_nodes.size(); ++i) {
        int arc = this->_min_cost_flow.AddArcWithCapacityAndUnitCost(start_nodes[i], end_nodes[i], capacities[i], unit_costs[i]);
        
        if (arc != i) LOG(FATAL) << "Internal error";
    }

    // Agrega los supplies de cada nodo.
    // Los taxis tienen supply 1 y los pasajeros -1.
    for (int i = 0; i < supplies.size(); ++i) {
        this->_min_cost_flow.SetNodeSupply(i, supplies[i]);
    }
}
double TaxiPrioritySolver::getObjectiveValue() const {
    return this->_objective_value;
}

double TaxiPrioritySolver::getTaxistObjectiveValue() const {
    /*
    *   El Taxist Objective value es la nueva métrica a partir de la cual se evaluará la calidad de la solución.
    *   Tiene en cuenta el costo de búsqueda de los taxis y el costo de viaje de los pasajeros.
    */

    return this->_taxist_objective_value;
}

TaxiAssignmentSolution TaxiPrioritySolver::getSolution() const {
    return this->_solution;
}

int TaxiPrioritySolver::getSolutionStatus() const {
    return this->_solution_status;
}

double TaxiPrioritySolver::getSolutionTime() const {
    return this->_solution_time;
}
