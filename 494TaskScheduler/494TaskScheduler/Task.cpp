#include "stdafx.h"
#include "Task.h"

Task::Task(double worstCaseExecutionTime, double relativeDeadline, int period) 
{
	i_period = period;
	f_relativeDeadline = relativeDeadline;
	f_wcet = worstCaseExecutionTime;
	i_nextArrival = 0;
	i_timeKeeper = 0;
}

int Task::getPeriod() { return i_period; }

double Task::getProcessorTimeConsumed() { return f_processorTimeConsumed; }

double Task::getRelativeDeadline() { return f_relativeDeadline; }

double Task::getWorstCaseExecutionTime() { return f_wcet; }

double Task::getUtlization() { return (f_wcet / (double) i_period); }

int Task::getNextArrivalTime() {return i_nextArrival;}

int Task::getTimeKeeper() {return i_timeKeeper;}

void Task::incrementProcessorTimeConsumed(float incrementValue) { f_processorTimeConsumed+=incrementValue; }

void Task::updateNextArrivalTime(int time) {i_nextArrival = time;}

void Task::timeKeep(int time) {i_timeKeeper = time;}

void Task::complete(bool complete) {b_isComplete = complete;}

Task::~Task() { return; }



