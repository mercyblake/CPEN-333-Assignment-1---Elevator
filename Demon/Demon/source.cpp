//-------===---===---===---===-----=--------------------------------------------------------------------------
//-------|--\---|----|-----|--|- -/-\----------------------------------------------------------------------------
//-------|--|---|----===---===---|===|----------------------------------------------------------------------------
//-------|--/---|------|---|-----|---|----------------------------------------------------------------------------
//-------===---===---===---|-----|---|----------------------------------------------------------------------------
//DISPATCHER...


#include <stdio.h>
#include "..\..\..\..\RTExample\rt.h"
#include "dispatcher.h"
#include <stdlib.h>

static const int STOPPED =	0	;
static const int UP	=		1	;
static const int DOWN =		2	;

static const int NOTUSED =	0	;
static const int USING =	1	;

static const int CLOSED	=	0	;
static const int OPEN =		1	;

static const int NO_DESTINATION =	11 ;

static const int eleCount =	2;

//INIT: struct DP: int floor, int direction, int floors[10]


int	flag = 1;

mydatapooldata ele[eleCount];
int eleStatus[eleCount];
struct mydatapooldata ele1, ele2;

//GLOBAL VAR: CRendezvous
CRendezvous r1("StartRendezvous", 4); //global rendezvous for starting all 4 threads at the same time.
CRendezvous r2("EndRendezvous", 4); // global rendezvous for ending all 4 threads at the same time.

//INIT: SEMAPHORES - NEEDS REWRITING (generic)
CSemaphore		ps2("PS2", 0, 1) ;    // semaphore with initial value 0 and max value 1. What is the purpose of initial value?
CSemaphore		cs2("CS2", 1, 1) ;    // semaphore with initial value 1 and max value 1
CSemaphore		ps4("PS4", 0, 1) ;    // semaphore with initial value 0 and max value 1
CSemaphore		cs4("CS4", 1, 1) ;    // semaphore with initial value 1 and max value 1

//need gatekeeper for the dp.

UINT __stdcall DispatcherToElevator1(void *args)			
{
	

	CDataPool	dp1("Ele1", sizeof(struct mydatapooldata)) ;
	struct		mydatapooldata *Ele1DP = (struct mydatapooldata *)(dp1.LinkDataPool());

	while(flag) {	
		if (ps2.Read()>0) {
			ps2.Wait();

			ele[0] = *Ele1DP;

			cs2.Signal();
		}
		Sleep(5);
	}
	return 0 ;									
}

UINT __stdcall DispatcherToElevator2(void *args)			
{
	CDataPool	dp2("Ele2", sizeof(struct mydatapooldata)) ;
	struct		mydatapooldata *Ele2DP = (struct mydatapooldata *)(dp2.LinkDataPool()) ;

	while(flag) {
		if (ps4.Read()>0) {
			ps4.Wait();

			ele[1] = *Ele2DP;

			cs4.Signal();
		}
		Sleep(5);
	}
	return 0 ;									
}

//void dp_dispatch (struct mydatapooldata *MyDataPool, struct mydatapooldata *MyDataPool2) {
//	printf("Ele1 linked to datapool at address %p.....\n", MyDataPool) ;
//	printf("Ele1 Read value for Floor = %d\n", MyDataPool->floor) ;
//	printf("Ele1 Read value for Direction = %d\n", MyDataPool->direction) ; 
//	printf("Ele1 Read values for floor array = ") ;
//	for(int i=0; i < 10; i ++) {
//		printf("%d ", MyDataPool->floors[i]) ;
//	}
//	printf("\n");
//	
//	printf("Ele2 linked to datapool at address %p.....\n", MyDataPool2) ;
//	printf("Ele2 has value for Floor = %d\n", MyDataPool2->floor) ;
//	printf("Ele2 has value for Direction = %d\n", MyDataPool2->direction) ; 
//	printf("Ele2 has values for floor array = ") ;
//	for(int i=0; i < 10; i ++) {
//		printf("%d ", MyDataPool2->floors[i]) ;
//	}
//	printf("\n");
//}

// Start Messaging (not crit; has 1 getchar())
void MsgStart() {
	//Sleep(3000);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\nSystem IO online.");
	printf("\nThis building has: 2 elevators.");
	//Sleep(2000);
	//printf("\nWhich elevator(s) would you like to activate?");
	//printf("\n(Press the number for the one you would like to activate, or press 'a' for activating all, 'n' will exit):\n");
	printf("\nPress any key to start...");
	printf("\n");
}

// @@------^-----^--------^-------===----|\---|----@@
// @@-----/-\---/-\------/-\-------|-----|-\--|----@@
// @@----/---\-/---\----/===\------|-----|--\-|----@@
// @@---/-----V-----\--/-----\----===----|---\|----@@
int main(void) {
	char x = 0;
	int val = 0, status_p1 = 0, status_p2 = 0;
	int pipe1data = 0, pipe2data = 0;
	int freezeOn = 1, freezeOff = 0;
	//char KeyData;
	double counter = 0;
	int tdir = 0, tfloor = 0, assign = 0, assignTarget = 99;
	int eleRank[eleCount];
	int wow0 = 0, wow1 = 1, wow2 = 2, wow3 = 3, wow4 = 4, wow5 = 5, wow6 = 6, wow7 = 7, wow8 = 8, wow9 = 9;
	int msgFlag = 0;


	printf("Welcome to Freefall, the Safest Transport to Hell and Back!\n\n\n");
	Sleep(100);
	printf("Please wait while we recognize the installed elevators...\n");
	
	CPipe	pipe1	("PipeIOToDispatcher", 1024) ;
	CPipe	pipe2	("PipeDispatcherToIO", 1024) ;
	CPipe	pEle1	("PipeDispatcherToElevator1", 1024) ;
	CPipe	pEle1_1	("PipeDispatcherToElevator1_1", 1024) ;
	CPipe	pEle1_2 ("PipeDispatcherToElevator1_2", 1024) ;
	CPipe	pEle1_3 ("PipeDispatcherToElevator1_3", 1024) ;
	CPipe	pEle1_4 ("PipeDispatcherToElevator1_4", 1024) ;
	CPipe	pEle2	("PipeDispatcherToElevator2", 1024) ;
	CPipe	pEle2_1	("PipeDispatcherToElevator2_1", 1024) ;
	CPipe	pEle2_2 ("PipeDispatcherToElevator2_2", 1024) ;
	CPipe	pEle2_3 ("PipeDispatcherToElevator2_3", 1024) ;
	CPipe	pEle2_4 ("PipeDispatcherToElevator2_4", 1024) ;


	// INIT: PROCESS 3x: IO, Ele1, Ele2
	CProcess pio("T:\\Take\\Y4\\CPEN 333\\Assignment 1 - Elevator\\CPEN-333-Assignment-1---Elevator\\Demon\\Debug\\ElevatorIO.exe",	// pathlist to child program executable				
			NORMAL_PRIORITY_CLASS,			// priority
			OWN_WINDOW,						// process has its own window					
			ACTIVE						// process is active immediately
	);
	
	CProcess p1("T:\\Take\\Y4\\CPEN 333\\Assignment 1 - Elevator\\CPEN-333-Assignment-1---Elevator\\Demon\\Debug\\Otis1.exe",	// pathlist to child program executable				
			NORMAL_PRIORITY_CLASS,			// priority
			OWN_WINDOW,						// process has its own window					
			ACTIVE							// process is active immediately
	);

	CProcess p2("T:\\Take\\Y4\\CPEN 333\\Assignment 1 - Elevator\\CPEN-333-Assignment-1---Elevator\\Demon\\Debug\\Otis2.exe",	// pathlist to child program executable				
			NORMAL_PRIORITY_CLASS,			// priority
			OWN_WINDOW,						// process has its own window					
			ACTIVE							// process is active immediately
	);

	MsgStart();
	printf("\nwaiting for others...\n");
	r1.Wait();
	
	printf("Creating threads..\n");
	CThread	t1(DispatcherToElevator1, ACTIVE, NULL) ;
	CThread	t2(DispatcherToElevator2, ACTIVE, NULL) ;




	while( flag )  {

		
		if ((pipe1.TestForData()) >= sizeof(pipe1data) )	{		// if at least 1 integer in pipeline
			pipe1.Read(&pipe1data , sizeof(pipe1data)) ;		// read data from pipe 
			if (pipe1data % 10 == 9 && pipe1data/10 == 9) {
				flag = 0;
				break;
			}
			//Termination from Master (I/O);

			else if (pipe1data/10 == 7 && pipe1data % 10 == 0) {
				if (eleStatus[0] == 0) {
					eleStatus[0] = 1;
					pEle1_2.Write(&freezeOn, sizeof(int));
				}
			}
			else if (pipe1data/10 == 7 && pipe1data % 10 == 1) {
				if (eleStatus[1] == 0) {
					eleStatus[1] = 1;
					pEle2_2.Write(&freezeOn, sizeof(int));
				}
			}
			else if (pipe1data/10 == 7 && pipe1data % 10 == 5) {
				if (eleStatus[0] == 1) {
					eleStatus[0] = 0;
					pEle1_2.Write(&freezeOff, sizeof(int));
				}
			}
			else if (pipe1data/10 == 7 && pipe1data % 10 == 6) {
				if (eleStatus[1] == 1) {
					eleStatus[1] = 0;
					pEle2_2.Write(&freezeOff, sizeof(int));
				}
			}
			else {
				tfloor = pipe1data % 10;
				tdir = pipe1data/10;
				pipe1data = 0;
				if (tdir == 5 && eleStatus[0] == 0) {
					pEle1.Write(&tfloor, sizeof(int));
					pEle1_1.Write(&tfloor, sizeof(int));
				}
				else if (tdir == 6 && eleStatus[1] == 0) {
					pEle2.Write(&tfloor, sizeof(int));
					pEle2_1.Write(&tfloor, sizeof(int));
				}
				else {
					for (int i = 0; i < eleCount; i++) {
						if (ele[i].usage == NOTUSED) {
							eleRank[i] = abs(tfloor - ele[i].floor);
						}
						else if (ele[i].direction == UP) {
							if (tdir == UP) {		
								if (tfloor > ele[i].floor) {
									eleRank[i] = tfloor - ele[i].floor;
								} //elevator goes up (tfloor - ele.floor) because both going up
								  //and tfloor higher than ele.
								else {
									eleRank[i] = ( (9 - ele[i].floor) + (9 - tfloor) );
								} //elevator goes up to 9 and then comes down to catch,
								  //because typical behaviour of elevator is not to change dir until
								  //no more requests along the same dir.
							}
							else if (tdir == DOWN) {
								eleRank[i] = ( (9 - ele[i].floor) + (9 - tfloor) ); 
							} //elevator goes up ele.floor, then down to tfloor.
						}
						else if (ele[i].direction == DOWN) {
							if (tdir == UP) {
								eleRank[i] = (ele[i].floor + tfloor); 
							} //elevator goes down ele.floor, then up to tfloor.
							else if (tdir == DOWN) {
								if (tfloor < ele[i].floor) {
									eleRank[i] = ele[i].floor - tfloor;
								} //elevator goes down (ele.floor -tfloor),
								  //because target is downwards of down-moving elevator.
								else {
									eleRank[i] = ele[i].floor + tfloor;
								} //possible longest route is elevator go down to 0 and then come back up.
								  //same logic applied for tdir == UP and lower than ele.dir == UP.
								  //equal = missed.
							}
						}
					}
					for (int i = 0; i < eleCount; i++) {
						if (eleStatus[i] == 1) {
							eleRank[i] = 40;
						}
					}
					for (int i = 0; (i < 30 && assign == 0); i++) {
						for (int j = 0; (j < eleCount && assign == 0); j++) {
							if (eleRank[j] - i == 0) {
								assign = 1;
								assignTarget = j;
							}
						}
					}
					if (tdir == UP) {
						tfloor += 10;
					}

					else if(tdir == DOWN) {
						tfloor += 20;
					}

					if (assignTarget == 0) {
						pEle1.Write(&tfloor, sizeof(int));
					}
					else if (assignTarget == 1) {
						pEle2.Write(&tfloor, sizeof(int));
					}
					//cleanup.
					for (int i = 0; i < eleCount; i++) {
						eleRank[i] = 0;
					}
					assign = 0;
					assignTarget = 99;
					tdir = 0;
					tfloor = 0;
				}
			}
		}



		//if (counter == 500) {
		//	pEle1.Write(&wow5, sizeof(wow5));
		//	pEle1_1.Write(&wow5, sizeof(int));
		//	printf("%d\n", wow5);
		//}
		//if (counter == 15000) {
		//	pEle1.Write(&wow8, sizeof(wow8));
		//	printf("%d\n", wow8);
		//}
		//if (counter == 10000) {
		//	pEle1.Write(&wow5, sizeof(wow5));
		//	printf("%d\n", wow5);
		//}
		//if (counter == 23000) {
		//	pEle1.Write(&wow1, sizeof(wow5));
		//	pEle1_1.Write(&wow1, sizeof(int));
		//	printf("%d\n", wow1);
		//}
		//if (counter == 25000) {
		//	pEle1.Write(&wow9, sizeof(int));
		//	printf("%d\n", wow9);
		//}
		//if (counter == 28000) {
		//	pEle1.Write(&wow6, sizeof(int));
		//	pEle1_1.Write(&wow6, sizeof(int));
		//	printf("%d\n", wow6);
		//}
		//if (counter == 30000) {
		//	pEle1.Write(&wow2, sizeof(int));
		//	printf("%d\n", wow2);
		//}
		//if (counter == 34000) {
		//	pEle1.Write(&wow0, sizeof(int));
		//	printf("%d\n", wow0);
		//}
		//if (counter == 34000) {
		//	pEle1.Write(&wow0, sizeof(int));
		//	pEle1_1.Write(&wow0, sizeof(int));
		//	printf("%d\n", wow0);
		//}
		Sleep(1);
		counter++;

	}

	t1.WaitForThread() ;
	t2.WaitForThread() ;

	printf("\n\nExiting... (waiting on child processes)\n");
	r2.Wait();

	pio.WaitForProcess();
	p1.WaitForProcess();
	p2.WaitForProcess();

	return 0;
}