#include <math.h>

#include "AlgorithmAnalyzers.h"
#include <iostream>

using namespace std;
bool WCRTAnalyzer::isTaskSetScheduable() 
{
	stack<Task> taskSet = t_taskSet.reverseStackOrdering();
	
	while(taskSet.size() > 1) {
		Task currentTask = taskSet.top();
		taskSet.pop();

		double interference = 0;
		stack<Task> tempTaskSet = taskSet;
		while(!tempTaskSet.empty()) {
			interference+=tempTaskSet.top().getWorstCaseExecutionTime();
			tempTaskSet.pop();
		}
		
		double responseTime = 0.0;
		do {
			responseTime = interference+currentTask.getWorstCaseExecutionTime();
			if (responseTime > currentTask.getRelativeDeadline()) {
				return false;
			} else {
				tempTaskSet = taskSet;
				interference = 0;
				while(!tempTaskSet.empty()) {
					Task topTask = tempTaskSet.top();
					cout << "\nPeriod: " << topTask.getPeriod();
					cout << "\nWCET: " << topTask.getWorstCaseExecutionTime();
					cout << "\nResponse Tim: " << responseTime;
					interference+=(ceil(responseTime / (double) topTask.getPeriod()) * topTask.getWorstCaseExecutionTime()); 
					cout << "\nINTERFERENCE: " << (ceil(responseTime / (double) topTask.getPeriod()) * topTask.getWorstCaseExecutionTime());	
					
					tempTaskSet.pop();
				}
			}
			cout << "\nR: " << responseTime << "\nI: " << interference << "\nRD: " << currentTask.getRelativeDeadline()
				<< "\nWC: " << currentTask.getWorstCaseExecutionTime();
		} while((interference + currentTask.getWorstCaseExecutionTime()) > responseTime);
	}
	return true;
}