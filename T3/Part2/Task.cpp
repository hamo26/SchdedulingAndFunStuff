#include "Task.h"
#include <iostream>
Task::Task ()
{

}

Task::Task(double worstCaseExecutionTime, double relativeDeadline, int period) 
{
	i_period = period;
	f_relativeDeadline = relativeDeadline;
	f_wcet = worstCaseExecutionTime;
	i_nextArrival = 0;
	f_processorTimeConsumed = 0;
}

int Task::getPeriod() { return i_period; }

double Task::getProcessorTimeConsumed() { return f_processorTimeConsumed; }

double Task::getRelativeDeadline() { return f_relativeDeadline; }

double Task::getWorstCaseExecutionTime() { return f_wcet; }

int Task::getCeiledWorstCaseExecutionTime() { return (int) ceil(f_wcet); }

double Task::getUtlization() { return (f_wcet / (double) i_period); }

int Task::getNextArrivalTime() {return i_nextArrival;}

void Task::incrementProcessorTimeConsumed(double incrementValue) {
//	cout << "\n current:" << f_processorTimeConsumed << "\n";
//	cout << "increment:" << incrementValue << "\n";

	f_processorTimeConsumed+=incrementValue;
//	cout << "result:" << f_processorTimeConsumed << "\n";
}

void Task::updateNextArrivalTime(int time) {i_nextArrival = time;}

void Task::resetProcessorTime()
{f_processorTimeConsumed = 0;}

//void Task::complete(bool complete) {b_isComplete = complete;}

Task::~Task() { return; }



