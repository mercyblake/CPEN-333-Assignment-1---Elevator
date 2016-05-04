#include "I-O.h"

/************************************
*** ACTIVECLASS - MONITOR UPDATE ****
************************************/

class MonitorUpdate : public ActiveClass
{
private:
	int myNumber;
	elevatorStatus myStatus;
public:
	MonitorUpdate(){}
	MonitorUpdate(int _myNumber) {
		myNumber = _myNumber;
	}
	~MonitorUpdate(){}

private:

	int main(void)
	{
		
		
		/*-------------------------------------------------------------------------------------------------------------*/
		/* Initializing Monitor Mutex */
		/*-------------------------------------------------------------------------------------------------------------*/	
		CMutex* MU_mutex = new CMutex ("Mutex_Monitor");
		
		/*-------------------------------------------------------------------------------------------------------------*/
		/* Initializing Datapools */
		/*-------------------------------------------------------------------------------------------------------------*/	
		string MU_DPString = "Datapool_" + to_string(static_cast<long long>(myNumber));

		CDataPool MU_ElevatorStruct_DataPool(MU_DPString, sizeof(struct elevatorStatus));
		elevatorStatus* MU_ElevatorStruct_Local = (struct elevatorStatus*)(MU_ElevatorStruct_DataPool.LinkDataPool());

		/*-------------------------------------------------------------------------------------------------------------*/
		/* Initializing Semaphores for Datapools - Consumer */
		/*-------------------------------------------------------------------------------------------------------------*/			
		string MU_SemaphoreString_Producer = "PS_DP_" + to_string(static_cast<long long>(myNumber));
		string MU_SemaphoreString_Consumer = "CS_DP_" + to_string(static_cast<long long>(myNumber));
		CSemaphore ps1 ( MU_SemaphoreString_Producer, 0, 1 );
		CSemaphore cs1 ( MU_SemaphoreString_Producer, 1, 1 );

		/*-------------------------------------------------------------------------------------------------------------*/
		/* Monitor Update Logic */
		/*-------------------------------------------------------------------------------------------------------------*/	
		return 0;
	}
} ;