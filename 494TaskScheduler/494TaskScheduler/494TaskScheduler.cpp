// 494TaskScheduler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TaskSetInputParser.h"
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	TaskSetInputParser parser;

	bool parseResult = parser.parseInputFile("C:\\Users\\Hamid\\Documents\\testFile.txt");
	if (!parseResult) {
		cout << "parse failed\n";
	} else {
		while (!parser.isEmpty()) {
			TaskSet taskSet = parser.getNext();
			stack<Task> sortedTasks = taskSet.sortTaskSetByPeriod();
			cout << "Printing TaskSet: \n" ;
			while (!sortedTasks.empty()) {
				Task task = sortedTasks.top();
				cout << "task: WCET: " << task.getWorstCaseExecutionTime() 
					<< "Relative Deadline: " << task.getRelativeDeadline()
					<< "Period: " << task.getPeriod() << "\n";
				sortedTasks.pop();
			}
		}
	}
	return 0;
}

