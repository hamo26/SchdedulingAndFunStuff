// 494CompaingSchedulingPolicies.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "TaskGenerator.h"
#include "TaskSetInputParser.h"
#include "AlgorithmAnalyzers.h"
#include <sstream>
#include <fstream>
#include <iostream>


#define NUMBER_OF_TASKS_SETS 100000

using namespace std;

int main(int argc, char** argv)
{
	int numTasks = strtod(argv[1], NULL);

	char tempFile[] = "tempFile.txt";
	TaskSetInputParser parser;
	
	stringstream csvFileName;
	csvFileName << "datapoints_number_tasks_" << numTasks;

	ofstream outputFile;
	outputFile.open((char*)csvFileName.str().c_str());
	outputFile << "Utilization,ScheduableByRM,ScheduableBySJF,ScheduableByMUF\n"; 

	for (double currentIncrement = 0.05; currentIncrement<=1.05; currentIncrement+=0.05) {
		TaskGenerator::generateTasksAndWriteToFile(tempFile, currentIncrement, numTasks, NUMBER_OF_TASKS_SETS); 
		
		parser.parseInputFile(tempFile);
		
		double setsScheduableByRm = 0;
		double setsScheduableBySJF = 0;
		double setsScheduableByMUF = 0;

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

			if  (lalAnalyzer.isTaskSetScheduable() || hbAnalyzer.isTaskSetScheduable() 
				|| rmwcrtAnalyzer.isTaskSetScheduable()) {
					setsScheduableByRm+=1;
			}
			
			if (sjfwcrtAnalyzer.isTaskSetScheduable()){ 
				setsScheduableBySJF+=1;
			}

			if (mufwcrtAnalyzer.isTaskSetScheduable()) {
				setsScheduableByMUF+=1;
			}
		}
		
		cout << "Utilization: " << currentIncrement << " Task Sets Scheduable by RM: " 
			<< setsScheduableByRm << " by SJF: " << setsScheduableBySJF << " by MUF: " << setsScheduableByMUF << "\n";
		outputFile << currentIncrement << "," << setsScheduableByRm << "," << setsScheduableBySJF << "," 
			<< setsScheduableByMUF << "\n"; 
	}
	return 0;
}

