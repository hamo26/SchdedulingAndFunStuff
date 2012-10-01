// 494TaskScheduler.cpp : Defines the entry point for the console application.
//

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

	BaseAnalyzer* p_analyzer;

public:
	AlgorithmicProfiler(BaseAnalyzer* analyzer) : p_analyzer(analyzer) {} 

	/**Get the time elapsed after running the profile for an algorithm.**/
	double getTimeElapsed() { return d_deltaTime; }

	/**Get whethr the algorithm is scheduable or not.**/
	bool getIsScheduable() { return b_isTaskSetScheduable; }

	/**Run profile for Liu and Layland.**/
	void profile() {
		time_t t_time_init, t_time_end;
		
		time(&t_time_init);
 		b_isTaskSetScheduable = p_analyzer->isTaskSetScheduable();
		time(&t_time_end);
		d_deltaTime = difftime(t_time_end, t_time_init); 
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
				<< "<" << lalProfiler.getTimeElapsed() << ">\n";

			//HYPERBOLIC BOUND TEST
			HBAnalyzer hbAnalayzer(taskSet);
			AlgorithmicProfiler hbProfiler(&hbAnalayzer);
			taskSet.printTaskSet();
                        hbProfiler.profile();
			cout << "Scheduable using RM Scheduling / Hyperbolic bound test: " 
				<< (hbProfiler.getIsScheduable() ? "Yes " : "No ")				
				<< "<" << hbProfiler.getTimeElapsed() << ">\n";


			//RM SCHEDULING, WCRT TEST
			taskSet.sortTaskSetByPeriod();
                        cout << "\n after sort \n";
			WCRTAnalyzer rmwcrtAnalyzer(taskSet);
                        cout << "\n after wmwcrtA \n";
			AlgorithmicProfiler rmwcrtProfiler(&rmwcrtAnalyzer);
                        cout << "\n after profiler \n";
			rmwcrtProfiler.profile();
                        cout << "\n after profile() \n";
			cout << "Scheduable using RM scheduling/ WCRT test: " 
				<< (rmwcrtProfiler.getIsScheduable() ? "Yes " : "No ")				
				<< "<" << rmwcrtProfiler.getTimeElapsed() << ">\n";

			//SJF SCHEDULING, WCRT TEST
			taskSet.sortTaskSetByWCET();
			WCRTAnalyzer sjfwcrtAnalyzer(taskSet);
			AlgorithmicProfiler sjfwcrtProfiler(&sjfwcrtAnalyzer);
			rmwcrtProfiler.profile();
			cout << "Scheduable using SJF scheduling/ WCRT test: " 
				<< (sjfwcrtProfiler.getIsScheduable() ? "Yes " : "No ")				
				<< "<" << sjfwcrtProfiler.getTimeElapsed() << ">\n";

			//MUF SCHEDULING, WCRT TEST
			taskSet.sortTaskSetByUtilization();
			WCRTAnalyzer mufwcrtAnalyzer(taskSet);
			AlgorithmicProfiler mufwcrtProfiler(&mufwcrtAnalyzer);
			mufwcrtProfiler.profile();
			cout << "Scheduable using MUF scheduling/ WCRT test: " 
				<< (mufwcrtProfiler.getIsScheduable() ? "Yes " : "No ")				
				<< "<" << mufwcrtProfiler.getTimeElapsed() << ">\n";
		}
	}
	return 0;
}

