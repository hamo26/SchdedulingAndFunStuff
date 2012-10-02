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

	double RM(TaskSet taskSet);
	double SJF(TaskSet taskSet);
	double MUF(TaskSet taskSet);
	queue<Task> addToWait(queue<Task> waitQueue, Task * t);
	bool isJobComplete(Task * t);
	bool isDeadlineMet(double time, Task * t);
	double successJobCompletion(double deadlinesMissed, double totalJobs);

//	~Simulator(void);
};

#endif
