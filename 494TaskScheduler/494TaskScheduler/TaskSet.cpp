#include "stdafx.h"
#include <algorithm>
#include "TaskSet.h"

using namespace std;

TaskSet::TaskSet(vector<Task> taskSet) 
{
	v_taskSet = taskSet;
}

bool compareTaskPeriods(Task taskA, Task taskB) { return (taskA.getPeriod() < taskB.getPeriod()); }

bool compareWCET(Task taskA, Task taskB) { return (taskA.getWorstCaseExecutionTime() < taskB.getWorstCaseExecutionTime()); }
	
bool compareUtilization(Task taskA, Task taskB) { return (taskA.getUtlization() < taskB.getUtlization()); }

stack<Task> TaskSet::sortTaskSetByUtilization()
{
	sort(v_taskSet.begin(), v_taskSet.end(), compareUtilization);
	return createStackFromVector(v_taskSet);
}

stack<Task> TaskSet::sortTaskSetByWCET()
{
	sort(v_taskSet.begin(), v_taskSet.end(), compareWCET);
	return createStackFromVector(v_taskSet);
}

stack<Task> TaskSet::sortTaskSetByPeriod()
{
	sort(v_taskSet.begin(), v_taskSet.end(), compareTaskPeriods);
	return createStackFromVector(v_taskSet);
}

stack<Task> TaskSet::createStackFromVector(vector<Task> taskVector) 
{
	stack<Task> taskStack;

	vector<Task>::iterator iterator;
	for (iterator = taskVector.begin(); iterator != taskVector.end(); ++iterator) {
		taskStack.push(*iterator);
	}
	return taskStack;
}

TaskSet::~TaskSet(void)
{
	//Empty for now.
}
