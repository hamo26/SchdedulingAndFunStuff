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
//	TaskSet v_ts;

public:
	Simulator(void);
//	Simulator(vector<Task> taskSet);

	int RM(TaskSet taskSet);
	bool SJF(TaskSet taskSet);
	bool MUF(TaskSet taskSet);
	queue<Task> addToWait(queue<Task> waitQueue, Task t);
	int checkNewArrivals(int time, queue<Task> waitQueue);
	double sucessJobCompletion(double deadlinesMissed, double totalJobs);

};

#endif
