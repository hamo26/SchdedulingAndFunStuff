#pragma once
#include <iostream>

using namespace std;

class TaskGrapher
{
public:
	TaskGrapher(void);

	static void generateAndWriteToFile(string fileName, double utilization, int numTasks);
	~TaskGrapher(void);
};

