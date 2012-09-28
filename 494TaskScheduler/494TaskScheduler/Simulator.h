/*
 *  Simulator.h
 *  eece494
 *
 *  Created by Cary Wong on 12-09-26.
 *  Copyright 2012 UBC. All rights reserved.
 *
 */
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "TaskSet.h"


class Simulator
{
private:

public:
	Simulator();

	bool RM(stack<Task> set);
	bool SJF(stack<Task> set);
	bool MUF(stack<Task> set);
	queue<Task> addToWait(queue<Task> waitQueue, Task t);
	stack<Task> checkNewArrivals(int time, queue<Task> waitQueue, stack<Task> readyStack);

};

#endif
