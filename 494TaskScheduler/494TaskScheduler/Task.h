#pragma once
#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>

using namespace std;

class Task
{

private:
	/** The worst case execution time of the task.**/
	double f_wcet;
	/** The period of the task.**/
	int i_period;
	/** The relative deadline of the task. **/
	double f_relativeDeadline;
	
	/** This parameter may be used to keep track of how many "clock ticks" the task has executed **/
	double f_processorTimeConsumed; 
	
	/**
	Flag to signal that the task has finished execution.
	**/
	bool b_isComplete;

public:
	
	/**
	Default Constructor
	**/
	Task(void);

	/**Paramterized Constructor**/
	Task(double worstCaseExecutionTime, double relativeDeadline, int period);

	//GETTERS
	double getRelativeDeadline();

	int getPeriod();

	double getWorstCaseExecutionTime();
	
	double getProcessorTimeConsumed();

	bool isExecutionComplete();
	
	double getUtlization();

	//SETTERS
	void incrementProcessorTimeConsumed(float incrementValue);

	/**
	Default Destructor
	**/
	~Task(void);
};

#endif

