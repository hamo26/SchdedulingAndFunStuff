#include "stdafx.h"
#include <math.h>

#include "AlgorithmAnalyzers.h"


bool HBAnalyzer::isTaskSetScheduable() 
{
	stack<Task> taskSet = t_taskSet.sortTaskSetByPeriod();
	bool isTaskSetScheduable = true;

	int taskSetSize = taskSet.size();
	double taskSetUtilization = 1;
	
	bool isAdjustedAlgorithm = AlgorithmHelper::isDeadlineSmallerThanPeriod(t_taskSet);

	//Accumulate utilization of task set.
	while(!taskSet.empty()) {
		Task topTask = taskSet.top();
		
		if (isAdjustedAlgorithm) {
			taskSetUtilization*=(1 + (topTask.getWorstCaseExecutionTime() / topTask.getRelativeDeadline()));
		} else {
			taskSetUtilization*=(topTask.getUtlization()+1);
		}
		taskSet.pop();
	}

	return taskSetUtilization <= 2;
}