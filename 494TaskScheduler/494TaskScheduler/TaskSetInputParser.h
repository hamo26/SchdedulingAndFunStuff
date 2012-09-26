#pragma once
#include "TaskSet.h"
#include <queue>

const string BEGIN_TASK_SET = "<begin task set>";
const string END_TASK_SET = "<end task set>";

/** Class which takes an input text file and parses task sets.
	The class also emulates an iterator of sorts by encapsulating the parsed task sets
	and simply returning the next task set to be processed upon request.**/
class TaskSetInputParser
{
private:
	/** 
	Queue representing the task sets being iterated through and added.
	Assume that the first task to be added is the first one run.
	**/
	queue<TaskSet> q_taskSets;

	/**Internal method to add a task set. **/
	void addTaskSet(TaskSet taskSet);

	Task createTaskFromString(string inputTask); 

public:
	/** Default Constructor **/
	TaskSetInputParser();

	/**Method that parses input file. 
	   Returns true or false based on whether file operations have succeeded/failed
	**/
	bool parseInputFile(string relativeFilePath);

	/** Pops the next task set in the queue.**/
	TaskSet getNext();

	/** Determines whether task set is empty. **/
	bool isEmpty();

	/**Destructor**/
	~TaskSetInputParser(void);
};

