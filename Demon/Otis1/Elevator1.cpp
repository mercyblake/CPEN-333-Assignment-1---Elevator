#include 	<stdio.h>
#include "..\..\..\..\RTExample\rt.h"
#include "..\Demon\elevator.h"

//#assignflag, semaphores, (additional rendezvous), 3x new pipes, eleCount
//#faultrank.
// end of structure template


CSemaphore		ps1("PS1", 0, 1) ;    // semaphore with initial value 0 and max value 1
CSemaphore		cs1("CS1", 1, 1) ;    // semaphore with initial value 1 and max value 1
CSemaphore		ps2("PS2", 0, 1) ;    // semaphore with initial value 0 and max value 1
CSemaphore		cs2("CS2", 1, 1) ;    // semaphore with initial value 1 and max value 1

CRendezvous r1("StartRendezvous", 4);
CRendezvous r2("EndRendezvous", 4);

int main(){
	char KeyData;
	int elevatorMoveCounter = 0, doorCounter = 0, counter = 0;
	int elevatorMoveFlag = 0;
	int pEle1data = NO_DESTINATION, pEle1_1data = NO_DESTINATION, pipe8data = 0;
	int pEle1data_floor = NO_DESTINATION;
	int pEle1data_dir = NO_DESTINATION;
	int pEle1_2data = 0;
	int destination = NO_DESTINATION;
	int updateDP = 1;
	int flag = 1;
	
	struct myDpData elevator;
	elevator.floor = 1;
	elevator.usage = NOTUSED;
	elevator.door = CLOSED;
	elevator.direction = STOPPED;
	for (int i = 0; i<10; i++) {
		elevator.floors[i] = 0;
	}
	for (int i = 0; i<10; i++) {
		elevator.lights[i] = 0;
	}
	for (int i = 0; i<10; i++) {
		elevator.updir[i] = 0;
	}
	for (int i = 0; i<10; i++) {
		elevator.downdir[i] = 0;
	}

	CDataPool dp("Ele1", sizeof(struct myDpData)) ;
	struct myDpData	 *MyDataPool = (struct myDpData *)(dp.LinkDataPool()) ;

	CPipe	pEle1	("PipeDispatcherToElevator1", 1024) ;
	CPipe	pEle1_1	("PipeDispatcherToElevator1_1", 1024) ; //only used for lights
	CPipe	pEle1_2 ("PipeDispatcherToElevator1_2", 1024) ; 
	CPipe	pEle1_3 ("PipeDispatcherToElevator1_3", 1024) ; //not used
	CPipe	pEle1_4 ("PipeDispatcherToElevator1_4", 1024) ; //not used
	CPipe	pipe8	("PipeIOToElevator1", 1024);

	
	r1.Wait();

	while(flag) {
		//termination from master
		if ((pipe8.TestForData()) >= sizeof(pipe8data) )	{		// if at least 1 integer in pipeline
			pipe8.Read(&pipe8data , sizeof(pipe8data)) ;		// read data from pipe 
			if (pipe8data % 10 == 9 && pipe8data/10 == 9) {
				break;
			}
		}



		//@@ Pipe 1: Destination
		if ((pEle1.TestForData()) >= sizeof(pEle1data) )	{		// if at least 1 integer in pipeline
			pEle1.Read(&pEle1data , sizeof(pEle1data)) ;		// read data from pipe 
			pEle1data_floor = pEle1data%10;
			pEle1data_dir = pEle1data/10;
			if (elevator.floors[pEle1data_floor] == 0) {
				elevator.floors[pEle1data_floor] = 1;
				updateDP = 1;
			}
			if (pEle1data_dir == UP) {
				elevator.updir[pEle1data_floor] = 1;
			}
			
			else if (pEle1data_dir == DOWN) {
				elevator.downdir[pEle1data_floor] = 1;
			}
			if (destination == NO_DESTINATION) {
				destination = pEle1data_floor;
			}
			else if (pEle1data_dir == UP && elevator.direction == UP && (destination - pEle1data_floor > 0)) {
				destination = pEle1data_floor;
			}
			else if (pEle1data_dir == DOWN && elevator.direction == DOWN && (pEle1data_floor - destination > 0)) {
				destination = pEle1data_floor;
			}			
			pEle1data = NO_DESTINATION;
			pEle1data_floor = NO_DESTINATION;
			pEle1data_dir = NO_DESTINATION;
		}

		// @@ Pipe 2: Lights
		if ((pEle1_1.TestForData()) >= sizeof(pEle1_1data) ){		// if at least 1 integer in pipeline
			pEle1_1.Read(&pEle1_1data , sizeof(pEle1_1data)) ;		// read data from pipe 
			elevator.lights[pEle1_1data] = 1;
		}

		// @@ Update Datapool
		if (updateDP == 1) {
			cs1.Wait();
			cs2.Wait();
			*MyDataPool = elevator;
			ps1.Signal();
			ps2.Signal();
			updateDP = 0;
		}
		
		//START CONDITION - Set elevator to USING. Set direction, and Set MoveFlag if destination != elevator.floor.
		if (elevator.usage == NOTUSED && destination != NO_DESTINATION) {
			elevator.usage = USING;
			updateDP = 1;
			if (destination > elevator.floor) {
				elevator.direction = UP;
				elevatorMoveFlag = 1;
			}
			else if (destination < elevator.floor) {
				elevator.direction = DOWN;
				elevatorMoveFlag = 1;
			}
		}

		//REASON TO OPEN DOOR. Primary reason is destination == elevator.floor && .door == CLOSED. 
		//Elevator.usage is failsafe.
		if (destination == elevator.floor && elevator.usage == USING && elevator.door == CLOSED) {
			elevator.door = OPEN;
			updateDP = 1;
			// destination calculation
			elevator.floors[destination] = 0; //destination met.
			elevator.lights[destination] = 0;

			if (elevator.direction == UP) {
				if (elevator.floor == 9) {
					for (int i = (destination - 1); (elevator.floor == destination) && (i >= 0); i--) {
						if (elevator.floors[i] == 1) {
							destination = i;
							elevator.direction = DOWN;
						}
					}
					if (elevator.floor == destination) {
						elevator.usage = NOTUSED;
						elevator.direction = STOPPED;
						destination = NO_DESTINATION;
					}
				}
			
				else if (elevator.floor < 9) {
					for (int i = (destination + 1); (elevator.floor == destination) && (i < 10); i++) {
						if (elevator.floors[i] == 1) {
							destination = i;
						}
					}
					if (elevator.floor == destination) {
						for (int i = (destination - 1); (elevator.floor == destination) && (i >= 0); i--) {
							if (elevator.floors[i] == 1) {
								destination = i;
								elevator.direction = DOWN;
							}
						}
						if (elevator.floor == destination) {
							elevator.usage = NOTUSED;
							elevator.direction = STOPPED;
							destination = NO_DESTINATION;
						}
					}
				}
				else {
					printf ("samefloor     \n");
				}
			}

			else if (elevator.direction == DOWN) {
				if (elevator.floor == 0) {
					for (int i = (destination + 1); (elevator.floor == destination) && (i < 10); i++) {
						if (elevator.floors[i] == 1) {
							destination = i;
							elevator.direction = UP;
						}
					}
					if (elevator.floor == destination) {
						elevator.usage = NOTUSED;
						elevator.direction = STOPPED;
						destination = NO_DESTINATION;
					}
				}

				else if (elevator.floor > 0) {
					for (int i = (destination - 1); (elevator.floor == destination) && (i >= 0); i--) {
						if (elevator.floors[i] == 1) {
							destination = i;
						}
					}
					if (elevator.floor == destination) {
						for (int i = (destination + 1); (elevator.floor == destination) && (i < 10); i++) {
							if (elevator.floors[i] == 1) {
								destination = i;
								elevator.direction = UP;
							}
						}
						if (elevator.floor == destination) {
							elevator.usage = NOTUSED;
							elevator.direction = STOPPED;
							destination = NO_DESTINATION;
						}
					}
				}
				else { printf("Error 2\n");
				getchar();
				}
			}
			else {
				elevator.usage = NOTUSED;
				destination = NO_DESTINATION;
			}
			if (elevator.direction == UP) { //!potential bug here
				elevator.updir[elevator.floor] = 0;
			}
			else if (elevator.direction == DOWN) {
				elevator.downdir[elevator.floor] = 0;
			}
			else if (elevator.direction == STOPPED) {
				elevator.updir[elevator.floor] = 0;
				elevator.downdir[elevator.floor]= 0;
			}
		}
		
		//door open counter.
		if (elevator.door == OPEN) {
			doorCounter++;
		}

		//close door.
		if (doorCounter >= 1500) { //should be 1501 or > 1500 but w/e
			updateDP = 1;
			elevator.door = CLOSED;
			doorCounter = 0;

			if (elevator.direction == UP || elevator.direction == DOWN) {
				elevatorMoveFlag = 1;
			}
		}


		//move elevator counter.
		if (elevatorMoveFlag == 1) {
			elevatorMoveCounter++;
		}			

		//moved a floor.
		if (elevatorMoveCounter >= 1000) { //should be 1001 or > 1000 but w/e
			updateDP = 1;
			elevatorMoveCounter = 0;
			if (elevator.direction == UP) {
				elevator.floor++;
			}
			else if (elevator.direction == DOWN) {
				elevator.floor--;
			}

			if (elevator.floor == destination) {
				elevatorMoveFlag = 0;
			}
		}



		counter++;

		if (counter >= 100) {
			MOVE_CURSOR(0,0);
			printf("elevator.floor: %d\n", elevator.floor);
			if (elevator.door == CLOSED)
				printf("elevator.door : CLOSED\n");
			else
				printf("elevator.door : OPEN\n");
			printf("elevator.usage: %d\n", elevator.usage);
			if (elevator.direction == DOWN) 
				printf("elevator.direction: DOWN    \n");
			else if (elevator.direction == UP)
				printf("elevator.direction: UP      \n");
			else
				printf("elevator.direction: STOPPED \n");
			for (int i = 0; i < 10; i++) {
				if (elevator.lights[i] == 1)
					TEXT_COLOUR(14);
				printf(" %d  ",i);
				TEXT_COLOUR(7);
			}
			printf("\n");
			for (int i = 0; i < 10; i++) 
				printf("[%d] ", elevator.floors[i]);
			printf("\n");
			for (int i = 0; i < 10; i++) {
				if (elevator.updir[i] == 1)
					TEXT_COLOUR(14);
				else 
					TEXT_COLOUR(7);
				printf("[%d] ", elevator.updir[i]);
			}
			TEXT_COLOUR(7);
			printf("updir\n");
			for (int i = 0; i < 10; i++) {
				if (elevator.downdir[i] == 1)
					TEXT_COLOUR(14);
				else 
					TEXT_COLOUR(7);
				printf("[%d] ", elevator.downdir[i]);
			}
			TEXT_COLOUR(7);
			printf("downdir\n");
			if (destination == 11)
				printf ("\ndestination: NO_DESTINATION\n");
			else
				printf ("\ndestination: %d              \n", destination);
			printf ("elevatorDoorFlag: %d      elevatorMoveFlag: %d     \n", elevator.door, elevatorMoveFlag);
			fflush(stdout);
			counter = 0;
		}

		Sleep(1);

		if (TEST_FOR_KEYBOARD() != 0) {
			KeyData = getch() ;					// read next character from keyboard
			if (KeyData == 'x'){
				break;
			}	
		}
		
		//Freeze

		if ((pEle1_2.TestForData()) >= sizeof(pEle1_2data) )	{		// if at least 1 integer in pipeline
			pEle1_2.Read(&pEle1_2data , sizeof(pEle1_2data)) ;		// read data from pipe 
			if (pEle1_2data == 1) {
				while (pEle1_2data == 1) {
					if ((pEle1_2.TestForData()) >= sizeof(pEle1_2data)) {	
						pEle1_2.Read(&pEle1_2data , sizeof(pEle1_2data)) ;
					}
					if ((pipe8.TestForData()) >= sizeof(pipe8data) )	{		// if at least 1 integer in pipeline
						pipe8.Read(&pipe8data , sizeof(pipe8data)) ;		// read data from pipe 
						if (pipe8data % 10 == 9 && pipe8data/10 == 9) {
							flag = 0;
							break;
						}
					}
					MOVE_CURSOR(0,1);
					printf("FROZEN!");
					fflush(stdout);
					Sleep(1);
				}
			}
		}

	}
	printf("Exiting... (waiting on other processes)");
	r2.Wait();
	return 0;
}