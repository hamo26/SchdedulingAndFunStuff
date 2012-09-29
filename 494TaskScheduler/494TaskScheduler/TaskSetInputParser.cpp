#include "stdafx.h"
#include "TaskSetInputParser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

TaskSetInputParser::TaskSetInputParser(void)
{
	//EMPTY.
}

bool TaskSetInputParser::parseInputFile(string relativeFilePath)
{
	ifstream inputFile;

	inputFile.open(relativeFilePath);
	if (!inputFile) {
		cout << "Error opening file with relative path: " << relativeFilePath;
		return false;
	} else {
		string currentLine;
		vector<Task> *currentTaskSet = NULL;
		while (getline(inputFile, currentLine)) {
			if (currentLine == BEGIN_TASK_SET) {
				currentTaskSet = new vector<Task>();
				continue;
			} else if(currentLine == END_TASK_SET) {
				if (currentTaskSet) {
					q_taskSets.push(*currentTaskSet);
				}
				delete currentTaskSet;
				continue;
			} else if(currentLine == "") {
				continue;
			} else {
				try 
				{
					currentTaskSet->push_back(createTaskFromString(currentLine));
					continue;
				} catch (...) {
					cout << "Could not parse: " << currentLine << "\n";
					continue;
				}
			}
		}
		return true;
	}
}

Task TaskSetInputParser::createTaskFromString(string inputTaskLine)
{
	string wcet, relativeDeadline, period;

	istringstream inputLine(inputTaskLine);
	getline(inputLine, wcet, ' ');
	getline(inputLine, relativeDeadline, ' ');
	getline(inputLine, period, ' ');

	if (wcet == "" || relativeDeadline == "" || period == "") { throw; }

	double d_wcet = atof(wcet.c_str());
	double d_relativeDeadline = atof(relativeDeadline.c_str());
	int i_period = atoi(period.c_str());

	return Task(d_wcet, d_relativeDeadline, i_period);
}

void TaskSetInputParser::addTaskSet(TaskSet taskSet) 
{
	q_taskSets.push(taskSet);
}

TaskSet TaskSetInputParser::getNext()
{
	TaskSet topTaskSet = q_taskSets.front();
	//Need to signal that a list of task sets have been iterated through and nothing left to pop.
	q_taskSets.pop();
	return topTaskSet;
}
				
bool TaskSetInputParser::isEmpty() 
{
	return q_taskSets.empty();
}

int TaskSetInputParser::getTaskSetSize()
{
	return q_taskSets.size();
}

TaskSetInputParser::~TaskSetInputParser(void)
{
	//EMPTY
}
