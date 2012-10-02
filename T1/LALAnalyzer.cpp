#include <math.h>

#include "AlgorithmAnalyzers.h"


bool LALAnalyzer::isTaskSetScheduable() 
{
	stack<Task> taskSet = t_taskSet.sortTaskSetByPeriod();
	bool isTaskSetScheduable = true;
	
	
	int taskSetSize = taskSet.size();
	double utilizationBound = (double) taskSetSize*(pow(2.0, (1 / (double) taskSetSize))-1);
	double taskSetUtilization = 0;
	
	bool isAdjustedAlgorithm = AlgorithmHelper::isDeadlineSmallerThanPeriod(t_taskSet);
	
	//Accumulate utilization of task set.
	while(!taskSet.empty()) {
		Task topTask = taskSet.top();
		if (isAdjustedAlgorithm) {
			taskSetUtilization+=(topTask.getWorstCaseExecutionTime() / topTask.getRelativeDeadline());
		} else {
			taskSetUtilization+=topTask.getUtlization();
		}
		taskSet.pop();
	}

	return taskSetUtilization <= utilizationBound;
}
