// 494TaskScheduler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TaskSetInputParser.h"
#include "AlgorithmAnalyzers.h"

#include <iostream>
#include <time.h>

using namespace std;

class AlgorithmicProfiler 
{
private:
	/**The time taken for a profiler to execute.**/
	double d_deltaTime;
	
	/**Ultimately determine if a task set is scheduable.**/
	bool b_isTaskSetScheduable;

	TaskSet t_taskSet;

	void AlgorithmicProfiler::isTaskSetScheduable(BaseAnalyzer* analyzer) {
		b_isTaskSetScheduable = analyzer->isTestValid() && analyzer->isTaskSetScheduable();
	}

public:
	AlgorithmicProfiler::AlgorithmicProfiler(TaskSet taskSet) : t_taskSet(taskSet) {} 

	/**Get the time elapsed after running the profile for an algorithm.**/
	double AlgorithmicProfiler::getTimeElapsed() { return d_deltaTime; }

	/**Get whethr the algorithm is scheduable or not.**/
	bool AlgorithmicProfiler::getIsScheduable() { return b_isTaskSetScheduable; }

	/**Run profile for Liu and Layland.**/
	void AlgorithmicProfiler::ProfileLAL() {
		double d_time_init, d_time_end;
		LALAnalyzer lalAnalyzer(t_taskSet);
		
		d_time_init = (double) clock() / CLOCKS_PER_SEC;
		b_isTaskSetScheduable = lalAnalyzer.isTestValid() && lalAnalyzer.isTaskSetScheduable();
		d_time_end = (double) clock();
		
		d_deltaTime = (((double) clock() / CLOCKS_PER_SEC) - d_time_init)*pow(10,3); //Get the time elapsed in milliseconds.
		return;
	}
};

int main(int argc, char** argv)
{
	TaskSetInputParser parser;

	bool parseResult = parser.parseInputFile(argv[1]);
	if (!parseResult) {
		cout << "parse failed\n";
	} else {
		while (!parser.isEmpty()) {
			TaskSet taskSet = parser.getNext();

			cout << "Analyzing task set: \n";
			taskSet.printTaskSet();

			AlgorithmicProfiler algorithmicProfiler(taskSet);

			algorithmicProfiler.ProfileLAL();

			cout << "Scheduable using RM Scheduling / Liu and Layland bound test: " 
				<< (algorithmicProfiler.getIsScheduable() ? "Yes " : "No ")				
				<< "<" << algorithmicProfiler.getTimeElapsed() << ">\n";
			
		}
	}
	return 0;
}

