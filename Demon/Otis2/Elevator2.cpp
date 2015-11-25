#include <stdio.h>
#include "..\..\..\..\RTExample\rt.h"
#include "..\Demon\elevator.h"


CRendezvous r1("StartRendezvous", 4);
CRendezvous r2("EndRendezvous", 4);

CSemaphore		ps3("PS3", 0, 1) ;    // semaphore with initial value 0 and max value 1
CSemaphore		cs3("CS3", 1, 1) ;    // semaphore with initial value 1 and max value 1
CSemaphore		ps4("PS4", 0, 1) ;    // semaphore with initial value 0 and max value 1
CSemaphore		cs4("CS4", 1, 1) ;    // semaphore with initial value 1 and max value 1



int main(){
	char KeyData;
	int elevatorMoveCounter = 0, doorCounter = 0, counter = 0;
	int elevatorMoveFlag = 0;
	int pEle2data = NO_DESTINATION, pEle2_1data = NO_DESTINATION; 
	int pipe9data = 0;
	int pEle2data_floor = NO_DESTINATION;
	int pEle2data_dir = NO_DESTINATION;
	int pEle2_2data = 0;
	int destination = NO_DESTINATION;
	int updateDP = 1;
	int flag = 1;

	CDataPool dp("Ele2", sizeof(struct myDpData)) ;	
	struct myDpData 	 *MyDataPool = (struct myDpData *)(dp.LinkDataPool()) ;
	
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
	
	CPipe	pEle2	("PipeDispatcherToElevator2", 1024) ;
	CPipe	pEle2_1	("PipeDispatcherToElevator2_1", 1024) ;
	CPipe	pEle2_2 ("PipeDispatcherToElevator2_2", 1024) ;
	CPipe	pEle2_3 ("PipeDispatcherToElevator2_3", 1024) ;
	CPipe	pEle2_4 ("PipeDispatcherToElevator2_4", 1024) ;
	CPipe	pipe9	("PipeIOToElevator2", 1024);

	r1.Wait();



	while(flag) {
		//termination from master.
		if ((pipe9.TestForData()) >= sizeof(pipe9data) )	{		// if at least 1 integer in pipeline
			pipe9.Read(&pipe9data , sizeof(pipe9data)) ;		// read data from pipe 
			if (pipe9data % 10 == 9 && pipe9data/10 == 9) {
				break;
			}
		}



		//@@ Pipe 1: Destination
		if ((pEle2.TestForData()) >= sizeof(pEle2data) )	{		// if at least 1 integer in pipeline
			pEle2.Read(&pEle2data , sizeof(pEle2data)) ;		// read data from pipe 
			pEle2data_floor = pEle2data%10;
			pEle2data_dir = pEle2data/10;
			if (elevator.floors[pEle2data_floor] == 0) {
				elevator.floors[pEle2data_floor] = 1;
				updateDP = 1;
			}
			if (pEle2data_dir == UP) {
				elevator.updir[pEle2data_floor] = 1;
			}
			
			else if (pEle2data_dir == DOWN) {
				elevator.downdir[pEle2data_floor] = 1;
			}
			if (destination == NO_DESTINATION) {
				destination = pEle2data_floor;
			}
			else if (pEle2data_dir == UP && elevator.direction == UP && (destination - pEle2data_floor > 0)) {
				destination = pEle2data_floor;
			}
			else if (pEle2data_dir == DOWN && elevator.direction == DOWN && (pEle2data_floor - destination > 0)) {
				destination = pEle2data_floor;
			}			
			pEle2data = NO_DESTINATION;
			pEle2data_floor = NO_DESTINATION;
			pEle2data_dir = NO_DESTINATION;
		}

		// @@ Pipe 2: Lights
		if ((pEle2_1.TestForData()) >= sizeof(pEle2_1data) ){		// if at least 1 integer in pipeline
			pEle2_1.Read(&pEle2_1data , sizeof(pEle2_1data)) ;		// read data from pipe 
			elevator.lights[pEle2_1data] = 1;
		}

		// @@ Update Datapool
		if (updateDP == 1) {
			cs3.Wait();
			cs4.Wait();
			*MyDataPool = elevator;
			ps3.Signal();
			ps4.Signal();
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
			else { printf("ERROR 4        \n"); // why were we moving ROFL?
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

		if ((pEle2_2.TestForData()) >= sizeof(pEle2_2data) )	{		// if at least 1 integer in pipeline
			pEle2_2.Read(&pEle2_2data , sizeof(pEle2_2data)) ;		// read data from pipe 
			if (pEle2_2data == 1) {
				while (pEle2_2data == 1) {
					if ((pEle2_2.TestForData()) >= sizeof(pEle2_2data)) {	
						pEle2_2.Read(&pEle2_2data , sizeof(pEle2_2data)) ;
					}
					if ((pipe9.TestForData()) >= sizeof(pipe9data) )	{		// if at least 1 integer in pipeline
						pipe9.Read(&pipe9data , sizeof(pipe9data)) ;		// read data from pipe 
						if (pipe9data % 10 == 9 && pipe9data/10 == 9) {
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