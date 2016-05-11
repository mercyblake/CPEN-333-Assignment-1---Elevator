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

// IO case states.
#define IO_state_MainMenu 0 //waits for first character
#define IO_state_PrintMainMenu 1 //prints main menu and enters it
#define IO_state_Outside_PrintFirstPrompt 2 //asking for floor number, and printing received direction.
#define IO_state_Outside_WaitForSecondButton 3 //waiting for user input of current floor number.
#define IO_state_Outside_PrintSecondPrompt 4 //restates the received current floor number.
#define IO_state_Inside_PrintFirstPrompt 5 //restates the received elevator number, ask for target floor number.
#define IO_state_Inside_WaitForSecondButton 6 //waiting for user input of desired floor number.
#define IO_state_Inside_PrintSecondPrompt 7 //restates the received desired floor number
#define IO_state_DisplayTwoSeconds 8 //displays the completed instruction for two seconds before returning to main menu
#define IO_state_Admin_PrintFirstPrompt 9 //prints first prompt of ADMIN
#define IO_state_Admin_WaitForSecondButton 10
#define IO_state_Admin_PrintSecondPrompt 11
#define IO_state_PrintReceivedWrongCommand 12 //tells the user that a wrong command has been received.
#define IO_state_PipeToDispatcher 13
#define IO_state_PrintElevatorDoesNotExist 14


#endif

#ifndef __ELEVATOR__
#define __ELEVATOR__
/**
  * Elevator definitions. Class imported from Demon: elevator.h
  *
  *
  */


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


#endif