#include "stdafx.h"
#include "Task.h"

Task::Task(double worstCaseExecutionTime, double relativeDeadline, int period) 
{
	i_period = period;
	f_relativeDeadline = relativeDeadline;
	f_wcet = worstCaseExecutionTime;
	i_nextArrival = 0;
}

int Task::getPeriod() { return i_period; }

double Task::getProcessorTimeConsumed() { return f_processorTimeConsumed; }

double Task::getRelativeDeadline() { return f_relativeDeadline; }

double Task::getWorstCaseExecutionTime() { return f_wcet; }

double Task::getUtlization() { return (f_wcet / (double) i_period); }

void Task::incrementProcessorTimeConsumed(float incrementValue) { f_processorTimeConsumed+=incrementValue; }

void Task::updateNextArrivalTime(int time) {i_nextArrival = time;}
Task::~Task() { return; }



