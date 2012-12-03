
/*******************************************************
 *
 *  Routing Table Routines for EECE 494, Assignment 2.
 *
 *  Created by _________, University of British Columbia
 *
 *  This is where you will put your routines to implement
 *  the routing table.  You should use the headers as
 *  supplied.  All your code will go in this
 *  file (and in cam.h)
 *
 ******************************************************/

#include "defs.h"
using namespace std;

/* Note that you probably want to have a data structure
   that is accessable to all routines in this file.  To do
   this, you probably would define some structure (perhaps
   called cam_t in the file cam.h.  Then you could 
   create a variable of this type here by using:

   cam_t cam;

   This will create  a variable called cam (of type cam_h)
   that can be accessed by any routine in this file.  */

cam_t cam_g;
cam_t* cam_g_ptr;
cam_t* cam_ptr;

string convertAddress(ip_address_t *address){
    int n1 = address->n1;
    int n2 = address->n2;
    int n3 = address->n3;
    int n4 = address->n4;

    string n1_string = static_cast<ostringstream*>( &(ostringstream() << n1) )->str();
    string n2_string = static_cast<ostringstream*>( &(ostringstream() << n2) )->str();
    string n3_string = static_cast<ostringstream*>( &(ostringstream() << n3) )->str();
    string n4_string = static_cast<ostringstream*>( &(ostringstream() << n4) )->str();

    string address_string = n1_string + "." + n2_string + "." + n3_string + "." + n4_string;
    return address_string;
}

void cam_init()
{
    cam_t cam;
    cam_ptr = (cam_t*) malloc (sizeof(cam));

    cam_g = cam;
    cam_g_ptr = (cam_t*) malloc (sizeof(cam));
}


void cam_add_entry(ip_address_t *address, int port)
{
    string address_string = convertAddress(address);
    cam_g.routing_table[address_string] = port;
}

int cam_lookup_address(ip_address_t *address)
{
    /* Write your code here */
    string address_string = convertAddress(address);

    unordered_map<string, double>::const_iterator got = cam_g.routing_table.find(address_string);

    int result_port;

    if ( got == cam_g.routing_table.end() )
	return -1;
    else{
	result_port = got->second;
        return result_port;
    }
}

void cam_free()
{
    /* Write your code here */;
    free(cam_g_ptr);
    free(cam_ptr);
}

