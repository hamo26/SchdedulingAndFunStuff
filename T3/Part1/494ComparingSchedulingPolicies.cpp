// 494CompaingSchedulingPolicies.cpp : Defines the entry point for the console application.
//


#include "TaskGenerator.h"
#include "TaskSetInputParser.h"
#include "AlgorithmAnalyzers.h"
#include <cstdlib>
#include <sstream>
#include <fstream>


#define NUMBER_OF_TASKS_SETS 10

using namespace std;

int main(int argc, char** argv)
{
	int numTasks = strtod(argv[1], NULL);

	char tempFile [] = "tempFile.txt";
	TaskSetInputParser parser;
	
	stringstream csvFileName;
	csvFileName << "datapoints_number_tasks_" << numTasks;

	ofstream outputFile;
	outputFile.open((char*)csvFileName.str().c_str());

	for (double currentIncrement = 0.05; currentIncrement<=1.05; currentIncrement+=0.05) {
		TaskGenerator::generateTasksAndWriteToFile(tempFile, currentIncrement, numTasks, NUMBER_OF_TASKS_SETS); 
		
		parser.parseInputFile(tempFile);
		
		double setsScheduable = 0;
		int totalTaskSets = parser.getTaskSetSize();

		while(!parser.isEmpty()) {
			TaskSet taskSet = parser.getNext();
			
			cout << "<Analyzing task set> \n";
			taskSet.printTaskSet();
			cout << "\n";
		
			taskSet.sortTaskSetByPeriod();
			WCRTAnalyzer rmwcrtAnalyzer(taskSet);

			taskSet.sortTaskSetByUtilization();
			WCRTAnalyzer mufwcrtAnalyzer(taskSet);

			taskSet.sortTaskSetByWCET();
			WCRTAnalyzer sjfwcrtAnalyzer(taskSet);

			if  (rmwcrtAnalyzer.isTaskSetScheduable() && mufwcrtAnalyzer.isTaskSetScheduable()
				&& sjfwcrtAnalyzer.isTaskSetScheduable()){ 
					cout << "TaskSet is Scheduable.\n";
					setsScheduable+=1; 
			} else {
				cout << "Taskset is not scheduable.\n";
			}
		}
		
		double percentScheduable = (setsScheduable / (double) totalTaskSets) * 100;
		cout << "Utilization: " << currentIncrement << " Percentage Scheduable: " 
			<< percentScheduable << "\n";
		outputFile << currentIncrement << " " << percentScheduable << "\n"; 
	}
	return 0;
}

