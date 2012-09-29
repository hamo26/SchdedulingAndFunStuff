#pragma once
#include <iostream>

using namespace std;

class TaskGenerator
{
public:
	TaskGenerator(void);
	
	static void generateTasksAndWriteToFile(string fileName, double utilization, int numTasks, int numTaskSets);

	~TaskGenerator(void);
};

