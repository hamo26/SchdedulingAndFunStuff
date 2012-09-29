#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include "TaskSet.h"
#include <numeric>

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

stack<Task> TaskSet::reverseStackOrdering()
{
	stack<Task> currentStack = createStackFromVector(v_taskSet);
	stack<Task> tempStack;

	while(!currentStack.empty()) {
		tempStack.push(currentStack.top());
		currentStack.pop();
	}
	return tempStack;
}

TaskSet::~TaskSet(void)
{
	//Empty for now.
}

//Code ref: http://stackoverflow.com/questions/4229870/c-algorithm-to-calculate-least-common-multiple-for-multiple-numbers
int TaskSet::gcd(int a, int b)
{
    for (;;)
    {
        if (a == 0) return b;
        b %= a;
        if (b == 0) return a;
        a %= b;
    }
    return -1;
}

int TaskSet::lcm(int a, int b)
{
    int temp = gcd(a, b);

    return temp ? (a / temp * b) : 0;
}

int TaskSet::calculateLCM()
{
	int result = 0;//accumulate(v_taskSet, v_taskSet + v_taskSet.size() - 1, 1, lcm);
	return result;
}
