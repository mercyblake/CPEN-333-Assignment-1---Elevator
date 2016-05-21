#include "I-O.h"

/********************************
*** ACTIVECLASS - DISPATCHER ****
********************************/

class Dispatcher : public ActiveClass
{

private:
	int rendezvousCount;
	int numElevator;

public:
	Dispatcher(int _numElevator, int _rendezvousCount)	{
		numElevator = _numElevator;
		rendezvousCount = _rendezvousCount;
	}

	~Dispatcher(){}

private:
	int main(void)
	{
	
	/*-------------------------------------------------------------------------------------------------------------*/
	/* Initializing Rendezvous */
	/*--------------------------------------------------------------------------------------------------------------*/	
	cout << "asdf" << "asdf" << endl;
	CRendezvous	DISP_r1	("Rendezvous_InitiateActiveClasses", rendezvousCount);
	CRendezvous DISP_r2	("Rendezvous_TerminateClasses", rendezvousCount);

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Initializing Pipelines to IO, pipeline semaphores, and storage for data */
	/* Originally pipelines had semaphores, but with our one directional 1-to-1 pipeline method, where consumption  */
	/* rate greatly exceeds production, there is no need.															*/
	/*--------------------------------------------------------------------------------------------------------------*/	
	CPipe	DISP_pipe1	("Pipe_IOToDispatcher", 1024) ;
	CPipe	DISP_pipe2	("Pipe_DispatcherToIO", 1024) ;
	

	int pipe1data = 0;
	int pipe2data = 0;

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Initializing Mutex */
	/*--------------------------------------------------------------------------------------------------------------*/	
	CMutex* DISP_mutex = new CMutex ("Mutex_Console"); // mutex to give writing access.
	//requires a delete.
	/*--------------------------------------------------------------------------------------------------------------*/
	/* Initializing Datapools */
	/* - Concatonates string Datapool_ with numElevator. EG: if numElevator is 3 from user input, then Datapool_0,	*/
	/* - Datapool_1, Datapool_2 will be created.                                                                    */
	/*--------------------------------------------------------------------------------------------------------------*/	
	string DISP_DP_StringBegin = "Datapool_";
	string* DISP_DP_StringArray_Full = new string[numElevator];
	for (int i = 0; i < numElevator ; i++) {
		DISP_DP_StringArray_Full[i] = DISP_DP_StringBegin + to_string(static_cast<long long>(i));
	}

	CDataPool** DISP_ElevatorStructArray_DataPool = new CDataPool*[numElevator]; // !!! know why you used double pointer? or even a pointer?
	elevatorStatus** DISP_ElevatorStructArray_Local = new elevatorStatus*[numElevator]; //!!! what is this used for?
	for (int i = 0; i < numElevator ; i++) {
		DISP_ElevatorStructArray_DataPool[i] = new CDataPool(DISP_DP_StringArray_Full[i], sizeof(struct elevatorStatus));
		DISP_ElevatorStructArray_Local[i] = (elevatorStatus*)(DISP_ElevatorStructArray_DataPool[i]->LinkDataPool()); 
	}

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Initializing Pipelines to Elevators */
	/* - Same concatonation logic as for Datapools, but there are two pipes per elevator, one sends data from       */
	/* - dispatcher to elevator, and the other receives.                                                            */
	/*                                                                                                              */
	/*--------------------------------------------------------------------------------------------------------------*/	

	string DISP_Pipe_StringBegin_DispToEle = "Pipe_DispatcherToEle_"; // !!!is it a bad idea to start initializations after performing loops?
	string DISP_Pipe_StringBegin_EleToDisp = "Pipe_EleToDispatcher_";
	string* DISP_Pipe_StringArray_Full_DispToEle = new string[numElevator];
	string* DISP_Pipe_StringArray_Full_EleToDisp = new string[numElevator];

	for (int i = 0; i < numElevator ; i++) {
		DISP_Pipe_StringArray_Full_DispToEle[i] = DISP_Pipe_StringBegin_DispToEle + to_string(static_cast<long long>(i));
		DISP_Pipe_StringArray_Full_EleToDisp[i] = DISP_Pipe_StringBegin_EleToDisp + to_string(static_cast<long long>(i));
	}

	CPipe** DISP_Pipe_DispToEle = new CPipe*[numElevator]; // !!! if only pointer, not double pointer, it says no constructor exists for CPipe. why?
	CPipe** DISP_Pipe_EleToDisp = new CPipe*[numElevator];

	for (int i = 0; i < numElevator ; i++) {
		DISP_Pipe_DispToEle[i] = new CPipe(DISP_Pipe_StringArray_Full_DispToEle[i]); //!!! why is new CPipe here fine? and not above?
		DISP_Pipe_EleToDisp[i] = new CPipe(DISP_Pipe_StringArray_Full_EleToDisp[i]); 
	}

	/*-------------------------------------------------------------------------------------------------------------*/
	/* Initializing Datapool Semaphores - Dispatcher act as a consumer for all the datapools*/
	/*-------------------------------------------------------------------------------------------------------------*/	

	string DISP_PS_Begin = "PS_DP_";
	string DISP_CS_Begin = "CS_DP_";
	string* DISP_PS_StringArray_Full = new string[numElevator];
	string* DISP_CS_StringArray_Full = new string[numElevator];

	for (int i = 0; i < numElevator ; i++) {
		DISP_PS_StringArray_Full[i] = DISP_PS_Begin + to_string(static_cast<long long>(i));
		DISP_CS_StringArray_Full[i] = DISP_CS_Begin + to_string(static_cast<long long>(i));
	}

	CSemaphore** DISP_PS_Array_DP = new CSemaphore*[numElevator];
	CSemaphore** DISP_CS_Array_DP = new CSemaphore*[numElevator];

	for (int i = 0; i < numElevator ; i++) {
		DISP_PS_Array_DP[i] = new CSemaphore(DISP_PS_StringArray_Full[i], 0 , 1);
		DISP_CS_Array_DP[i] = new CSemaphore(DISP_CS_StringArray_Full[i], 0 , 1);
	}
	
	/*--------------------------------------------------------------------------------------------------------------*/
	/* Dispatcher Logic */
	/*--------------------------------------------------------------------------------------------------------------*/	

	//printf("%d rendC Dispatcher\n", rendezvousCount);
	DISP_r1.Wait();

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Terminate Arrays */
	/*--------------------------------------------------------------------------------------------------------------*/	
	
	// Datapool arrays
	//-----------------------
	delete[] DISP_DP_StringArray_Full; // does this require multiple deletes?
	delete[] DISP_ElevatorStructArray_Local;
	delete[] DISP_ElevatorStructArray_DataPool;

	// Pipe arrays
	//-----------------------
	delete[] DISP_Pipe_StringArray_Full_DispToEle;
	delete[] DISP_Pipe_StringArray_Full_EleToDisp;
	delete[] DISP_Pipe_DispToEle;
	delete[] DISP_Pipe_EleToDisp;

	// Semaphore arrays
	//-----------------------
	delete[] DISP_PS_StringArray_Full;
	delete[] DISP_CS_StringArray_Full;
	delete[] DISP_PS_Array_DP;
	delete[] DISP_CS_Array_DP;
	
	DISP_r2.Wait();
	return 0;
	}
} ;
