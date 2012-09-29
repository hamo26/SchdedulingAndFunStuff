// 494CompaingSchedulingPolicies.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "TaskGenerator.h"
#include "TaskSetInputParser.h"
#include "AlgorithmAnalyzers.h"
#include <sstream>
#include <fstream>


#define NUMBER_OF_TASKS_SETS 100000

using namespace std;

int main(int argc, char** argv)
{
	int numTasks = strtod(argv[1], NULL);

	string tempFile = "tempFile.txt";
	TaskSetInputParser parser;
	
	stringstream csvFileName;
	csvFileName << "datapoints_number_tasks_" << numTasks;

	ofstream outputFile;
	outputFile.open(csvFileName.str());

	for (double currentIncrement = 0.05; currentIncrement<=1.05; currentIncrement+=0.05) {
		TaskGenerator::generateTasksAndWriteToFile(tempFile, currentIncrement, numTasks, NUMBER_OF_TASKS_SETS); 
		
		parser.parseInputFile(tempFile);
		
		double setsScheduable = 0;
		int totalTaskSets = parser.getTaskSetSize();

		while(!parser.isEmpty()) {
			TaskSet taskSet = parser.getNext();
			
			LALAnalyzer lalAnalyzer(taskSet);
			HBAnalyzer hbAnalyzer(taskSet);
			
			taskSet.sortTaskSetByPeriod();
			WCRTAnalyzer rmwcrtAnalyzer(taskSet);

			taskSet.sortTaskSetByUtilization();
			WCRTAnalyzer mufwcrtAnalyzer(taskSet);

			taskSet.sortTaskSetByWCET();
			WCRTAnalyzer sjfwcrtAnalyzer(taskSet);

			if  (lalAnalyzer.isTaskSetScheduable() && hbAnalyzer.isTaskSetScheduable() 
				&& rmwcrtAnalyzer.isTaskSetScheduable() && mufwcrtAnalyzer.isTaskSetScheduable()
				&& sjfwcrtAnalyzer.isTaskSetScheduable()){ 
					setsScheduable+=1; 
			}
		}
		
		double percentScheduable = (setsScheduable / (double) totalTaskSets) * 100;
		cout << "Utilization: " << currentIncrement << " Percentage Scheduable: " 
			<< percentScheduable << "\n";
		outputFile << currentIncrement << " " << percentScheduable << "\n"; 
	}
	return 0;
}

