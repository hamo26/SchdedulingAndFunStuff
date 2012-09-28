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

Simulator::Simulator(void)
{
	
}

//
bool Simulator::RM(stack<Task> set)
{
	queue<Task> waitQueue;
	Task t = NULL;
	int time = 0;

	//It is sufficient to say a job set meet all its deadlines
	//As long as jobs in a job set meet deadlines up to the LCM of job periods
	while (time <= LCM)
	{
		set = checkNewArrivals(time, waitQueue, set);
		t = set.pop();
		t.timeKeep(t.getTimeKeeper() + 1);
		if (t.getTimeKeeper() == t.getWorstCaseExecutionTime())
			{
				t.complete(true);
				waitQueue = addToWait(waitQueue, t);
			}
		else
			set.push(t);
		time++;
	}

	return true;
}

bool Simulator::SJF(stack<Task> set)
{
	return true;
}

bool Simulator::MUF(stack<Task> set)
{
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
stack<Task> Simulator::checkNewArrivals(int time, queue<Task> waitQueue, stack<Task> readyStack)
{
	int size = waitQueue.size();
	for (int i = 0; i < size; i++)
	{
		if (waitQueue.front().getNextArrivalTime() >= time)
			readyStack = TaskSet.addTaskByPeriod(readySet, waitQueue.pop());
		else
			return readyStack;
	}
}
