// 494TaskScheduler.cpp : Defines the entry point for the console application.
//

#include "TaskSetInputParser.h"
#include "AlgorithmAnalyzers.h"

#include <iostream>
#include <time.h>
#include <math.h>

using namespace std;

class AlgorithmicProfiler 
{
private:
	/**The time taken for a profiler to execute.**/
	double d_deltaTime;
	
	/**Ultimately determine if a task set is scheduable.**/
	bool b_isTaskSetScheduable;

	BaseAnalyzer* p_analyzer;

public:
	AlgorithmicProfiler(BaseAnalyzer* analyzer) : p_analyzer(analyzer) {} 

	/**Get the time elapsed after running the profile for an algorithm.**/
	double getTimeElapsed() { return d_deltaTime; }

	/**Get whethr the algorithm is scheduable or not.**/
	bool getIsScheduable() { return b_isTaskSetScheduable; }

	/**Run profile for Liu and Layland.**/
	void profile() {
		timespec ts, te;
		
		clock_gettime(CLOCK_REALTIME, &ts);
 		b_isTaskSetScheduable = p_analyzer->isTaskSetScheduable();
		clock_gettime(CLOCK_REALTIME, &te);
		d_deltaTime = (double) (te.tv_nsec-ts.tv_nsec)*pow(10,-9);
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
			
			//LIU AND LAYLAND
			LALAnalyzer lalAnalyzer(taskSet);
			AlgorithmicProfiler lalProfiler(&lalAnalyzer);
			lalProfiler.profile();
			cout << "Scheduable using RM Scheduling / Liu and Layland bound test: " 
				<< (lalProfiler.getIsScheduable() ? "Yes " : "No ")				
				<< "<" << lalProfiler.getTimeElapsed() << " seconds>\n";

			//HYPERBOLIC BOUND TEST
			HBAnalyzer hbAnalayzer(taskSet);
			AlgorithmicProfiler hbProfiler(&hbAnalayzer);
                        hbProfiler.profile();
			cout << "Scheduable using RM Scheduling / Hyperbolic bound test: " 
				<< (hbProfiler.getIsScheduable() ? "Yes " : "No ")				
				<< "<" << hbProfiler.getTimeElapsed() << " seconds>\n";


			//RM SCHEDULING, WCRT TEST
			taskSet.sortTaskSetByPeriod();
			WCRTAnalyzer rmwcrtAnalyzer(taskSet);
			AlgorithmicProfiler rmwcrtProfiler(&rmwcrtAnalyzer);
			rmwcrtProfiler.profile();
			cout << "Scheduable using RM scheduling/ WCRT test: " 
				<< (rmwcrtProfiler.getIsScheduable() ? "Yes " : "No ")				
				<< "<" << rmwcrtProfiler.getTimeElapsed() << " seconds>\n";

			//SJF SCHEDULING, WCRT TEST
			taskSet.sortTaskSetByWCET();
			WCRTAnalyzer sjfwcrtAnalyzer(taskSet);
			AlgorithmicProfiler sjfwcrtProfiler(&sjfwcrtAnalyzer);
			sjfwcrtProfiler.profile();
			cout << "Scheduable using SJF scheduling/ WCRT test: " 
				<< (sjfwcrtProfiler.getIsScheduable() ? "Yes " : "No ")				
				<< "<" << sjfwcrtProfiler.getTimeElapsed() << " seconds>\n";

			//MUF SCHEDULING, WCRT TEST
			taskSet.sortTaskSetByUtilization();
			WCRTAnalyzer mufwcrtAnalyzer(taskSet);
			AlgorithmicProfiler mufwcrtProfiler(&mufwcrtAnalyzer);
			mufwcrtProfiler.profile();
			cout << "Scheduable using MUF scheduling/ WCRT test: " 
				<< (mufwcrtProfiler.getIsScheduable() ? "Yes " : "No ")				
				<< "<" << mufwcrtProfiler.getTimeElapsed() << " seconds>\n";
		}
	}
	return 0;
}

