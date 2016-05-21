#include "I-O.h"

/************************************
*** ACTIVECLASS - DISPLAY UPDATE ****
************************************/

class DisplayUpdate : public ActiveClass
{
private:
	int myNumber;
	int rendezvousCount;
	int flag;
	elevatorStatus elevatorStatus_Local ;
public:
	DisplayUpdate(){}
	DisplayUpdate(int _myNumber, int _rendezvousCount) {
		myNumber = _myNumber;
		rendezvousCount = _rendezvousCount;
	}
	~DisplayUpdate(){}

private:

	int main(void)
	{
		
		/*-------------------------------------------------------------------------------------------------------------*/
		/* Initializing Rendezvous */
		/*-------------------------------------------------------------------------------------------------------------*/	
		CRendezvous	DU_r1	("Rendezvous_InitiateActiveClasses_DU", rendezvousCount); //rendezvous point for all programs to start together.
		CRendezvous DU_r2	("Rendezvous_TerminateClasses_DU", rendezvousCount);

		/*-------------------------------------------------------------------------------------------------------------*/
		/* Initializing Display Mutex */
		/*-------------------------------------------------------------------------------------------------------------*/	
		CMutex* DU_mutex = new CMutex ("Mutex_Console");
		
		/*-------------------------------------------------------------------------------------------------------------*/
		/* Initializing Datapools */
		/* datapool is shared with dispatcher and elevator ACs, NOT IO. */
		/*-------------------------------------------------------------------------------------------------------------*/	
		string DU_DPString = "Datapool_" + to_string(static_cast<long long>(myNumber));

		CDataPool DU_ElevatorStatus_DataPool(DU_DPString, sizeof(struct elevatorStatus));
		elevatorStatus* DU_elevatorStatus_DPLocal = (struct elevatorStatus*)(DU_ElevatorStatus_DataPool.LinkDataPool());

		/*-------------------------------------------------------------------------------------------------------------*/
		/* Initializing Semaphores for Datapools - Producer and Consumer */
		/* datapool is shared with dispatcher and elevator ACs, NOT IO. */
		/*-------------------------------------------------------------------------------------------------------------*/			
		string DU_Semaphore_String_Producer = "PS_DP_" + to_string(static_cast<long long>(myNumber));
		string DU_Semaphore_String_Consumer = "CS_DP_" + to_string(static_cast<long long>(myNumber));
		CSemaphore ps1 ( DU_Semaphore_String_Producer, 0, 1 );
		CSemaphore cs1 ( DU_Semaphore_String_Producer, 1, 1 );
		
		/*-------------------------------------------------------------------------------------------------------------*/
		/* Initializing Pipelines with IO*/
		/* Used currently to receive IO termination commands */
		/*-------------------------------------------------------------------------------------------------------------*/			

		string DU_Pipe_String_Full_IOToDU = "Pipe_IOToDU_" + to_string(static_cast<long long>(myNumber));
		string DU_Pipe_String_Full_DUToIO = "Pipe_DUToIO_" + to_string(static_cast<long long>(myNumber));

		CPipe DU_Pipe_IOToDU (DU_Pipe_String_Full_IOToDU, 1024);
		CPipe DU_Pipe_DUToIO (DU_Pipe_String_Full_DUToIO, 1024);

		int pipe1data = 0;
		int pipe2data = 0;

		//Rendezvous
		DU_r1.Wait();

		/*-------------------------------------------------------------------------------------------------------------*/
		/* Display Update Logic */
		/*-------------------------------------------------------------------------------------------------------------*/	

		flag = 1;

		while(flag) {	
			if (ps1.Read()>0) {
				ps1.Wait();

				elevatorStatus_Local = *DU_elevatorStatus_DPLocal;

				cs1.Signal();
			}

			if (DU_Pipe_IOToDU.TestForData() >= sizeof(pipe1data)) {
				DU_Pipe_IOToDU.Read(&pipe1data, sizeof(pipe1data)) ;
				if(1) { // write a command in IO to initiate flag change
					flag = 0;
				}
			}



			//FLOOR 9
			DU_mutex->Wait();

			MOVE_CURSOR(myNumber*7+12,1);

			if (elevatorStatus_Local.floor == 9) 
				cout << "._." << flush;
			else
				cout << "   " << flush;

			MOVE_CURSOR(myNumber*7+12,2);

			if (elevatorStatus_Local.floor == 9) {
				cout << "|" << flush;
				if (elevatorStatus_Local.door == OPEN) 
					cout << "O" << flush;
				else
					cout << "-" << flush;
				cout << "|" << flush;
			}
			else
				cout << "   " << flush;

			MOVE_CURSOR(myNumber*7+12,3);

			if (elevatorStatus_Local.floor == 9) 
				cout << "^-^" << flush;
			else
				cout << "   " << flush;

			//FLOOR 8
			MOVE_CURSOR(myNumber*7+12,5);

			if (elevatorStatus_Local.floor == 8) 
				cout << "._." << flush;
			else
				cout << "   " << flush;

			MOVE_CURSOR(myNumber*7+12,6);
			if (elevatorStatus_Local.floor == 8) {
				cout << "|" << flush;
				if (elevatorStatus_Local.door == OPEN) 
					cout << "O" << flush;
				else
					cout << "-" << flush;
				cout << "|" << flush;
			}
			else
				cout << "   " << flush;

			MOVE_CURSOR(myNumber*7+12,7);
			if (elevatorStatus_Local.floor == 8) 
				cout << "^-^" << flush;
			else
				cout << "   " << flush;


			//FLOOR 7
			MOVE_CURSOR(myNumber*7+12,9);

			if (elevatorStatus_Local.floor == 7) 
				cout << "._." << flush;
			else
				cout << "   " << flush;
			MOVE_CURSOR(myNumber*7+12,10);

			if (elevatorStatus_Local.floor == 7) {
				cout << "|" << flush;
				if (elevatorStatus_Local.door == OPEN) 
					cout << "O" << flush;
				else
					cout << "-" << flush;
				cout << "|" << flush;
			}
			else
				cout << "   " << flush;

			MOVE_CURSOR(myNumber*7+12,11);

			if (elevatorStatus_Local.floor == 7) 
				cout << "^-^" << flush;
			else
				cout << "   " << flush;


			//FLOOR 6
			MOVE_CURSOR(myNumber*7+12,13);

			if (elevatorStatus_Local.floor == 6) 
				cout << "._." << flush;
			else
				cout << "   " << flush;

			MOVE_CURSOR(myNumber*7+12,14);

			if (elevatorStatus_Local.floor == 6) {
				cout << "|" << flush;
				if (elevatorStatus_Local.door == OPEN) 
					cout << "O" << flush;
				else
					cout << "-" << flush;
				cout << "|" << flush;
			}
			else
				cout << "   " << flush;

			MOVE_CURSOR(myNumber*7+12,15);

			if (elevatorStatus_Local.floor == 6) 
				cout << "^-^" << flush;
			else
				cout << "   " << flush;

			//FLOOR 5
			MOVE_CURSOR(myNumber*7+12,17);

			if (elevatorStatus_Local.floor == 5) 
				cout << "._." << flush;
			else
				cout << "   " << flush;

			MOVE_CURSOR(myNumber*7+12,18);

			if (elevatorStatus_Local.floor == 5) {
				cout << "|" << flush;
				if (elevatorStatus_Local.door == OPEN) 
					cout << "O" << flush;
				else
					cout << "-" << flush;
				cout << "|" << flush;
			}
			else
				cout << "   " << flush;

			MOVE_CURSOR(myNumber*7+12,19);

			if (elevatorStatus_Local.floor == 5) 
				cout << "^-^" << flush;
			else
				cout << "   " << flush;


			//FLOOR 4
			MOVE_CURSOR(myNumber*7+12,21);

			if (elevatorStatus_Local.floor == 4) 
				cout << "._." << flush;
			else
				cout << "   " << flush;

			MOVE_CURSOR(myNumber*7+12,22);

			if (elevatorStatus_Local.floor == 4) {
				cout << "|" << flush;
				if (elevatorStatus_Local.door == OPEN) 
					cout << "O" << flush;
				else
					cout << "-" << flush;
				cout << "|" << flush;
			}
			else
				cout << "   " << flush;

			MOVE_CURSOR(myNumber*7+12,23);

			if (elevatorStatus_Local.floor == 4) 
				cout << "^-^" << flush;
			else
				cout << "   " << flush;
			

			//FLOOR 3
			MOVE_CURSOR(myNumber*7+12,25);

			if (elevatorStatus_Local.floor == 3) 
				cout << "._." << flush;
			else
				cout << "   " << flush;

			MOVE_CURSOR(myNumber*7+12,26);

			if (elevatorStatus_Local.floor == 3) {
				cout << "|" << flush;
				if (elevatorStatus_Local.door == OPEN) 
					cout << "O" << flush;
				else
					cout << "-" << flush;
				cout << "|" << flush;
			}
			else
				cout << "   " << flush;

			MOVE_CURSOR(myNumber*7+12,27);

			if (elevatorStatus_Local.floor == 3) 
				cout << "^-^" << flush;
			else
				cout << "   " << flush;

			//FLOOR 2
			MOVE_CURSOR(myNumber*7+12,29);

			if (elevatorStatus_Local.floor == 2) 
				cout << "._." << flush;
			else
				cout << "   " << flush;

			MOVE_CURSOR(myNumber*7+12,30);

			if (elevatorStatus_Local.floor == 2) {
				cout << "|" << flush;
				if (elevatorStatus_Local.door == OPEN) 
					cout << "O" << flush;
				else
					cout << "-" << flush;
				cout << "|" << flush;
			}
			else
				cout << "   " << flush;

			MOVE_CURSOR(myNumber*7+12,31);

			if (elevatorStatus_Local.floor == 2) 
				cout << "^-^" << flush;
			else
				cout << "   " << flush;


			//FLOOR 1
			MOVE_CURSOR(myNumber*7+12,33);

			if (elevatorStatus_Local.floor == 1) 
				cout << "._." << flush;
			else
				cout << "   " << flush;


			MOVE_CURSOR(myNumber*7+12,34);

			if (elevatorStatus_Local.floor == 1) {
				cout << "|" << flush;
				if (elevatorStatus_Local.door == OPEN) 
					cout << "O" << flush;
				else
					cout << "-" << flush;
				cout << "|" << flush;
			}
			else
				cout << "   " << flush;


			MOVE_CURSOR(myNumber*7+12,35);

			if (elevatorStatus_Local.floor == 1) 
				cout << "^-^" << flush;
			else
				cout << "   " << flush;


			//FLOOR 0
			MOVE_CURSOR(myNumber*7+12,37);

			if (elevatorStatus_Local.floor == 0) 
				cout << "._." << flush;
			else
				cout << "   " << flush;

			MOVE_CURSOR(myNumber*7+12,38);

			if (elevatorStatus_Local.floor == 0) {
				cout << "|" << flush;
				if (elevatorStatus_Local.door == OPEN) 
					cout << "O" << flush;
				else
					cout << "-" << flush;
				cout << "|" << flush;
			}
			else
				cout << "   " << flush;

			MOVE_CURSOR(myNumber*7+12,39);

			if (elevatorStatus_Local.floor == 0) 
				cout << "^-^" << flush;
			else
				cout << "   " << flush;



			//MOVE_CURSOR(0,41);
			//printf("Buttons in Ele1");
			//MOVE_CURSOR(7,42);
			//for (int i = 7; i < 10; i++) {
			//	if (elevatorStatus_Local.lights[i] == 1) {
			//		TEXT_COLOUR(14);
			//	}
			//	printf("[%d] ", i);
			//	TEXT_COLOUR(7);
			//}
			//MOVE_CURSOR(7,43);
			//for (int i = 4; i < 7; i++) {
			//	if (elevatorStatus_Local.lights[i] == 1) {
			//		TEXT_COLOUR(14);
			//	}
			//	printf("[%d] ", i);
			//	TEXT_COLOUR(7);
			//}
			//MOVE_CURSOR(7,44);
			//for (int i = 1; i < 4; i++) {
			//	if (elevatorStatus_Local.lights[i] == 1) {
			//		TEXT_COLOUR(14);
			//	}
			//	printf("[%d] ", i);
			//	TEXT_COLOUR(7);
			//}
			//MOVE_CURSOR(11,45);
			//if (elevatorStatus_Local.lights[0] == 1) {
			//	TEXT_COLOUR(14);
			//}
			//printf("[0]");
			//TEXT_COLOUR(7);
			////printf("Ele1 Read value for Floor = %d                                \n", elevatorStatus_Local.floor) ;
			////printf("Ele1 Read value for Usage = %d                                \n", elevatorStatus_Local.usage);
			////printf("Ele1 Read value for Door = %d                                 \n", elevatorStatus_Local.direction) ; 
			////printf("Ele1 Read value for Direction = %d                            \n", elevatorStatus_Local.direction) ; 
			////printf("Ele1 Read values for floor array = ") ;
			////for(int i=0; i < 10; i ++) {
			////	printf("%d ", elevatorStatus_Local.floors[i]) ;
			////}
			//fflush(stdout);
			DU_mutex->Signal();

			SLEEP(10);
	}




		DU_r2.Wait();
		return 0;
	}
} ;