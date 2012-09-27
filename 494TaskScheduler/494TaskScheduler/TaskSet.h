#pragma once
#ifndef TASK_SET_H
#define TASK_SET_H

#include <vector>
#include <stack>
#include "Task.h"

/**
The task set manages a set of tasks and includes convinience methods for sorting the tasks
based on the algorithm required. The task set will ultimately be represented as an ordered stack (vector).
TaskSets are immutable for now.
**/
class TaskSet
{

private:

	/**Internal representation of a task set. **/
	vector<Task> v_taskSet;
	
	//HELPER METHODS

	/** Creates a stack from a vector. **/
	stack<Task> createStackFromVector(vector<Task> taskVector);

public:
	/** Constructor taking vector of tasks representing a task set as a parameter. **/
	TaskSet(vector<Task> taskSet);

	//SORTING METHODS

	/** Sorts the internal task set representation by period. **/
	stack<Task> sortTaskSetByPeriod();

	/** Sorts teh internal task set represenation by utilization. **/
	stack<Task> sortTaskSetByUtilization();

	/** Sorts the internal task set representation by WCET. **/
	stack<Task> sortTaskSetByWCET();

	stack<Task> addTaskByUtilization(stack<Task> taskStack, Task newTask);

	stack<Task> addTaskByWCET(stack<Task> taskStack, Task newTask);

	stack<Task> addTaskByPeriod(stack<Task> taskStack, Task newTask);

	~TaskSet(void);
};	

#endif
