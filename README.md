# Trabajo Práctico 1. Servicios de movilidad on-demand en tiempo real"

## Diseño de Algoritmos - Licenciatura en Tecnología Digital - UTDT


### Integrantes del grupo

- [Ignacio Pardo](https://https://github.com/IgnacioPardo)
- [Luca Mazzarello](https://github.com/Luquish)

### Descripción del problema

El problema a investigar consiste en la asignación de vehículos disponibles a pasajeros que demanden de un viaje.
Este trabajo presenta tres algoritmos con distintos enfoques para el problema de asignación de taxis.
Una heurística golosa que priorice los pasajeros por orden de llegada, un algoritmo que minimice las distancias recorridas por los taxis globales y un algoritmo que minimice los viajes menos rentables para los taxistas. 
El objetivo de este informe es el de detallar los algoritmos y compararlos en términos de performance y calidad de solución.

### Instancias

Las instancias de prueba se encuentran en la carpeta `input`.

Para la evaluación del problema, se cuenta con 4 sets de instancias de distintos tamaño, 10 por cada tamaño, para modelar escenarios variados de demanda. Las mismas deben ser utilizadas para comparar la efectividad de nuestro enfoque y realizar una comparación extensiva entre el método de FCFS y el de matching. Las características de las instancias son:

- `small`: $n = 10$;
- `medium`: $n = 100$;
- `large`: $n = 250$;
- `xl`: $n = 500$.

Ademas, se generaron ~500 sets de instancias, 10 por cada tamaño, para modelar escenarios variados de demanda. 
Se encuentran en la carpeta `input/fake_instances` y se generaron con el Jupiter Notebook `src/scripts/experimentación.ipynb`.

### Descripción de las soluciones

#### Heurística golosa: GreedySolver

El primer algoritmo que se implementó es una heurística greedy que prioriza los pasajeros de manera FCFS (First Come First Served), asignando el taxi más cercano al pasajero que lo solicitó primero.

#### Algoritmo de asignación de taxis globales, Batching Matching: BatchingSolver

El segundo algoritmo que se implementó es un algoritmo de asignación de taxis globales, que minimiza la distancia total recorrida por los taxis.

#### Costo de Taxistas: TaxiPrioritySolver

El tercer algoritmo que se implementó es un algoritmo de asignación de taxis globales, que minimiza el costo de los viajes menos rentables para los taxistas.

### Experimentación

Para la experimentación se utilizó el Jupiter Notebook `src/scripts/experimentación.ipynb`.

### Resultados

Los resultados de la experimentación estan detallados en el Informe `Informe.pdf`.

### Ejecución

Para ejecutar el programa, se debe compilar utilizando `make` y luego ejecutar el binario generado `taxi_batching`.
Ademas, se debe instalar la libreria Google `OR-TOOLS` y actualizar el path en el Makefile.
