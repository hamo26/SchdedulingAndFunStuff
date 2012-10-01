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

int Simulator::RM(TaskSet ts)
{
	TaskSet v_ts = ts;
	//Initialization of variables
	stack<Task> readyQueue = v_ts.sortTaskSetByPeriod(); 		//Converts the taskSet into RM policy readyQueue (implemented in a stack)
	queue<Task> waitQueue;
	double time = 0, k = 0, deadlinesMissed = 0;
	Task * t;

	//It is sufficient to say a job set meet all its deadlines
	//As long as jobs in a job set meet deadlines up to the LCM of job periods
	while (time <= 10000)
	{
		k = checkNewArrivals(time, waitQueue); 					//Determines how many jobs in the waitQueue are ready
		for (int i = 0; i < k; i++)
		{
			cout << "Checking arrivals\n";

			readyQueue = v_ts.addTaskByPeriod(readyQueue, waitQueue.front()); //Adds ready tasks to readyQueue
			waitQueue.pop();
		}

		//Service the next job in the readyQueue
		t = &readyQueue.top();
		t->incrementProcessorTimeConsumed(1);
//		cout << t.getProcessorTimeConsumed();

		//Determines whether the task missed its deadline - if it does, unschedulable
//		if (time > (t.getNextArrivalTime() + t.getRelativeDeadline()))
//			return false;

		//Determines whether the task is complete
		if (t->getProcessorTimeConsumed() >= t->getWorstCaseExecutionTime())
		{
			cout << "Task Complete\n";
			double overshoot = t->getProcessorTimeConsumed() - t->getWorstCaseExecutionTime();
			time = time - overshoot;
			if (time > (t->getNextArrivalTime() + t->getRelativeDeadline()))
			{
				deadlinesMissed++;
				cout << "Missed deadline\n";
			}

			t->updateNextArrivalTime(t->getNextArrivalTime() + t->getPeriod());
			waitQueue = addToWait(waitQueue, t);
			readyQueue.pop();

			Task t = readyQueue.top();
			t.incrementProcessorTimeConsumed(overshoot);
			readyQueue.push(t);
		}
//		else
//			readyQueue.push(t);									//Done servicing - placing back onto stack (it remains the highest priority, therefore it belongs on the top)
		time++;												//Time tick
//		cout << "\n" << time << "\n";
	}

	return deadlinesMissed;
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
		k = checkNewArrivals(time, waitQueue); 					//Determines how many jobs in the waitQueue are ready
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
		k = checkNewArrivals(time, waitQueue); 					//Determines how many jobs in the waitQueue are ready
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
	queue<Task> tempQueue;
	Task * tempTask;
	while (!waitQueue.empty())
	{
		tempTask = &waitQueue.front();
		waitQueue.pop();
		if (tempTask->getNextArrivalTime() < t->getNextArrivalTime())
			tempQueue.push(*tempTask);
		else
		{
			tempQueue.push(*t);
			tempQueue.push(*tempTask);
			while(!waitQueue.empty())
			{
				tempQueue.push(*tempTask);
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

double Simulator::sucessJobCompletion(double deadlinesMissed, double totalJobs)
{
	return (deadlinesMissed/totalJobs);
}

/*bool Simulator::addNewTaskOnPeriod(int time, Task t)
{
	int period = t.getPeriod();
	if ((time mod period) == 0)
		return true;
	else
		return false;
}*/

//Generates new periodic task -- needs work

/*if (addNewTaskOnPeriod(time, readyQueue.top()) == true)
{
	Task newTask = readyQueue.top();
	newTask.updateNextArrivalTime(time + newTask.getPeriod());
	addToWait(waitQueue, newTask);
}*/
