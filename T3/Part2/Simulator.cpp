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
#include <iostream>


//Given a taskSet, determines whether the taskSet is schedulable according to rate-monotonic scheduling policy.
Simulator::Simulator()
{

}

double Simulator::RM(TaskSet ts)
{
	TaskSet v_ts = ts;
	//Initialization of variables
	stack<Task> readyQueue = v_ts.sortTaskSetByPeriod(); 		//Converts the taskSet into RM policy readyQueue (implemented in a stack)
	queue<Task> waitQueue;
	double time = 0;
	int k = 0, deadlinesMissed = 0, deadlinesMet = 0;
	Task * t;

	//Investigation of 100,000 time units
	while (time <= 100000)
	{
		if (time == 99999) {
			cout << "";
		}
		//Check for any waiting jobs
		while (!waitQueue.empty())
		{
			//Moves job from waiting to ready
			if (waitQueue.front().getNextArrivalTime() <= time)
			{
				readyQueue = v_ts.addTaskByPeriod(readyQueue, waitQueue.front());
				waitQueue.pop();
			}
			else { break; }
		}

		//Service the next job in the readyQueue
		if (!readyQueue.empty())
		{
			t = &readyQueue.top();
			t->incrementProcessorTimeConsumed(1);

			//Determines whether the task is complete
			if (isJobComplete(t))
			{
				//Checking for missed deadline
				if (isDeadlineMet(time, t))
				{
					deadlinesMet++;
				}
				else { deadlinesMissed++; }

				//Removing completed task from readyQueue
				readyQueue.pop();

				//Services task and adds to waitQueue
				waitQueue = addToWait(waitQueue, t);
			}
		}
		time++;											//Time tick
	}

	return successJobCompletion(deadlinesMissed, deadlinesMet);
}

//Given a taskSet, determines whether the taskSet is schedulable according to shortest-job-first scheduling policy.
bool Simulator::SJF(TaskSet ts)
{
	TaskSet v_ts = ts;
	//Initialization of variables
	stack<Task> readyQueue = v_ts.sortTaskSetByWCET(); 		//Converts the taskSet into SJF policy readyQueue (implemented in a stack)
	queue<Task> waitQueue;
	int time, k = 0;

	//It is sufficient to say a job set meet all its deadlines
	//As long as jobs in a job set meet deadlines up to the LCM of job periods
	while (time <= v_ts.calculateLCM())
	{
		//		k = checkNewArrivals(time, waitQueue); 					//Determines how many jobs in the waitQueue are ready
		for (int i = 0; i < k; i++)
		{
			readyQueue = v_ts.addTaskByWCET(readyQueue, waitQueue.front()); 	//Adds ready tasks to readyQueue
			waitQueue.pop();
		}

		//Service the next job in the readyQueue
		Task t = readyQueue.top();
		t.incrementProcessorTimeConsumed(1);

		//Determines whether the task missed its deadline - if it does, unschedulable
		if (time > (t.getNextArrivalTime() + t.getRelativeDeadline()))
			return false;

		//Determines whether the task is complete
		if (t.getProcessorTimeConsumed() == t.getWorstCaseExecutionTime())
		{
			t.updateNextArrivalTime(t.getNextArrivalTime() + t.getPeriod());
			//				waitQueue = addToWait(waitQueue, t);
		}
		else
			readyQueue.push(t);									//Done servicing - placing back onto stack (it remains the highest priority, therefore it belongs on the top)
		time++;													//Time tick
	}

	return true;
}

//Given a taskSet, determines whether the taskSet is schedulable according to max-utilization-first scheduling policy.
bool Simulator::MUF(TaskSet ts)
{
	TaskSet v_ts = ts;
	//Initialization of variables
	stack<Task> readyQueue = v_ts.sortTaskSetByUtilization(); 		//Converts the taskSet into MUF policy readyQueue (implemented in a stack)
	queue<Task> waitQueue;
	int time, k = 0;

	//It is sufficient to say a job set meet all its deadlines
	//As long as jobs in a job set meet deadlines up to the LCM of job periods
	while (time <= v_ts.calculateLCM())
	{
		//		k = checkNewArrivals(time, waitQueue); 					//Determines how many jobs in the waitQueue are ready
		for (int i = 0; i < k; i++)
		{
			readyQueue = v_ts.addTaskByUtilization(readyQueue, waitQueue.front()); 	//Adds ready tasks to readyQueue
			waitQueue.pop();
		}

		//Service the next job in the readyQueue
		Task t = readyQueue.top();
		t.incrementProcessorTimeConsumed(1);

		//Determines whether the task missed its deadline - if it does, unschedulable
		if (time > (t.getNextArrivalTime() + t.getRelativeDeadline()))
			return false;

		//Determines whether the task is complete
		if (t.getProcessorTimeConsumed() == t.getWorstCaseExecutionTime())
		{
			t.updateNextArrivalTime(t.getNextArrivalTime() + t.getPeriod());
			//				waitQueue = addToWait(waitQueue, t);
		}
		else
			readyQueue.push(t);									//Done servicing - placing back onto stack (it remains the highest priority, therefore it belongs on the top)
		time++;													//Time tick
	}

	return true;
}

queue<Task> Simulator::addToWait(queue<Task> waitQueue, Task * t)
{
	//Making adjustments for next service
	t->updateNextArrivalTime(t->getNextArrivalTime() + t->getPeriod());
	t->resetProcessorTime();

	queue<Task> tempQueue;

	while (!waitQueue.empty())
	{
	Task tempTask = waitQueue.front();

    if (t->getNextArrivalTime() < tempTask.getNextArrivalTime()) {
            tempQueue.push(*t);
            while(!waitQueue.empty())
            {
            	tempQueue.push(waitQueue.front());
            	waitQueue.pop();
            }
            return tempQueue;
    }
    tempQueue.push(tempTask);
    waitQueue.pop();
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
bool Simulator::isJobComplete(Task * t)
{
	if (t->getProcessorTimeConsumed() >= t->getCeiledWorstCaseExecutionTime())
		return true;
	else
		return false;
}

//Checks if a job meets its deadline
bool Simulator::isDeadlineMet(double time, Task * t)
{
	if (time > (t->getNextArrivalTime() + t->getRelativeDeadline()))
		return false;
	else
		return true;
}
