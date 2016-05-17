#include "I-O.h"

/************************************
*** ACTIVECLASS - MONITOR UPDATE ****
************************************/

class MonitorUpdate : public ActiveClass
{
private:
	int myNumber;
	int rendezvousCount;
	int flag;
	elevatorStatus elevatorStatus_Local ;
public:
	MonitorUpdate(){}
	MonitorUpdate(int _myNumber, int _rendezvousCount) {
		myNumber = _myNumber;
		rendezvousCount = _rendezvousCount;
	}
	~MonitorUpdate(){}

private:

	int main(void)
	{
		
		/*-------------------------------------------------------------------------------------------------------------*/
		/* Initializing Rendezvous */
		/*-------------------------------------------------------------------------------------------------------------*/	
		CRendezvous	MU_r1	("Rendezvous_InitiateActiveClasses", rendezvousCount); //rendezvous point for all programs to start together.
		CRendezvous MU_r2	("Rendezvous_TerminateClasses", rendezvousCount);

		/*-------------------------------------------------------------------------------------------------------------*/
		/* Initializing Monitor Mutex */
		/*-------------------------------------------------------------------------------------------------------------*/	
		CMutex* MU_mutex = new CMutex ("Mutex_Console");
		
		/*-------------------------------------------------------------------------------------------------------------*/
		/* Initializing Datapools */
		/*-------------------------------------------------------------------------------------------------------------*/	
		string MU_DPString = "Datapool_" + to_string(static_cast<long long>(myNumber));

		CDataPool MU_ElevatorStatus_DataPool(MU_DPString, sizeof(struct elevatorStatus));
		elevatorStatus* MU_elevatorStatus_DPLocal = (struct elevatorStatus*)(MU_ElevatorStatus_DataPool.LinkDataPool());

		/*-------------------------------------------------------------------------------------------------------------*/
		/* Initializing Semaphores for Datapools - Consumer */
		/*-------------------------------------------------------------------------------------------------------------*/			
		string MU_SemaphoreString_Producer = "PS_DP_" + to_string(static_cast<long long>(myNumber));
		string MU_SemaphoreString_Consumer = "CS_DP_" + to_string(static_cast<long long>(myNumber));
		CSemaphore ps1 ( MU_SemaphoreString_Producer, 0, 1 );
		CSemaphore cs1 ( MU_SemaphoreString_Producer, 1, 1 );
		

		//Rendezvous
		MU_r1.Wait();

		/*-------------------------------------------------------------------------------------------------------------*/
		/* Monitor Update Logic */
		/*-------------------------------------------------------------------------------------------------------------*/	

		flag = 1;

		while(flag) {	
			if (ps1.Read()>0) {
				ps1.Wait();

				elevatorStatus_Local = *MU_elevatorStatus_DPLocal;

				cs1.Signal();
			}

			//FLOOR 9
			MU_mutex->Wait();

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
			MU_mutex->Signal();

			SLEEP(10);
	}




		MU_r2.Wait();
		return 0;
	}
} ;