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
	TaskSet v_ts;
public:
	Simulator(TaskSet ts);

	bool RM();
	bool SJF();
	bool MUF();
	queue<Task> addToWait(queue<Task> waitQueue, Task t);
	int checkNewArrivals(int time, queue<Task> waitQueue);
	//bool Simulator::addNewTaskOnPeriod(int time, Task t);


};

#endif
