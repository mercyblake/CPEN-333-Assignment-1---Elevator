#ifndef __DISPATCHER__
#define __DISPATCHER__

struct 	mydatapooldata 	{	// start of structure template
		int floor ;			// floor corresponding to lifts current position
		int usage;
		int door;			
		int direction ;		// direction of travel of lift
		int floors[10] ;	// an array representing the floors and whether requests are set 
		int lights[10] ;
		int updir[10];
		int downdir[10];
} ;							// end of structure template



#endif;