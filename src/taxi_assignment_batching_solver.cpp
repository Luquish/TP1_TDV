#include "taxi_assignment_batching_solver.h"


BatchingSolver::BatchingSolver() {
    /*
    *   Crea una instancia del solver de Min-Cost Flow (Batching).
    */
    this->_objective_value = 0;
    this->_solution_status = 0;
    this->_solution_time = 0;
}

BatchingSolver::BatchingSolver(TaxiAssignmentInstance &instance) {
    /*
    *   Crea una instancia del solver de Min-Cost Flow (Batching).
    *   Guarda la instancia en el atributo _instance.
    */
    this->_instance = instance;
    this->_objective_value = 0;
    this->_solution_status = 0;
    this->_solution_time = 0;
}

void BatchingSolver::setInstance(TaxiAssignmentInstance &instance) {
    /*
    *   Guarda la instancia en el atributo _instance.
    */
    this->_instance = instance;
}

void BatchingSolver::solve() {
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
    // Pero consideramos que rellenar la solución compone terminar de resolver el problema de Batching.

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

            // Modificar el costo de la solución.
            this->_objective_value += this->_min_cost_flow.UnitCost(i) / 10.0;
        }
    } else {
        std::cout << "Solving the min cost flow problem failed. Solver status: "
                << this->_solution_status << std::endl;
    }

    auto end = std::chrono::steady_clock::now();
    //this->_solution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    this->_solution_time = std::chrono::duration<double, std::milli>(end - start).count();
}


void BatchingSolver::_createMinCostFlowNetwork() {
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
    * 10 es un factor de escala para que los costos sean enteros
    */

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
double BatchingSolver::getObjectiveValue() const {
    return this->_objective_value;
}

TaxiAssignmentSolution BatchingSolver::getSolution() const {
    return this->_solution;
}

int BatchingSolver::getSolutionStatus() const {
    return this->_solution_status;
}

double BatchingSolver::getSolutionTime() const {
    return this->_solution_time;
}


// -----------------------------

/* 

Explicación de BatchingSolver:

BatchingSolver es una clase que resuelve el problema de asignación de taxis utilizando un grafo de flujo de costo mínimo.
Para esto, utiliza la librería OR-Tools de Google.

El constructor de BatchingSolver recibe una instancia del problema de asignación de taxis y la guarda en el atributo _instance.
El método solve() resuelve el problema de asignación de taxis utilizando un grafo de flujo de costo mínimo.
El grafo de flujo de costo mínimo se crea en el método _createMinCostFlowNetwork().

_createMinCostFlowNetwork crea una red de flujo de costo mínimo utilizando la instancia guardada en _instance de la siguiente forma:

- Crea un grafo con 2n nodos, donde los primeros n nodos representan a los taxis y los últimos n nodos representan a los pasajeros.
- Crea un arco entre cada taxi y cada pasajero, de costo 10 * distancia(taxi, pasajero) y capacidad 1.
- Crea un nodo source con supply 1 y un nodo sink con supply -1.

Luego de crear la red de flujo de costo mínimo, se resuelve el problema utilizando el método Solve() de la librería OR-Tools.

El método Solve() devuelve un status que indica si la solución es óptima o no.

Si la solución es óptima, se recorren todos los arcos de la red de flujo de costo mínimo y se agregan a la solución los arcos que tienen flujo mayor a 0.

El costo de la solución se calcula sumando los costos de los arcos de la solución.


*/
