#include "stdafx.h"
#include <math.h>

#include "AlgorithmAnalyzers.h"

bool WCRTAnalyzer::isTaskSetScheduable() 
{
	stack<Task> taskSet = t_taskSet.reverseStackOrdering();
	
	while(!taskSet.empty()) {
		Task currentTask = taskSet.top();
		taskSet.pop();

		double interference = 0;
		stack<Task> tempTaskSet = taskSet;
		while(!tempTaskSet.empty()) {
			interference+=tempTaskSet.top().getWorstCaseExecutionTime();
			tempTaskSet.pop();
		}
		
		double responseTime = 0;
		do {
			responseTime = interference+currentTask.getWorstCaseExecutionTime();
			if (responseTime > currentTask.getRelativeDeadline()) {
				return false;
			} else {
				tempTaskSet = taskSet;
				interference = 0;
				while(!tempTaskSet.empty()) {
					Task topTask = tempTaskSet.top();
					interference+=(ceil(responseTime / (double) topTask.getPeriod()) * topTask.getWorstCaseExecutionTime()); 
					tempTaskSet.pop();
				}
			}
		} while((interference + currentTask.getWorstCaseExecutionTime()) > responseTime);
	}
	return true;
}