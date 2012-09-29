#pragma once

#include "BaseAnalyzer.h"

/**Simple algorithm helper class.**/
class AlgorithmHelper 
{
public:
	/**Determine whether to use adjusted Liu and Laland/Hyperbolic bound test.**/
	static bool isDeadlineSmallerThanPeriod(TaskSet taskSet) {
		stack<Task> tasks = taskSet.sortTaskSetByPeriod();

		while (!tasks.empty()) {
			Task task = tasks.top();
			if (task.getRelativeDeadline() < task.getPeriod()) {
				return false;
			}
			tasks.pop();
		}
		return true;
	}
};

/**Liu and Layland Analyzer implements BaseAnalyzer.**/
class LALAnalyzer : public BaseAnalyzer {
public:
	/**Constructor.**/
	LALAnalyzer(TaskSet taskSet) : BaseAnalyzer(taskSet) {};
	
	virtual bool isTaskSetScheduable();

};

class HBAnalyzer : public BaseAnalyzer {
public:
	/**Contructor.**/
	HBAnalyzer(TaskSet taskSet) : BaseAnalyzer(taskSet) { };

	virtual bool isTaskSetScheduable();

};


