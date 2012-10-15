simulator: Task.o TaskSet.o TaskSetInputParser.o Simulator.o 494ComparingSchedulingPolicies.o TaskGenerator.o
	g++ -o simulator Task.o TaskSet.o TaskSetInputParser.o Simulator.o 494ComparingSchedulingPolicies.o TaskGenerator.o

Task.o: Task.cpp Task.h
	g++ -c Task.cpp

TaskSet.o: TaskSet.cpp TaskSet.h
	g++ -c TaskSet.cpp

TaskSetInputParser.o: TaskSetInputParser.h TaskSetInputParser.cpp 
	g++ -c TaskSetInputParser.cpp

Simulator.o: Simulator.h Simulator.cpp
	g++ -c Simulator.cpp

TaskGenerator.o: TaskGenerator.h TaskGenerator.cpp
	g++ -c TaskGenerator.cpp

494ComparingSchedulingPolicies.o: 494ComparingSchedulingPolicies.cpp
	g++ -c 494ComparingSchedulingPolicies.cpp