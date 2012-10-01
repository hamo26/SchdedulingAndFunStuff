// 494CompaingSchedulingPolicies.cpp : Defines the entry point for the console application.
//


#include "TaskGenerator.h"
#include "TaskSetInputParser.h"
#include "Simulator.h"
#include <sstream>
#include <fstream>


#define NUMBER_OF_TASKS_SETS 1
#define NUMBER_OF_TASKS 1

using namespace std;

int main(int argc, char** argv)
{

	char tempFile [] = "tempFile.txt";
	TaskSetInputParser parser;
	
	stringstream csvFileName;
	csvFileName << "datapoints_number_tasks_" << NUMBER_OF_TASKS;

	ofstream outputFile;
	outputFile.open((char*)csvFileName.str().c_str());

	Simulator s;

	for (double currentIncrement = 0.1; currentIncrement<=1; currentIncrement+=0.1) {
		TaskGenerator::generateTasksAndWriteToFile(tempFile, currentIncrement, NUMBER_OF_TASKS, NUMBER_OF_TASKS_SETS);
		
		parser.parseInputFile(tempFile);
		
		int totalTaskSets = parser.getTaskSetSize();

		while(!parser.isEmpty()) {
			TaskSet taskSet = parser.getNext();
			
			cout << "<Analyzing task set> \n";
			taskSet.printTaskSet();
			cout << "\n";

//			taskSet.sortTaskSetByUtilization();
//			s.MUF(taskSet);
			
			taskSet.sortTaskSetByPeriod();
			double d = s.RM(taskSet);

//			taskSet.sortTaskSetByWCET();
//			s.SJF(taskSet);
		}

	}
	return 0;
}

