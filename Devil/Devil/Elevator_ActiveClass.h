#include "I-O.h"

/******************************
*** ACTIVECLASS - ELEVATOR ****
******************************/

class Elevator : public ActiveClass
{
private:
	int myNumber;
	int rendezvousCount;
	elevatorStatus myStatus;
public:
	Elevator(){}
	Elevator(int _myNumber, int _rendezvousCount) {
		myNumber = _myNumber;
		rendezvousCount = _rendezvousCount;
	}
	~Elevator(){}

private:
	int Fxn1(void *threaddata1) {
		//Testing.
		for (int i = 0; i < 3; i++) {
			//printf("this is Fxn1  -> %d <- \n", myNumber) ;
			Sleep(5);
		}
		return 0;
	}

	int Fxn2(void *threaddata1) {
		//Testing.
		for (int i = 0; i < 3; i++) {
			//printf("this is Fxn2  -> %d <- -> %d <-\n", rand()%100 + 100, myNumber ) ;
			Sleep(5);
		}
		return 0;
	}

	int main(void)
	{
		
		/*-------------------------------------------------------------------------------------------------------------*/
		/* Initializing Rendezvous */
		/*-------------------------------------------------------------------------------------------------------------*/	
		CRendezvous	ELE_r1	("Rendezvous_InitiateActiveClasses", rendezvousCount); //rendezvous point for all programs to start together.
		CRendezvous ELE_r2	("Rendezvous_TerminateClasses", rendezvousCount);
		
		/*-------------------------------------------------------------------------------------------------------------*/
		/* Initializing Mutexes */
		/*-------------------------------------------------------------------------------------------------------------*/	
		CMutex* ELE_mutex = new CMutex ("Mutex_Console");
		//Requires a delete.
		//Why use new.?
		/*-------------------------------------------------------------------------------------------------------------*/
		/* Initializing Datapools */
		/*-------------------------------------------------------------------------------------------------------------*/	
		string ELE_DP_String = "Datapool_" + to_string(static_cast<long long>(myNumber));

		CDataPool ELE_ElevatorStruct_DataPool(ELE_DP_String, sizeof(struct elevatorStatus));
		elevatorStatus* ELE_ElevatorStruct_Local = (struct elevatorStatus*)(ELE_ElevatorStruct_DataPool.LinkDataPool());

		/*-------------------------------------------------------------------------------------------------------------*/
		/* Initializing Pipes */
		/*-------------------------------------------------------------------------------------------------------------*/	

		CPipe ELE_Pipe_EleToDisp = "Pipe_DispatcherToEle_" + to_string(static_cast<long long>(myNumber));
		CPipe ELE_Pipe_DisptoEle = "Pipe_EleToDispatcher_" + to_string(static_cast<long long>(myNumber));

		/*-------------------------------------------------------------------------------------------------------------*/
		/* Initializing Semaphores - Elevators act as producers for the datapool*/
		/*-------------------------------------------------------------------------------------------------------------*/	

		string ELE_Semaphore_String_Producer = "PS_DP_" + to_string(static_cast<long long>(myNumber));
		string ELE_Semaphore_String_Consumer = "CS_DP_" + to_string(static_cast<long long>(myNumber));
		CSemaphore ps1 ( ELE_Semaphore_String_Producer, 0, 1 );
		CSemaphore cs1 ( ELE_Semaphore_String_Producer, 1, 1 );

		/*-------------------------------------------------------------------------------------------------------------*/
		/*  */
		/*-------------------------------------------------------------------------------------------------------------*/	
		//printf("%d rendEle %d \n", rendezvousCount,myNumber);
		ELE_r1.Wait();
		ClassThread<Elevator> Thread1 (this, &Elevator::Fxn1, ACTIVE, NULL) ;
		ClassThread<Elevator> Thread2 (this, &Elevator::Fxn2, ACTIVE, NULL) ;
		ClassThread<Elevator> Thread3 (this, &Elevator::Fxn1, ACTIVE, NULL) ;
		Thread1.WaitForThread();
		Thread2.WaitForThread();
		Thread3.WaitForThread();

		ELE_r2.Wait();
		return 0;
	}
} ;