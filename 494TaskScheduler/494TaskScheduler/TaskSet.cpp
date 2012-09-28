#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include "TaskSet.h"

using namespace std;

TaskSet::TaskSet(vector<Task> taskSet) 
{
	v_taskSet = taskSet;
}

/** We want the top of the stack to have the shortest period for RM.**/
bool compareTaskPeriods(Task taskA, Task taskB) { return (taskA.getPeriod() > taskB.getPeriod()); }

/** We want the top of the stack to have the shortest wcet for SJF.**/
bool compareWCET(Task taskA, Task taskB) { return (taskA.getWorstCaseExecutionTime() > taskB.getWorstCaseExecutionTime()); }

/**We want the top of the stack to have the largest utilization for MUF.**/
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


void TaskSet::printTaskSet() 
{
	vector<Task>::iterator iterator;
	int i=1;
	for(iterator = v_taskSet.begin(); iterator != v_taskSet.end(); ++iterator) {
		cout << "Task" << i << ":" << "WCET: " << iterator->getWorstCaseExecutionTime() 
			<< " RDL: " << iterator->getRelativeDeadline() << " Period: " << iterator->getPeriod() << "\n"; 
		i++;
	}
}

stack<Task> TaskSet::addTaskByUtilization(stack<Task> taskStack, Task newTask)
{
	v_taskSet.push_back(newTask);
	return sortTaskSetByUtilization();
}

stack<Task> TaskSet::addTaskByWCET(stack<Task> taskStack, Task newTask)
{
	v_taskSet.push_back(newTask);
	return sortTaskSetByWCET();
}

stack<Task> TaskSet::addTaskByPeriod(stack<Task> taskStack, Task newTask)
{
	v_taskSet.push_back(newTask);
	return sortTaskSetByPeriod();
}

TaskSet::~TaskSet(void)
{
	//Empty for now.
}
