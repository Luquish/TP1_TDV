CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=c++17
CFLAGS = -std=c++17
INCDIRS = libs/or-tools_arm64/include/
LIBDIRS = libs/or-tools_arm64/lib/
SRC = taxi_assignment_instance.cpp taxi_assignment_solution.cpp checker.cpp greedy_solver.cpp taxi_assignment_batching_solver.cpp taxi_priority_solver.cpp main.cpp 
OBJ = $(SRC:.cpp=.o)
EXEC = taxi_batching

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -L $(LIBDIRS) -lortools -o $(EXEC)

%.o: %.cpp
	$(CC) -I $(INCDIRS) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
