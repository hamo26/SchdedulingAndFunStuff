#include "stdafx.h"
#include <math.h>

#include "AlgorithmAnalyzers.h"

bool LALAnalyzer::isTestValid() 
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

bool LALAnalyzer::isTaskSetScheduable() 
{
	stack<Task> taskSet = t_taskSet.sortTaskSetByPeriod();
	bool isTaskSetScheduable = true;

	int taskSetSize = taskSet.size();
	double utilizationBound = taskSetSize*(pow(2,taskSetSize)-1);
	double taskSetUtilization = 0;
	
	//Accumulate utilization of task set.
	while(!taskSet.empty()) {
		Task topTask = taskSet.top();
		taskSetUtilization+=topTask.getUtlization();
		taskSet.pop();
	}

	return taskSetUtilization <= utilizationBound;
}