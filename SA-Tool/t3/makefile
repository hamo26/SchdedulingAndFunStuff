graphingtool: Task.o TaskSet.o TaskSetInputParser.o HBAnalyzer.o LALAnalyzer.o WCRTAnalyzer.o 494ComparingSchedulingPolicies.o TaskGenerator.o
	g++ -o graphingtool Task.o TaskSet.o TaskSetInputParser.o HBAnalyzer.o LALAnalyzer.o WCRTAnalyzer.o 494ComparingSchedulingPolicies.o TaskGenerator.o

Task.o: Task.cpp Task.h
	g++ -c Task.cpp

TaskSet.o: TaskSet.cpp TaskSet.h
	g++ -c TaskSet.cpp

TaskSetInputParser.o: TaskSetInputParser.h TaskSetInputParser.cpp 
	g++ -c TaskSetInputParser.cpp

HBAnalyzer.o: AlgorithmAnalyzers.h HBAnalyzer.cpp
	g++ -c HBAnalyzer.cpp

LALAnalyzer.o: AlgorithmAnalyzers.h LALAnalyzer.cpp 
	g++ -c LALAnalyzer.cpp

WCRTAnalyzer.o: AlgorithmAnalyzers.h WCRTAnalyzer.cpp
	g++ -c WCRTAnalyzer.cpp

TaskGenerator.o: TaskGenerator.h TaskGenerator.cpp
	g++ -c TaskGenerator.cpp

494ComparingSchedulingPolicies.o: 494ComparingSchedulingPolicies.cpp
	g++ -c 494ComparingSchedulingPolicies.cpp