analyze: Task.o TaskSet.o TaskSetInputParser.o HBAnalyzer.o LALAnalyzer.o analyze.o WCRTAnalyzer.o
	g++ -o analyze Task.o TaskSet.o TaskSetInputParser.o HBAnalyzer.o LALAnalyzer.o analyze.o WCRTAnalyzer.o -lrt

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

analyze.o: analyze.cpp
	g++ -c analyze.cpp
