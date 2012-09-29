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

Simulator::Simulator(TaskSet ts)
{
	v_ts = ts;
}

//Given a taskSet, determines whether the taskSet is schedulable according to rate-monotonic scheduling policy.
bool Simulator::RM()
{
	//Initialization of variables
	stack<Task> readyQueue = v_ts.sortTaskSetByPeriod(); 		//Converts the taskSet into RM policy readyQueue (implemented in a stack)
	queue<Task> waitQueue;
	Task t = NULL;
	int time, k = 0;

	//It is sufficient to say a job set meet all its deadlines
	//As long as jobs in a job set meet deadlines up to the LCM of job periods
	while (time <= LCM)
	{
		k = checkNewArrivals(time, waitQueue); 					//Determines how many jobs in the waitQueue are ready
		for (int i = 0; i < k; i++)
		{
			t = waitQueue.pop();
			readyQueue = v_ts.addTaskByPeriod(readyQueue, t); 	//Adds ready tasks to readyQueue
		}

		//Generates new periodic task -- needs work

		/*if (addNewTaskOnPeriod(time, readyQueue.top()) == true)
		{
			Task newTask = readyQueue.top();
			newTask.updateNextArrivalTime(time + newTask.getPeriod());
			addToWait(waitQueue, newTask);
		}*/

		//Service the next job in the readyQueue
		t = readyQueue.pop();
		t.timeKeep(t.getTimeKeeper() + 1);

		//Determines whether the task missed its deadline - if it does, unschedulable
		if (time > (t.getNextArrivalTime() + t.getRelativeDeadline()))
			return false;

		//Determines whether the task is complete
		if (t.getTimeKeeper() == t.getWorstCaseExecutionTime())
			{
				t.updateNextArrivalTime(t.getNextArrivalTime() + t.getPeriod());
				waitQueue = addToWait(waitQueue, t);
			}
		else
			readyQueue.push(t);									//Done servicing - placing back onto stack (it remains the highest priority, therefore it belongs on the top)
		time++;													//Time tick
	}

	return true;
}

//Given a taskSet, determines whether the taskSet is schedulable according to shortest-job-first scheduling policy.
bool Simulator::SJF()
{
	//Initialization of variables
	stack<Task> readyQueue = v_ts.sortTaskSetByWCET(); 		//Converts the taskSet into SJF policy readyQueue (implemented in a stack)
	queue<Task> waitQueue;
	Task t = NULL;
	int time, k = 0;

	//It is sufficient to say a job set meet all its deadlines
	//As long as jobs in a job set meet deadlines up to the LCM of job periods
	while (time <= LCM)
	{
		k = checkNewArrivals(time, waitQueue); 					//Determines how many jobs in the waitQueue are ready
		for (int i = 0; i < k; i++)
		{
			t = waitQueue.pop();
			readyQueue = v_ts.addTaskByWCET(readyQueue, t); 	//Adds ready tasks to readyQueue
		}

		//Service the next job in the readyQueue
		t = readyQueue.pop();
		t.timeKeep(t.getTimeKeeper() + 1);

		//Determines whether the task missed its deadline - if it does, unschedulable
		if (time > (t.getNextArrivalTime() + t.getRelativeDeadline()))
			return false;

		//Determines whether the task is complete
		if (t.getTimeKeeper() == t.getWorstCaseExecutionTime())
			{
				t.updateNextArrivalTime(t.getNextArrivalTime() + t.getPeriod());
				waitQueue = addToWait(waitQueue, t);
			}
		else
			readyQueue.push(t);									//Done servicing - placing back onto stack (it remains the highest priority, therefore it belongs on the top)
		time++;													//Time tick
	}

	return true;
}

//Given a taskSet, determines whether the taskSet is schedulable according to max-utilization-first scheduling policy.
bool Simulator::MUF()
{
	//Initialization of variables
	stack<Task> readyQueue = v_ts.sortTaskSetByUtilization(); 		//Converts the taskSet into MUF policy readyQueue (implemented in a stack)
	queue<Task> waitQueue;
	Task t = NULL;
	int time, k = 0;

	//It is sufficient to say a job set meet all its deadlines
	//As long as jobs in a job set meet deadlines up to the LCM of job periods
	while (time <= LCM)
	{
		k = checkNewArrivals(time, waitQueue); 					//Determines how many jobs in the waitQueue are ready
		for (int i = 0; i < k; i++)
		{
			t = waitQueue.pop();
			readyQueue = v_ts.addTaskByUtilization(readyQueue, t); 	//Adds ready tasks to readyQueue
		}

		//Service the next job in the readyQueue
		t = readyQueue.pop();
		t.timeKeep(t.getTimeKeeper() + 1);

		//Determines whether the task missed its deadline - if it does, unschedulable
		if (time > (t.getNextArrivalTime() + t.getRelativeDeadline()))
			return false;

		//Determines whether the task is complete
		if (t.getTimeKeeper() == t.getWorstCaseExecutionTime())
			{
				t.updateNextArrivalTime(t.getNextArrivalTime() + t.getPeriod());
				waitQueue = addToWait(waitQueue, t);
			}
		else
			readyQueue.push(t);									//Done servicing - placing back onto stack (it remains the highest priority, therefore it belongs on the top)
		time++;													//Time tick
	}

	return true;
}

queue<Task> Simulator::addToWait(queue<Task> waitQueue, Task t)
{
	queue<Task> tempQueue;
	Task tempTask;
	while (!waitQueue.empty)
	{
		tempTask = waitQueue.pop();
		if (tempTask.getNextArrivalTime() < t.getNextArrivalTime())
			tempQueue.push(tempTask);
		else
		{
			tempQueue.push(t);
			tempQueue.push(tempTask);
			while(!waitQueue.empty())
			{
				tempQueue.push(tempTask);
			}
		}
	}
	return tempQueue;
}
int Simulator::checkNewArrivals(int time, queue<Task> waitQueue)
{
	int k = 0;
	int size = waitQueue.size();
	for (int i = 0; i < size; i++)
	{
		if (waitQueue.front().getNextArrivalTime() >= time)
			k++;
		else
			return k;
	}
}

/*bool Simulator::addNewTaskOnPeriod(int time, Task t)
{
	int period = t.getPeriod();
	if ((time mod period) == 0)
		return true;
	else
		return false;
}*/
