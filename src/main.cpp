#include "taxi_assignment_instance.h"
#include "taxi_assignment_solution.h"
#include "checker.h"
#include "greedy_solver.h"
#include "taxi_assignment_batching_solver.h"
#include "priority_solver.h"
#include "ortools/graph/min_cost_flow.h"

#include <filesystem>
#include <limits>

namespace fs = std::filesystem;

//https://stackoverflow.com/a/253874
//https://books.google.ca/books?ei=KRkLSYLZEJ7ktQOX3-ChBQ&id=T89QAAAAMAAJ&dq=%22essentially+equal%22+%22approximately+equal%22+knuth&q=%22essentially+equal%22+%22approximately+equal%22+%22definitely+less+than%22&pgis=1&hl=en#search
bool approximatelyEqual(float a, float b, float epsilon)
{
    /*
    *   Devuelve true si los dos floats son iguales dentro de un margen de error epsilon.
    */
    return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool essentiallyEqual(float a, float b, float epsilon)
{
    /*
    *   Devuelve true si los dos floats son iguales dentro de un margen de error epsilon.
    */
    return fabs(a - b) <= ( (fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

void single_check(std::string filename) {

    TaxiAssignmentInstance instance(filename);
    std::cout << filename << std::endl;
    std::cout << std::endl;


    TaxiAssignmentChecker checker = TaxiAssignmentChecker();


    // Greedy Solver

    GreedySolver greedy_solver(instance);

    greedy_solver.solve();

    assert(greedy_solver.getSolutionStatus() == operations_research::MinCostFlow::OPTIMAL || greedy_solver.getSolutionStatus() == operations_research::MinCostFlow::FEASIBLE);

    TaxiAssignmentSolution greedy_solution = greedy_solver.getSolution();

    std::cout << "Greedy Solution: " << std::endl;
    std::cout << greedy_solution << std::endl;

    std::cout << "Objective Value: " << greedy_solver.getObjectiveValue() << std::endl;
    std::cout << "Solution Time: " << greedy_solver.getSolutionTime() << std::endl;
    std::cout << std::endl;

    double greedy_cost = checker.getSolutionCost(instance, greedy_solution);
    assert(approximatelyEqual(greedy_cost, greedy_solver.getObjectiveValue(), 1e-5));


    // Batching Matching Solver

    BatchingSolver min_cost_flow_solver(instance);

    min_cost_flow_solver.solve();

    assert(min_cost_flow_solver.getSolutionStatus() == operations_research::MinCostFlow::OPTIMAL || min_cost_flow_solver.getSolutionStatus() == operations_research::MinCostFlow::FEASIBLE);

    TaxiAssignmentSolution min_cost_flow_solution = min_cost_flow_solver.getSolution();

    std::cout << "Batching Matching Solution: " << std::endl;
    std::cout << min_cost_flow_solution << std::endl;

    std::cout << "Objective Value: " << min_cost_flow_solver.getObjectiveValue() << std::endl;
    std::cout << "Solution Time: " << min_cost_flow_solver.getSolutionTime() << std::endl;
    std::cout << std::endl;
    
    double min_cost_flow_cost = checker.getSolutionCost(instance, min_cost_flow_solution);
    assert(approximatelyEqual(min_cost_flow_cost, min_cost_flow_solver.getObjectiveValue(), 1e-5));


    // Priority Solver

    PrioritySolver priority_solver(instance);

    priority_solver.solve();

    assert(priority_solver.getSolutionStatus() == operations_research::MinCostFlow::OPTIMAL || priority_solver.getSolutionStatus() == operations_research::MinCostFlow::FEASIBLE);

    TaxiAssignmentSolution priority_solution = priority_solver.getSolution();

    std::cout << "Priority Solution: " << std::endl;
    std::cout << priority_solution << std::endl;

    std::cout << "Objective Value: " << priority_solver.getObjectiveValue() << std::endl;
    std::cout << "Solution Time: " << priority_solver.getSolutionTime() << std::endl;
    std::cout << std::endl;
    
    double priority_cost = checker.getSolutionCost(instance, priority_solution);
    assert(approximatelyEqual(priority_cost, priority_solver.getObjectiveValue(), 1e-5));
    

    // Results

    std::cout << "Greedy Solution Cost: " << greedy_cost << std::endl;
    std::cout << "Batching Matching Solution Cost: " << min_cost_flow_cost << std::endl;
    std::cout << "Priority Solution Cost: " << priority_cost << std::endl;
}

void taxi_priority_cost(TaxiAssignmentSolution &priority_solution, TaxiAssignmentSolution &min_cost_flow_solution, TaxiAssignmentSolution &greedy_solution, TaxiAssignmentInstance &instance, std::ofstream &log_file, double priority_solver_taxi_cost){
    /*
    *  Dadas las soluciones greedy, batching y priority
    *  Calcula el Costo de Taxistas de cada una y lo escribe en el archivo de log_file.
    *  El Costo Unitario es la razon entre la distancia entre un taxista y su pasajero y la distancia del viaje del pasajero.
    *  El Costo se multiplica por 100 para trabajar con enteros, pero sin perder presición.
    *  Ademas permite tratar al costo unitario o ratio como un porcentaje.
    *  El Costo de Taxistas es la suma de los Costos Unitarios de cada asignación.
    */
    
    double avg_priority_ratio = 0;
    double avg_min_cost_flow_ratio = 0;
    double avg_greedy_ratio = 0;

    for (int taxi = 0; taxi < instance.n; taxi++){
    
        int priority_pax = priority_solution.getAssignedPax(taxi);
        double priority_trip_dist = instance.pax_trip_dist[priority_pax];
        double priority_search_dist = instance.dist[taxi][priority_pax];
        
        double priority_ratio;
        if (priority_trip_dist == 0){
            priority_ratio = 0;   
        }
        else{
            priority_ratio = 100 * (priority_search_dist / priority_trip_dist);
        }

        int min_cost_flow_pax = min_cost_flow_solution.getAssignedPax(taxi);
        double min_cost_flow_trip_dist = instance.pax_trip_dist[min_cost_flow_pax];
        double min_cost_flow_search_dist = instance.dist[taxi][min_cost_flow_pax];

        double min_cost_flow_ratio;
        if (min_cost_flow_trip_dist == 0){
            min_cost_flow_ratio = 0;
        }
        else{
            min_cost_flow_ratio = 100 * (min_cost_flow_search_dist / min_cost_flow_trip_dist);
        }

        int greedy_pax = greedy_solution.getAssignedPax(taxi);
        double greedy_trip_dist = instance.pax_trip_dist[greedy_pax];
        double greedy_search_dist = instance.dist[taxi][greedy_pax];

        double greedy_ratio;
        if (greedy_trip_dist == 0){
            greedy_ratio = 0;
        }
        else{
            greedy_ratio = 100 * (greedy_search_dist / greedy_trip_dist);
        }
        
        avg_priority_ratio += priority_ratio / instance.n;
        avg_min_cost_flow_ratio += min_cost_flow_ratio / instance.n;
        avg_greedy_ratio += greedy_ratio / instance.n;
    }

    log_file << instance.n << "," << avg_priority_ratio << "," << avg_min_cost_flow_ratio << "," << avg_greedy_ratio << std::endl;
}

void original_check(int n = 10, int sizes_n = 0, bool log = false){
    /*
    *   Corre el greedy solver, el batching solver y el priority solver
    *   Para los archivos de input/small_i.csv, input/medium_i.csv, input/large_i.csv, input/xl_i.csv
    *   Opctionalmente se puede especificar el numero de archivos a correr con n
    *   y el numero de tamaños de archivos a correr con sizes_n: 1 para small, 2 para small y medium, etc.
    *   Si sizes_n = 0, se corren todos los tamaños.
    * 
    *   Utiliza un TaxiAssignmentChecker para verificar las soluciones.
    *   Si los costos calculados internos a cada solver no son iguales a los calculados por el checker, sale del programa.
    * 
    *   Si log = true, guarda los resultados en output/results.csv y output/taxi_priorities_original.csv
    */
    std::ofstream log_file;
    std::ofstream taxi_priority_log_file;

    if (log){
        std::string log_results_filename = "output/results.csv";
        log_file.open(log_results_filename, std::ios_base::app);
        log_file << "filename,n,greedy_cost,min_cost_flow_cost,greedy_time,min_cost_flow_time,priority_cost,priority_time" << std::endl;

        std::string log_taxi_priority_filename = "output/taxi_priorities_original.csv";
        taxi_priority_log_file.open(log_taxi_priority_filename, std::ios_base::app);
        taxi_priority_log_file << "n,avg_priority_ratio,avg_min_cost_flow_ratio,avg_greedy_ratio" << std::endl;
    }
    std::vector<std::string> sizes = {"small", "medium", "large", "xl"};

    // Using a TaxiAssignmentChecker to check the solution
    TaxiAssignmentChecker checker = TaxiAssignmentChecker();

    if (sizes_n == 0){
        sizes_n = sizes.size();
    }

    for (int size_i = 0; size_i < sizes_n; size_i++){
        std::string filename_start = "input/" + sizes[size_i];

        for (int i = 0; i < n; i++){
            std::string filename = filename_start + "_" + std::to_string(i) + ".csv";

            TaxiAssignmentInstance instance(filename);
            
            std::cout << "\r" << filename << std::flush;


            // Greedy Solver
            
            GreedySolver greedy_solver(instance);

            greedy_solver.solve();

            assert(greedy_solver.getSolutionStatus() == operations_research::MinCostFlow::OPTIMAL || greedy_solver.getSolutionStatus() == operations_research::MinCostFlow::FEASIBLE);

            TaxiAssignmentSolution greedy_solution = greedy_solver.getSolution();

            //std::cout << "Greedy Solution Feasible: " << checker.checkFeasibility(instance, greedy_solution) << std::endl;
            double greedy_cost = checker.getSolutionCost(instance, greedy_solution);
            //std::cout << "Greedy Objective Value: " << greedy_solver.getObjectiveValue() << std::endl;
            //std::cout << "Greedy Solution Cost: " << greedy_cost << std::endl;
            //std::cout << std::endl;
            
            assert(approximatelyEqual(greedy_cost, greedy_solver.getObjectiveValue(), 1e-5));


            //Batching Matching Solver

            BatchingSolver min_cost_flow_solver(instance);

            min_cost_flow_solver.solve();

            assert(min_cost_flow_solver.getSolutionStatus() == operations_research::MinCostFlow::OPTIMAL || min_cost_flow_solver.getSolutionStatus() == operations_research::MinCostFlow::FEASIBLE);

            TaxiAssignmentSolution min_cost_flow_solution = min_cost_flow_solver.getSolution();

            //std::cout << "Batching Matching Solution Feasible: " << checker.checkFeasibility(instance, min_cost_flow_solution) << std::endl;
            double min_cost_flow_cost = checker.getSolutionCost(instance, min_cost_flow_solution);
            //std::cout << "Min Cost Flow Objective Value: " << min_cost_flow_solver.getObjectiveValue() << std::endl;
            //std::cout << "Batching Matching Solution Cost: " << min_cost_flow_cost << std::endl;
            //std::cout << std::endl;

            assert(approximatelyEqual(min_cost_flow_cost, min_cost_flow_solver.getObjectiveValue(), 1e-5));


            // Priority Solver
            
            PrioritySolver priority_solver(instance);

            priority_solver.solve();

            assert(priority_solver.getSolutionStatus() == operations_research::MinCostFlow::OPTIMAL || priority_solver.getSolutionStatus() == operations_research::MinCostFlow::FEASIBLE);

            TaxiAssignmentSolution priority_solution = priority_solver.getSolution();

            //std::cout << "Priority Solution Feasible: " << checker.checkFeasibility(instance, priority_solution) << std::endl;
            double priority_cost = checker.getSolutionCost(instance, priority_solution);
            //std::cout << "Priority Objective Value: " << priority_solver.getObjectiveValue() << std::endl;
            //std::cout << "Priority Solution Cost: " << priority_cost << std::endl;
            //std::cout << std::endl;

            assert(approximatelyEqual(priority_cost, priority_solver.getObjectiveValue(), 1e-5));
            

            // Taxi Priorities

            taxi_priority_cost(priority_solution, min_cost_flow_solution, greedy_solution, instance, taxi_priority_log_file, priority_solver.getTaxistObjectiveValue());


            // Log

            if (log)
                log_file << filename << "," << instance.n << "," << greedy_cost << "," << min_cost_flow_cost << "," << greedy_solver.getSolutionTime() << "," << min_cost_flow_solver.getSolutionTime() << "," << priority_cost << "," << priority_solver.getSolutionTime() << std::endl;
        }
    }

    if (log){
        log_file.close();
        taxi_priority_log_file.close();
    }
}

void random_check(std::string path){
    /*
    *   Corre el greedy solver, el batching solver y el priority solver
    *   Para los archivos de input/fake_instance_k_n.csv
    *   Llamamos fake_instances a estos archivos porque no son las instancias originales, sino que son instancias generadas aleatoriamente.
    * 
    *   Utiliza un TaxiAssignmentChecker para verificar las soluciones.
    *   Si los costos calculados internos a cada solver no son iguales a los calculados por el checker, sale del programa.
    *   Loguea los resultados en output/fake/results.csv y output/fake/taxi_priorities_random.csv
    */

    std::string log_results_filename = "output/fake/results.csv";
    
    std::ofstream log_file(log_results_filename);

    log_file << "filename,n,greedy_cost,min_cost_flow_cost,greedy_time,min_cost_flow_time,priority_cost,priority_time" << std::endl;

    std::string log_taxi_priority_filename = "output/fake/taxi_priorities_random.csv";

    std::ofstream taxi_priority_log_file(log_taxi_priority_filename);

    taxi_priority_log_file << "n,avg_priority_ratio,avg_min_cost_flow_ratio,avg_greedy_ratio" << std::endl;

    
    TaxiAssignmentChecker checker = TaxiAssignmentChecker();

    int fn = 0;

    for (const auto & entry : fs::directory_iterator(path)){

        std::string filename = entry.path().string();
        if (filename == "input/fake_instances/.DS_Store")
            continue;

        TaxiAssignmentInstance instance(filename);
        
        std::cout << "\rProgress: " << fn << "/5000 - " << filename << std::flush;
        fn++;

        // Greedy Solver
        
        GreedySolver greedy_solver(instance);

        greedy_solver.solve();

        assert(greedy_solver.getSolutionStatus() == operations_research::MinCostFlow::OPTIMAL || greedy_solver.getSolutionStatus() == operations_research::MinCostFlow::FEASIBLE);

        TaxiAssignmentSolution greedy_solution = greedy_solver.getSolution();

        //std::cout << "Greedy Solution Feasible: " << checker.checkFeasibility(instance, greedy_solution) << std::endl;
        double greedy_cost = checker.getSolutionCost(instance, greedy_solution);
        //std::cout << "Greedy Objective Value: " << greedy_solver.getObjectiveValue() << std::endl;
        //std::cout << "Greedy Solution Cost: " << greedy_cost << std::endl;
        //std::cout << std::endl;
        
        assert(approximatelyEqual(greedy_cost, greedy_solver.getObjectiveValue(), 1e-5));


        //Batching Matching Solver

        BatchingSolver min_cost_flow_solver(instance);

        min_cost_flow_solver.solve();

        assert(min_cost_flow_solver.getSolutionStatus() == operations_research::MinCostFlow::OPTIMAL || min_cost_flow_solver.getSolutionStatus() == operations_research::MinCostFlow::FEASIBLE);

        TaxiAssignmentSolution min_cost_flow_solution = min_cost_flow_solver.getSolution();

        //std::cout << "Batching Matching Solution Feasible: " << checker.checkFeasibility(instance, min_cost_flow_solution) << std::endl;
        double min_cost_flow_cost = checker.getSolutionCost(instance, min_cost_flow_solution);
        //std::cout << "Min Cost Flow Objective Value: " << min_cost_flow_solver.getObjectiveValue() << std::endl;
        //std::cout << "Batching Matching Solution Cost: " << min_cost_flow_cost << std::endl;

        //assert(approximatelyEqual(min_cost_flow_cost, min_cost_flow_solver.getObjectiveValue(), 1e-5));


        // Priority Solver

        PrioritySolver priority_solver(instance);

        priority_solver.solve();

        assert(priority_solver.getSolutionStatus() == operations_research::MinCostFlow::OPTIMAL || priority_solver.getSolutionStatus() == operations_research::MinCostFlow::FEASIBLE);

        TaxiAssignmentSolution priority_solution = priority_solver.getSolution();

        //std::cout << "Priority Solution Feasible: " << checker.checkFeasibility(instance, priority_solution) << std::endl;
        double priority_cost = checker.getSolutionCost(instance, priority_solution);
        //std::cout << "Priority Objective Value: " << priority_solver.getObjectiveValue() << std::endl;
        //std::cout << "Priority Solution Cost: " << priority_cost << std::endl;
        //std::cout << std::endl;

        assert(approximatelyEqual(priority_cost, priority_solver.getObjectiveValue(), 1e-5));


        // Taxi Priorities

        taxi_priority_cost(priority_solution, min_cost_flow_solution, greedy_solution, instance, taxi_priority_log_file, priority_solver.getTaxistObjectiveValue());


        // Log  

        log_file << filename << "," << instance.n << "," << greedy_cost << "," << min_cost_flow_cost << "," << greedy_solver.getSolutionTime() << "," << min_cost_flow_solver.getSolutionTime() << "," << priority_cost << "," << priority_solver.getSolutionTime() << std::endl;

    }
}

void write_solution_csv(std::string filename, TaxiAssignmentSolution &solution, TaxiAssignmentInstance &instance){
    /*
    *   Escribe la solucion en un archivo csv con el formato:
    *   taxi_id,pax_id,dist
    *   donde taxi_id es el id del taxi, pax_id es el id del pasajero asignado al taxi y dist es la distancia entre el taxi y el pasajero.
    */
    std::ofstream file(filename);

    file << "taxi_id,pax_id,dist" << std::endl;
    for (int i = 0; i < instance.n; i++){
        int taxi = i;
        int pax = solution.getAssignedPax(i);

        double dist = instance.dist[taxi][pax];

        file << taxi << "," << pax << "," << dist << std::endl;
    }

    file.close();
}

int main(int argc, char** argv) {
    
    // Evaluacion de soluciones

    if (argc >= 2 && std::string(argv[1]) == "-b"){
        // Evaluar soluciones de greedy, batching y priority en las 40 instancias de input/small
        std::cout << "Batch check" << std::endl;

        if (argc == 3 && std::string(argv[2]) == "-l")
            original_check(10, 4, true);
        else
            original_check(10, 4, false);
    }
    else if (argc == 3 && std::string(argv[1]) == "-s"){
        // Evaluar soluciones de greedy, batching y priority en un sola instancia
        
        std::cout << "Single check" << std::endl;
        
        single_check(argv[2]);

    }
    else if (argc == 3 && std::string(argv[1]) == "-f"){
        // Evaluar soluciones de greedy, batching y priority en las 5000 instancias de input/fake_instances
        
        std::cout << "Large check" << std::endl;

        random_check(argv[2]);
    }
    else{
        // Mostrar opciones
        std::cout << "Options: " << std::endl;
        std::cout << "    -b : Evaluar greedy, batching y priority en las 40 instancias de input/small" << std::endl;
        std::cout << "    -b -l : Evaluar greedy, batching y priority en las 40 instancias de input/small y loguear" << std::endl;
        std::cout << "    -s <filename> : Evaluar greedy, batching y priority en una sola instancia" << std::endl;
        std::cout << "    -f <path> : Evaluar greedy, batching y priority en todas las instancias en <path>" << std::endl;
        std::cout << std::endl;
        
        std::cout << "Corriendo Single Check Demo" << std::endl;
        std::cout << std::endl;
        
        single_check("input/small_0.csv");
    }

    return 0;
}
