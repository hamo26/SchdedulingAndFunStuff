/*
 *  Simulator.cpp
 *  eece494
 *
 *  Created by Cary Wong on 12-09-26.
 *  Copyright 2012 UBC. All rights reserved.
 *
 */

#include <queue>
#include "Simulator.h"
#include "TaskSet.h"
#include "Task.h"

//Given a taskSet, determines whether the taskSet is schedulable according to rate-monotonic scheduling policy.
Simulator::Simulator()
{

}

double Simulator::RM(TaskSet ts)
{
	//Initialization of variables
	stack<Task> readyQueue = ts.sortTaskSetByPeriod(); 		//Converts the taskSet into RM policy readyQueue (implemented in a stack)
	queue<Task> waitQueue;
	double time = 0;
	int k = 0;
	double deadlinesMissed = 0;
	double deadlinesMet = 0;

	//Investigation of 100,000 time units
	while (time <= 100000)
	{
		//Check for any waiting jobs
		while (!waitQueue.empty())
		{
			//Moves job from waiting to ready
			if (waitQueue.front().getNextArrivalTime() <= time)
			{
				readyQueue = addTaskByPeriod(readyQueue, waitQueue.front());
				waitQueue.pop();
			}
			else { break; }
		}

		//Service the next job in the readyQueue
		if (!readyQueue.empty())
		{
			readyQueue.top().incrementProcessorTimeConsumed(1);

			//Determines whether the task is complete
			if (isJobComplete(readyQueue.top()))
			{
				//Checking for missed deadline
				if (isDeadlineMet(time, readyQueue.top()))
				{
					deadlinesMet++;
				}
				else { deadlinesMissed++; }

				//Services task and adds to waitQueue
				waitQueue = addToWait(waitQueue, readyQueue.top());

				//Removing completed task from readyQueue
				readyQueue.pop();
			}
		}
		time++;											//Time tick
	}

	return successJobCompletion(deadlinesMissed, deadlinesMet);
}

//Given a taskSet, determines whether the taskSet is schedulable according to shortest-job-first scheduling policy.
double Simulator::SJF(TaskSet ts)
{
	//Initialization of variables
	stack<Task> readyQueue = ts.sortTaskSetByWCET(); 		//Converts the taskSet into SJF policy readyQueue (implemented in a stack)
	queue<Task> waitQueue;
	double time = 0;
	double deadlinesMissed = 0;
	double deadlinesMet = 0;

	//Investigation of 100,000 time units
	while (time <= 100000)
	{
		//Check for any waiting jobs
		while (!waitQueue.empty())
		{
			//Moves job from waiting to ready
			if (waitQueue.front().getNextArrivalTime() <= time)
			{
				readyQueue = addTaskByWCET(readyQueue, waitQueue.front());
				waitQueue.pop();
			}
			else { break; }
		}

		//Service the next job in the readyQueue
		if (!readyQueue.empty())
		{
			readyQueue.top().incrementProcessorTimeConsumed(1);

			//Determines whether the task is complete
			if (isJobComplete(readyQueue.top()))
			{
				//Checking for missed deadline
				if (isDeadlineMet(time, readyQueue.top()))
				{
					deadlinesMet++;
				}
				else { deadlinesMissed++; }

				//Services task and adds to waitQueue
				waitQueue = addToWait(waitQueue, readyQueue.top());

				//Removing completed task from readyQueue
				readyQueue.pop();
			}
		}
		time++;											//Time tick
	}

	return successJobCompletion(deadlinesMissed, deadlinesMet);
}

//Given a taskSet, determines whether the taskSet is schedulable according to max-utilization-first scheduling policy.
double Simulator::MUF(TaskSet ts)
{
	//Initialization of variables
	stack<Task> readyQueue = ts.sortTaskSetByUtilization(); 		//Converts the taskSet into MUF policy readyQueue (implemented in a stack)
	queue<Task> waitQueue;
	double time = 0;
	int k = 0;
	double deadlinesMissed = 0;
	double deadlinesMet = 0;

	//Investigation of 100,000 time units
	while (time <= 100000)
	{
		//Check for any waiting jobs
		while (!waitQueue.empty())
		{
			//Moves job from waiting to ready
			if (waitQueue.front().getNextArrivalTime() <= time)
			{
				readyQueue = addTaskByUtilization(readyQueue, waitQueue.front());
				waitQueue.pop();
			}
			else { break; }
		}

		//Service the next job in the readyQueue
		if (!readyQueue.empty())
		{
			readyQueue.top().incrementProcessorTimeConsumed(1);

			//Determines whether the task is complete
			if (isJobComplete(readyQueue.top()))
			{
				//Checking for missed deadline
				if (isDeadlineMet(time, readyQueue.top()))
				{
					deadlinesMet++;
				}
				else { deadlinesMissed++; }

				//Services task and adds to waitQueue
				waitQueue = addToWait(waitQueue, readyQueue.top());

				//Removing completed task from readyQueue
				readyQueue.pop();
			}
		}
		time++;											//Time tick
	}

	return successJobCompletion(deadlinesMissed, deadlinesMet);
}

queue<Task> Simulator::addToWait(queue<Task> waitQueue, Task t)
{
	//Making adjustments for next service
	Task newTask = t;
	newTask.updateNextArrivalTime(t.getNextArrivalTime() + t.getPeriod());
	newTask.resetProcessorTime();

	queue<Task> tempQueue;

	if (waitQueue.empty())
	{
		tempQueue.push(newTask);
		return tempQueue;
	}

	while (!waitQueue.empty())
	{

		if (newTask.getNextArrivalTime() < waitQueue.front().getNextArrivalTime()) {
			tempQueue.push(newTask);
			while(!waitQueue.empty())
			{
				tempQueue.push(waitQueue.front());
				waitQueue.pop();
			}
			return tempQueue;
		}
		tempQueue.push(waitQueue.front());
		waitQueue.pop();
		if (waitQueue.empty())
			tempQueue.push(newTask);
	}
	return tempQueue;
}

//Calculates the job success percentage
double Simulator::successJobCompletion(double deadlinesMissed, double deadlinesMet)
{
	double totalJobs = deadlinesMissed + deadlinesMet;
	if (totalJobs == 0) {return 0;}
	return (deadlinesMet/totalJobs)*100;
}

//Checks if a job is complete
bool Simulator::isJobComplete(Task t)
{
	if (t.getProcessorTimeConsumed() >= t.getCeiledWorstCaseExecutionTime())
		return true;
	else
		return false;
}

//Checks if a job meets its deadline
bool Simulator::isDeadlineMet(double time, Task t)
{
	double overshoot = (t.getProcessorTimeConsumed() - t.getWorstCaseExecutionTime());
	if ((time - overshoot + 1) <= (t.getNextArrivalTime() + t.getRelativeDeadline()))
		return true;
	else
		return false;
}

stack<Task> Simulator::addTaskByPeriod (stack<Task> readyQueue, Task t)
{
	stack<Task> tempReady;

	if (readyQueue.empty())
	{
		tempReady.push(t);
		return tempReady;
	}

	while (!readyQueue.empty())
	{

		if (t.getPeriod() < readyQueue.top().getPeriod()) {
			tempReady.push(t);
			while(!readyQueue.empty())
			{
				tempReady.push(readyQueue.top());
				readyQueue.pop();
			}
			return reverseStackOrdering(tempReady);
		}
		tempReady.push(readyQueue.top());
		readyQueue.pop();
		if (readyQueue.empty())
			tempReady.push(t);
	}
	return reverseStackOrdering(tempReady);
}

stack<Task> Simulator::addTaskByWCET (stack<Task> readyQueue, Task t)
{
	stack<Task> tempReady;

	if (readyQueue.empty())
	{
		tempReady.push(t);
		return tempReady;
	}

	while (!readyQueue.empty())
	{

		if (t.getWorstCaseExecutionTime() < readyQueue.top().getWorstCaseExecutionTime()) {
			tempReady.push(t);
			while(!readyQueue.empty())
			{
				tempReady.push(readyQueue.top());
				readyQueue.pop();
			}
			return reverseStackOrdering(tempReady);
		}
		tempReady.push(readyQueue.top());
		readyQueue.pop();
		if (readyQueue.empty())
			tempReady.push(t);
	}
	return reverseStackOrdering(tempReady);
}

stack<Task> Simulator::addTaskByUtilization (stack<Task> readyQueue, Task t)
{
	stack<Task> tempReady;

	if (readyQueue.empty())
	{
		tempReady.push(t);
		return tempReady;
	}

	while (!readyQueue.empty())
	{

		if (t.getUtlization() < readyQueue.top().getUtlization()) {
			tempReady.push(t);
			while(!readyQueue.empty())
			{
				tempReady.push(readyQueue.top());
				readyQueue.pop();
			}
			return reverseStackOrdering(tempReady);
		}
		tempReady.push(readyQueue.top());
		readyQueue.pop();
		if (readyQueue.empty())
			tempReady.push(t);
	}
	return reverseStackOrdering(tempReady);
}

stack<Task> Simulator::reverseStackOrdering(stack<Task> currentStack)
{
	stack<Task> tempStack;

	while(!currentStack.empty()) {
		tempStack.push(currentStack.top());
		currentStack.pop();
	}
	return tempStack;
}