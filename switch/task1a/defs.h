
/*******************************************************
 *
 *  Global Header File for EECE 494, Assignment 2
 *
 *  Created by Steve Wilton, University of British Columbia
 *
 *  This file contains definitions that everyone might need 
 *  for Assignment 4
 *
 ******************************************************/


/*-------------------------------------------------
 * 
 * These are some standard definitions.  Including
 * them here means that the include statements don't 
 * have to be repeated in each file.
 *
 *-------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unordered_map>

/*----------------------------------------------------
 * The maximum length of a line in the input file
 *----------------------------------------------------*/

#define BUFFERLENGTH 400
#define BIGINT (1e20)

#define MAX(a,b)  ((a)>(b)?(a):(b))

#define BOOL char
#define TRUE ((BOOL)1)
#define FALSE ((BOOL)0)

#include "misc.h"
#include "cam.h"
#include "ext.h"

//Defines the max num of entries
#define MAX_ENTRIES 400000
