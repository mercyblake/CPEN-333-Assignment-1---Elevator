#ifndef __IO__
#define __IO__

#include <stdio.h>

/* stdio.h
 * printf
 */

#include <stdlib.h>

/* stdlib.h
 * 
 */

//#include <iostream>
//
///* iostream // QUES: Why is it not a header file?
// *
// */
//
//#include <string>
//
///* string
// * - concatenate strings to create DPs and pipelines
// */

#include "rt.h"

/* rt.h
 * Dialog with win32 for multithreaded.
 */


using namespace std;

#endif

/**
  * Elevator definitions. Class imported from Demon: elevator.h
  *
  *
  */
#ifndef __ELEVATOR__
#define __ELEVATOR__

struct 	elevatorStatus 	{	// start of structure template
		int floor ;			// floor corresponding to lifts current position
		int usage;
		int door;			
		int direction ;		// direction of travel of lift
		int floors[10] ;	// an array representing the floors and whether requests are set 
		int lights[10] ;
		int updir[10];
		int downdir[10];
} ;							// end of structure template

static const int STOPPED =	0	;
static const int UP	=		1	;
static const int DOWN =		2	;

static const int NOTUSED =	0	;
static const int USING =	1	;

static const int CLOSED	=	0	;
static const int OPEN =		1	;

static const int NO_DESTINATION =	11 ;


#define IS_CAN_BS1(BS1) ((BS1) <= (uint8_t)0x0F)
#endif