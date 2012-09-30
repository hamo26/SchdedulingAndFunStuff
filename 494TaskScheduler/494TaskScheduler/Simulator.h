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

	bool RM(vector<Task> taskSet);
	bool SJF(vector<Task> taskSet);
	bool MUF(vector<Task> taskSet);
	queue<Task> addToWait(queue<Task> waitQueue, Task t);
	int checkNewArrivals(int time, queue<Task> waitQueue);

};

#endif
