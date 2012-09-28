#pragma once

#include "TaskSet.h"

/**Abstract base analyzer for all algorithmic analyzers to implement.**/
class BaseAnalyzer
{
protected:
	/**The taskset in question.**/
	TaskSet t_taskSet;

	/**Constructor used by all implementing classes.**/
	BaseAnalyzer(TaskSet taskSet) : t_taskSet(taskSet) {}
	
	/**Destructor.**/
	~BaseAnalyzer() {}
public:
	/**Returns whether the algorithmic test can be applied to the task set.**/
	virtual bool isTestValid() = 0;

	/**Returns whether a task set is scheduable using an algorithmic test.**/
	virtual bool isTaskSetScheduable() = 0;	
};

