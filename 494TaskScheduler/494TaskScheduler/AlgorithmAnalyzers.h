#pragma once

#include "BaseAnalyzer.h"

/**Liu and Layland Analyzer implements BaseAnalyzer.**/
class LALAnalyzer : public BaseAnalyzer {
public:
	/**Constructor.**/
	LALAnalyzer(TaskSet taskSet) : BaseAnalyzer(taskSet) {};
	
	virtual bool isTestValid();

	virtual bool isTaskSetScheduable();

};

class HBAnalyzer : public BaseAnalyzer {
public:
	/**Contructor.**/
	HBAnalyzer(TaskSet taskSet) : BaseAnalyzer(taskSet) { };

	virtual bool isTestValid();

	virtual bool isTaskSetScheduable();

};


