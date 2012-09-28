#include "stdafx.h"
#include <math.h>

#include "AlgorithmAnalyzers.h"

bool HBAnalyzer::isTestValid() 
{
	stack<Task> taskSet = t_taskSet.sortTaskSetByPeriod();
	bool isTestValid = true;

	while(!taskSet.empty()) {
		Task topTask = taskSet.top();
		if (topTask.getRelativeDeadline() < topTask.getPeriod()) {
			isTestValid = false;
			break;
		}
		taskSet.pop();
	}
	return isTestValid;
}

bool HBAnalyzer::isTaskSetScheduable() 
{
	stack<Task> taskSet = t_taskSet.sortTaskSetByPeriod();
	bool isTaskSetScheduable = true;

	int taskSetSize = taskSet.size();
	double taskSetUtilization = 1;
	
	//Accumulate utilization of task set.
	while(!taskSet.empty()) {
		Task topTask = taskSet.top();
		taskSetUtilization*=(topTask.getUtlization()+1);
		taskSet.pop();
	}

	return taskSetUtilization <= 2;
}