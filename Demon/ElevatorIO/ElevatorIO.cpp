#include 	<stdio.h>
#include "..\..\..\rt.h"

static const int STOPPED =	0	;
static const int UP	=		1	;
static const int DOWN =		2	;

static const int NOTUSED =	0	;
static const int USING =	1	;

static const int CLOSED	=	0	;
static const int OPEN =		1	;

static const int NO_DESTINATION =	11 ;

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

int		flag = 1; //flag to allow threads to continue.

struct		mydatapooldata Ele1Status, Ele2Status;

//@ INIT: DP 2x (also used for connect


CSemaphore		ps1("PS1", 0, 1) ;    // semaphore with initial value 0 and max value 1
CSemaphore		cs1("CS1", 1, 1) ;    // semaphore with initial value 1 and max value 1
CSemaphore		ps3("PS3", 0, 1) ;    // semaphore with initial value 0 and max value 1
CSemaphore		cs3("CS3", 1, 1) ;    // semaphore with initial value 1 and max value 1

CMutex			*m1 = new CMutex ("new_int"); // mutex to give writing access.

CRendezvous		r1("StartRendezvous", 4); //rendezvous point for all programs to start together.
CRendezvous		r2("EndRendezvous", 4); //rendezvous point for all programs to exit together.

CPipe		pipe1	("PipeIOToDispatcher", 1024);
CPipe		pipe2	("PipeDispatcherToIO", 1024);
CPipe		pipe3	("PipeIOToElevator1", 1024);
CPipe		pipe4	("PipeIOToElevator2", 1024);

UINT __stdcall IOToElevator1(void *args)			
{
	CDataPool	dp1("Ele1", sizeof(struct mydatapooldata)) ;
	struct		mydatapooldata *Ele1DP = (struct mydatapooldata *)(dp1.LinkDataPool());

	while(flag) {	
		if (ps1.Read()>0) {
			ps1.Wait();
			m1->Wait();

			Ele1Status = *Ele1DP;
			
			m1->Signal();
			cs1.Signal();
		}

		//FLOOR 9
		m1->Wait();
		MOVE_CURSOR(0,1);
		printf("          |");
		if (Ele1Status.floor == 9) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(0,2);
		printf("Floor 9   |");
		if (Ele1Status.floor == 9) {
			printf("|");
			if (Ele1Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(0,3);
		printf("          |");
		if (Ele1Status.floor == 9) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");

		//FLOOR 8
		MOVE_CURSOR(0,5);
		printf("          |");
		if (Ele1Status.floor == 8) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(0,6);
		printf("Floor 8   |");
		if (Ele1Status.floor == 8) {
			printf("|");
			if (Ele1Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(0,7);
		printf("          |");
		if (Ele1Status.floor == 8) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");

		//FLOOR 7
		MOVE_CURSOR(0,9);
		printf("          |");
		if (Ele1Status.floor == 7) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(0,10);
		printf("Floor 7   |");
		if (Ele1Status.floor == 7) {
			printf("|");
			if (Ele1Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(0,11);
		printf("          |");
		if (Ele1Status.floor == 7) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");

		//FLOOR 6
		MOVE_CURSOR(0,13);
		printf("          |");
		if (Ele1Status.floor == 6) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(0,14);
		printf("Floor 6   |");
		if (Ele1Status.floor == 6) {
			printf("|");
			if (Ele1Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(0,15);
		printf("          |");
		if (Ele1Status.floor == 6) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");

		//FLOOR 5
		MOVE_CURSOR(0,17);
		printf("          |");
		if (Ele1Status.floor == 5) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(0,18);
		printf("Floor 5   |");
		if (Ele1Status.floor == 5) {
			printf("|");
			if (Ele1Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(0,19);
		printf("          |");
		if (Ele1Status.floor == 5) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");

		//FLOOR 4
		MOVE_CURSOR(0,21);
		printf("          |");
		if (Ele1Status.floor == 4) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(0,22);
		printf("Floor 4   |");
		if (Ele1Status.floor == 4) {
			printf("|");
			if (Ele1Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(0,23);
		printf("          |");
		if (Ele1Status.floor == 4) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");

		//FLOOR 3
		MOVE_CURSOR(0,25);
		printf("          |");
		if (Ele1Status.floor == 3) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(0,26);
		printf("Floor 3   |");
		if (Ele1Status.floor == 3) {
			printf("|");
			if (Ele1Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(0,27);
		printf("          |");
		if (Ele1Status.floor == 3) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");

		//FLOOR 2
		MOVE_CURSOR(0,29);
		printf("          |");
		if (Ele1Status.floor == 2) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(0,30);
		printf("Floor 2   |");
		if (Ele1Status.floor == 2) {
			printf("|");
			if (Ele1Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(0,31);
		printf("          |");
		if (Ele1Status.floor == 2) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");

		//FLOOR 1
		MOVE_CURSOR(0,33);
		printf("          |");
		if (Ele1Status.floor == 1) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");

		MOVE_CURSOR(0,34);
		printf("Floor 1   |");
		if (Ele1Status.floor == 1) {
			printf("|");
			if (Ele1Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");

		MOVE_CURSOR(0,35);
		printf("          |");
		if (Ele1Status.floor == 1) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");

		//FLOOR 0
		MOVE_CURSOR(0,37);
		printf("          |");
		if (Ele1Status.floor == 0) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(0,38);
		printf("Floor 0   |");
		if (Ele1Status.floor == 0) {
			printf("|");
			if (Ele1Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(0,39);
		printf("          |");
		if (Ele1Status.floor == 0) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");


		MOVE_CURSOR(0,41);
		printf("Buttons in Ele1");
		MOVE_CURSOR(7,42);
		for (int i = 7; i < 10; i++) {
			if (Ele1Status.lights[i] == 1) {
				TEXT_COLOUR(14);
			}
			printf("[%d] ", i);
			TEXT_COLOUR(7);
		}
		MOVE_CURSOR(7,43);
		for (int i = 4; i < 7; i++) {
			if (Ele1Status.lights[i] == 1) {
				TEXT_COLOUR(14);
			}
			printf("[%d] ", i);
			TEXT_COLOUR(7);
		}
		MOVE_CURSOR(7,44);
		for (int i = 1; i < 4; i++) {
			if (Ele1Status.lights[i] == 1) {
				TEXT_COLOUR(14);
			}
			printf("[%d] ", i);
			TEXT_COLOUR(7);
		}
		MOVE_CURSOR(11,45);
		if (Ele1Status.lights[0] == 1) {
			TEXT_COLOUR(14);
		}
		printf("[0]");
		TEXT_COLOUR(7);
		//printf("Ele1 Read value for Floor = %d                                \n", Ele1Status.floor) ;
		//printf("Ele1 Read value for Usage = %d                                \n", Ele1Status.usage);
		//printf("Ele1 Read value for Door = %d                                 \n", Ele1Status.direction) ; 
		//printf("Ele1 Read value for Direction = %d                            \n", Ele1Status.direction) ; 
		//printf("Ele1 Read values for floor array = ") ;
		//for(int i=0; i < 10; i ++) {
		//	printf("%d ", Ele1Status.floors[i]) ;
		//}
		fflush(stdout);
		MOVE_CURSOR(0,50);
		fflush(stdout);
		m1->Signal();

		SLEEP(10);
	}
	return 0 ;									
}

UINT __stdcall IOToElevator2(void *args)
{								
	
	CDataPool	dp2("Ele2", sizeof(struct mydatapooldata)) ;
	struct		mydatapooldata *Ele2DP = (struct mydatapooldata *)(dp2.LinkDataPool()) ;

	while(flag) {
		
		if (ps3.Read()>0) {
			ps3.Wait();
			m1->Wait();
			
			Ele2Status = *Ele2DP;
			
			m1->Signal();
			cs3.Signal();
		}
		
		m1->Wait();

		//FLOOR 9
		MOVE_CURSOR(29,1);
		printf("          |");
		if (Ele2Status.floor == 9) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(29,2);
		printf("          |");
		if (Ele2Status.floor == 9) {
			printf("|");
			if (Ele2Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(29,3);
		printf("          |");
		if (Ele2Status.floor == 9) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");

		//FLOOR 8
		MOVE_CURSOR(29,5);
		printf("          |");
		if (Ele2Status.floor == 8) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(29,6);
		printf("          |");
		if (Ele2Status.floor == 8) {
			printf("|");
			if (Ele2Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(29,7);
		printf("          |");
		if (Ele2Status.floor == 8) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");

		//FLOOR 7
		MOVE_CURSOR(29,9);
		printf("          |");
		if (Ele2Status.floor == 7) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(29,10);
		printf("          |");
		if (Ele2Status.floor == 7) {
			printf("|");
			if (Ele2Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(29,11);
		printf("          |");
		if (Ele2Status.floor == 7) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");

		//FLOOR 6
		MOVE_CURSOR(29,13);
		printf("          |");
		if (Ele2Status.floor == 6) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(29,14);
		printf("          |");
		if (Ele2Status.floor == 6) {
			printf("|");
			if (Ele2Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(29,15);
		printf("          |");
		if (Ele2Status.floor == 6) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");

		//FLOOR 5
		MOVE_CURSOR(29,17);
		printf("          |");
		if (Ele2Status.floor == 5) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(29,18);
		printf("          |");
		if (Ele2Status.floor == 5) {
			printf("|");
			if (Ele2Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(29,19);
		printf("          |");
		if (Ele2Status.floor == 5) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");

		//FLOOR 4
		MOVE_CURSOR(29,21);
		printf("          |");
		if (Ele2Status.floor == 4) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(29,22);
		printf("          |");
		if (Ele2Status.floor == 4) {
			printf("|");
			if (Ele2Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(29,23);
		printf("          |");
		if (Ele2Status.floor == 4) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");

		//FLOOR 3
		MOVE_CURSOR(29,25);
		printf("          |");
		if (Ele2Status.floor == 3) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(29,26);
		printf("          |");
		if (Ele2Status.floor == 3) {
			printf("|");
			if (Ele2Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(29,27);
		printf("          |");
		if (Ele2Status.floor == 3) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");

		//FLOOR 2
		MOVE_CURSOR(29,29);
		printf("          |");
		if (Ele2Status.floor == 2) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(29,30);
		printf("          |");
		if (Ele2Status.floor == 2) {
			printf("|");
			if (Ele2Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(29,31);
		printf("          |");
		if (Ele2Status.floor == 2) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");

		//FLOOR 1
		MOVE_CURSOR(29,33);
		printf("          |");
		if (Ele2Status.floor == 1) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");

		MOVE_CURSOR(29,34);
		printf("          |");
		if (Ele2Status.floor == 1) {
			printf("|");
			if (Ele2Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");

		MOVE_CURSOR(29,35);
		printf("          |");
		if (Ele2Status.floor == 1) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");

		//FLOOR 0
		MOVE_CURSOR(29,37);
		printf("          |");
		if (Ele2Status.floor == 0) 
			printf("._.");
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(29,38);
		printf("          |");
		if (Ele2Status.floor == 0) {
			printf("|");
			if (Ele2Status.door == OPEN) 
				printf("O");
			else
				printf("-");
			printf("|");
		}
		else
			printf("   ");
		printf("|   ");
		MOVE_CURSOR(29,39);
		printf("          |");
		if (Ele2Status.floor == 0) 
			printf("^-^");
		else
			printf("   ");
		printf("|   ");


		MOVE_CURSOR(29,41);
		printf("           Ele2");
		MOVE_CURSOR(36,42);
		for (int i = 7; i < 10; i++) {
			if (Ele2Status.lights[i] == 1) {
				TEXT_COLOUR(14);
			}
			printf("[%d] ", i);
			TEXT_COLOUR(7);
		}
		MOVE_CURSOR(36,43);
		for (int i = 4; i < 7; i++) {
			if (Ele2Status.lights[i] == 1) {
				TEXT_COLOUR(14);
			}
			printf("[%d] ", i);
			TEXT_COLOUR(7);
		}
		MOVE_CURSOR(36,44);
		for (int i = 1; i < 4; i++) {
			if (Ele2Status.lights[i] == 1) {
				TEXT_COLOUR(14);
			}
			printf("[%d] ", i);
			TEXT_COLOUR(7);
		}
		MOVE_CURSOR(40,45);
		if (Ele2Status.lights[0] == 1) {
			TEXT_COLOUR(14);
		}
		printf("[0]");
		TEXT_COLOUR(7);

		fflush(stdout);
		MOVE_CURSOR(0,50);
		fflush(stdout);
		m1->Signal();


		SLEEP(10);
	}
	return 0 ;									
}

//@ MAIN
int main(){
	char KeyData;
	char KeyData1;
	int iKeyData;
	int preflag = 1;
	int termination_code = 99;
	//@ SEQ: STARTUP RENDEZVOUS
	printf("Pretending to rendezvous by sleeping 3 seconds...\n");
	Sleep(3000);
	r1.Wait();
	printf("let's go!");
	Sleep(1000);

	MOVE_CURSOR(0,0);
	printf("----------+---+---------------------+---+--------------------");
	MOVE_CURSOR(0,4);
	printf("----------+---+---------------------+---+--------------------");
	MOVE_CURSOR(0,8);
	printf("----------+---+---------------------+---+--------------------");
	MOVE_CURSOR(0,12);
	printf("----------+---+---------------------+---+--------------------");
	MOVE_CURSOR(0,16);
	printf("----------+---+---------------------+---+--------------------");
	MOVE_CURSOR(0,20);
	printf("----------+---+---------------------+---+--------------------");
	MOVE_CURSOR(0,24);
	printf("----------+---+---------------------+---+--------------------");
	MOVE_CURSOR(0,28);
	printf("----------+---+---------------------+---+--------------------");
	MOVE_CURSOR(0,32);
	printf("----------+---+---------------------+---+--------------------");
	MOVE_CURSOR(0,36);
	printf("----------+---+---------------------+---+--------------------");
	MOVE_CURSOR(0,40);
	printf("----------+---+---------------------+---+--------------------");
	fflush(stdout);

	//@ INIT: THREADS
	CThread	t1(IOToElevator1, ACTIVE, NULL) ;
	CThread	t2(IOToElevator2, ACTIVE, NULL) ;

	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	//MOVE_CURSOR(0,0);
	//printf("Press x at any time to exit...                      ");
	//fflush(stdout);

	while(flag) {
		
		// Master Termination.
		if (preflag == 0) {
			pipe3.Write(&termination_code, sizeof(int));
			pipe4.Write(&termination_code, sizeof(int));
			flag = 0;
			Sleep(50);
			pipe1.Write(&termination_code, sizeof(int));
			break;
		}

		if (TEST_FOR_KEYBOARD() != 0) {
			KeyData = getch() ;					// read next character from keyboard
			
			m1->Wait();
			MOVE_CURSOR(0,50);
			printf("first character = %c            \n", KeyData);
			printf("second character?               \n");
			printf("                                                   \n");
			printf("                                                   \n");
			fflush(stdout);
			m1->Signal();

			while (TEST_FOR_KEYBOARD() == 0) {}
			KeyData1 = getch();
			m1->Wait();
			MOVE_CURSOR(0,51);
			printf("second character = %c           \n", KeyData1);
			printf("command entered is: %c%c        \n", KeyData, KeyData1);

			//Command Organization.

			if (KeyData == 'e' && KeyData1 == 'e'){
				preflag = 0;
				printf("terminating. (2 seconds)                  \n   ");
				Sleep(2000);
			}
			else if (KeyData == '-' && KeyData1 == '1') {
				iKeyData = 70;
				pipe1.Write(&iKeyData, sizeof(int));
				printf("FREEZING Elevator 1!                            ");
			}

			else if (KeyData == '-' && KeyData1 == '2') {
				iKeyData = 71;
				pipe1.Write(&iKeyData, sizeof(int));
				printf("FREEZING Elevator 2!                            ");
			}

			else if (KeyData == '+' && KeyData1 == '1') {
				iKeyData = 75;
				pipe1.Write(&iKeyData, sizeof(int));
				printf("FAULT FIXED for Elevator 1!                      ");
			}

			else if (KeyData == '+' && KeyData1 == '2') {
				iKeyData = 76;
				pipe1.Write(&iKeyData, sizeof(int));
				printf("FAULT FIXED for Elevator 2!                     ");
			}

			else if (KeyData == 'u'&& ( (KeyData1-'0') >= 0 && (KeyData1-'0') < 10 ) ) {
				iKeyData = 10 + KeyData1 - '0';
				pipe1.Write(&iKeyData, sizeof(int));
				printf("Someone Outside Floor %d wants to go UP.       ", (KeyData1 - '0') );
			}
			else if (KeyData == 'd'&& ( (KeyData1-'0') >= 0 && (KeyData1-'0') < 10 )) {
				iKeyData = 20 + KeyData1 - '0';
				pipe1.Write(&iKeyData, sizeof(int));
				printf("Someone Outside Floor %d wants to go DOWN.     ", (KeyData1 - '0') );
			}
			else if (KeyData == '1'&& ( (KeyData1-'0') >= 0 && (KeyData1-'0') < 10 )) {
				iKeyData = 50 + KeyData1 - '0';
				pipe1.Write(&iKeyData, sizeof(int));
				printf("Someone Inside Elevator 1 Pressed Floor %d.    ", (KeyData1 - '0') );
			}
			else if (KeyData == '2'&& ( (KeyData1-'0') >= 0 && (KeyData1-'0') < 10 )) {
				iKeyData = 60 + KeyData1 - '0';
				pipe1.Write(&iKeyData, sizeof(int));
				printf("Someone Inside Elevator 2 Pressed Floor %d.    ", (KeyData1 - '0') );
			}
			else{
				printf("ERROR: illegible code.                         ");
			}
			fflush(stdout);
			m1->Signal();
			KeyData = '0';
			KeyData1 = '0';
			iKeyData = 0;
		}
		m1->Wait();

		// 9
		MOVE_CURSOR(20,2);
		printf(" |");
		if (Ele1Status.updir[9] == 1 || Ele2Status.updir[9] == 1)
			TEXT_COLOUR(14);
		printf("UP ");
		TEXT_COLOUR(7);
		if (Ele1Status.downdir[9] == 1 || Ele2Status.downdir[9] == 1)
			TEXT_COLOUR(14);
		printf("DOWN");
		TEXT_COLOUR(7);
		printf("| ");

		//8
		MOVE_CURSOR(20,6);
		printf(" |");
		if (Ele1Status.updir[8] == 1 || Ele2Status.updir[8] == 1)
			TEXT_COLOUR(14);
		printf("UP ");
		TEXT_COLOUR(7);
		if (Ele1Status.downdir[8] == 1 || Ele2Status.downdir[8] == 1)
			TEXT_COLOUR(14);
		printf("DOWN");
		TEXT_COLOUR(7);
		printf("| ");

		//7
		MOVE_CURSOR(20,10);
		printf(" |");
		if (Ele1Status.updir[7] == 1 || Ele2Status.updir[7] == 1)
			TEXT_COLOUR(14);
		printf("UP ");
		TEXT_COLOUR(7);
		if (Ele1Status.downdir[7] == 1 || Ele2Status.downdir[7] == 1)
			TEXT_COLOUR(14);
		printf("DOWN");
		TEXT_COLOUR(7);
		printf("| ");

		//6
		MOVE_CURSOR(20,14);
		printf(" |");
		if (Ele1Status.updir[6] == 1 || Ele2Status.updir[6] == 1)
			TEXT_COLOUR(14);
		printf("UP ");
		TEXT_COLOUR(7);
		if (Ele1Status.downdir[6] == 1 || Ele2Status.downdir[6] == 1)
			TEXT_COLOUR(14);
		printf("DOWN");
		TEXT_COLOUR(7);
		printf("| ");

		//5
		MOVE_CURSOR(20,18);
		printf(" |");
		if (Ele1Status.updir[5] == 1 || Ele2Status.updir[5] == 1)
			TEXT_COLOUR(14);
		printf("UP ");
		TEXT_COLOUR(7);
		if (Ele1Status.downdir[5] == 1 || Ele2Status.downdir[5] == 1)
			TEXT_COLOUR(14);
		printf("DOWN");
		TEXT_COLOUR(7);
		printf("| ");

		//4
		MOVE_CURSOR(20,22);
		printf(" |");
		if (Ele1Status.updir[4] == 1 || Ele2Status.updir[4] == 1)
			TEXT_COLOUR(14);
		printf("UP ");
		TEXT_COLOUR(7);
		if (Ele1Status.downdir[4] == 1 || Ele2Status.downdir[4] == 1)
			TEXT_COLOUR(14);
		printf("DOWN");
		TEXT_COLOUR(7);
		printf("| ");

		//3
		MOVE_CURSOR(20,26);
		printf(" |");
		if (Ele1Status.updir[3] == 1 || Ele2Status.updir[3] == 1)
			TEXT_COLOUR(14);
		printf("UP ");
		TEXT_COLOUR(7);
		if (Ele1Status.downdir[3] == 1 || Ele2Status.downdir[3] == 1)
			TEXT_COLOUR(14);
		printf("DOWN");
		TEXT_COLOUR(7);
		printf("| ");

		//2
		MOVE_CURSOR(20,30);
		printf(" |");
		if (Ele1Status.updir[2] == 1 || Ele2Status.updir[2] == 1)
			TEXT_COLOUR(14);
		printf("UP ");
		TEXT_COLOUR(7);
		if (Ele1Status.downdir[2] == 1 || Ele2Status.downdir[2] == 1)
			TEXT_COLOUR(14);
		printf("DOWN");
		TEXT_COLOUR(7);
		printf("| ");

		//1
		MOVE_CURSOR(20,34);
		printf(" |");
		if (Ele1Status.updir[1] == 1 || Ele2Status.updir[1] == 1)
			TEXT_COLOUR(14);
		printf("UP ");
		TEXT_COLOUR(7);
		if (Ele1Status.downdir[1] == 1 || Ele2Status.downdir[1] == 1)
			TEXT_COLOUR(14);
		printf("DOWN");
		TEXT_COLOUR(7);
		printf("| ");

		//0
		MOVE_CURSOR(20,38);
		printf(" |");
		if (Ele1Status.updir[0] == 1 || Ele2Status.updir[0] == 1)
			TEXT_COLOUR(14);
		printf("UP ");
		TEXT_COLOUR(7);
		if (Ele1Status.downdir[0] == 1 || Ele2Status.downdir[0] == 1)
			TEXT_COLOUR(14);
		printf("DOWN");
		TEXT_COLOUR(7);
		printf("| ");

		fflush(stdout);
		MOVE_CURSOR(0,50);
		fflush(stdout);
		m1->Signal();

		Sleep(1);
	}
	t1.WaitForThread() ;
	t2.WaitForThread() ;

	printf("\nExiting... (waiting on other processes)");
	r2.Wait();
	return 0;
}