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
	/*--------------------------------------------------------------------------------------------------------------*/	
	CPipe	DISP_pipe1	("Pipe_IOToDispatcher", 1024) ;
	CPipe	DISP_pipe2	("Pipe_DispatcherToIO", 1024) ;
	
	CSemaphore PS_pipe1 ("PS_pipe_1", 0 , 1);
	CSemaphore CS_pipe1 ("CS_pipe_1", 1 , 1);
	CSemaphore PS_pipe2 ("PS_pipe_2", 0 , 1);
	CSemaphore CS_pipe2 ("CS_pipe_2", 1 , 1);

	int pipe1data = 0;
	int pipe2data = 0;

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Initializing Mutex */
	/*--------------------------------------------------------------------------------------------------------------*/	
	CMutex* DISP_mutex = new CMutex ("Mutex_Monitor"); // mutex to give writing access.
	
	/*--------------------------------------------------------------------------------------------------------------*/
	/* Initializing Datapools */
	/* - Concatonates string Datapool_ with numElevator. EG: if numElevator is 3 from user input, then Datapool_0,	*/
	/* - Datapool_1, Datapool_2 will be created.                                                                    */
	/*--------------------------------------------------------------------------------------------------------------*/	
	string DISP_DPStringBegin = "Datapool_";
	string* DISP_DPStringArray_Full = new string[numElevator];
	for (int i = 0; i < numElevator ; i++) {
		DISP_DPStringArray_Full[i] = DISP_DPStringBegin + to_string(static_cast<long long>(i));
	}

	CDataPool** DISP_ElevatorStructArray_DataPool = new CDataPool*[numElevator]; // !!! know why you used double pointer? or even a pointer?
	elevatorStatus** DISP_ElevatorStructArray_Local = new elevatorStatus*[numElevator]; //!!! what is this used for?
	for (int i = 0; i < numElevator ; i++) {
		DISP_ElevatorStructArray_DataPool[i] = new CDataPool(DISP_DPStringArray_Full[i], sizeof(struct elevatorStatus));
		DISP_ElevatorStructArray_Local[i] = (elevatorStatus*)(DISP_ElevatorStructArray_DataPool[i]->LinkDataPool()); 
	}

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Initializing Pipelines to Elevators */
	/* - Same concatonation logic as for Datapools, but there are two pipes per elevator, one sends data from       */
	/* - dispatcher to elevator, and the other receives.                                                            */
	/*                                                                                                              */
	/*--------------------------------------------------------------------------------------------------------------*/	

	string DISP_PipeStringBegin_DispToEle = "Pipe_DispatcherToEle_"; // !!!is it a bad idea to start initializations after performing loops?
	string DISP_PipeStringBegin_EleToDisp = "Pipe_Ele_";
	string DISP_PipeStringEnd_EleToDisp = "_ToDispatcher";
	string* DISP_PipeStringArray_DispToEle_Full = new string[numElevator];
	string* DISP_PipeStringArray_EleToDisp_Full = new string[numElevator];

	for (int i = 0; i < numElevator ; i++) {
		DISP_PipeStringArray_DispToEle_Full[i] = DISP_PipeStringBegin_DispToEle + to_string(static_cast<long long>(i));
		DISP_PipeStringArray_EleToDisp_Full[i] = DISP_PipeStringBegin_EleToDisp + to_string(static_cast<long long>(i)) + DISP_PipeStringEnd_EleToDisp;
	}

	CPipe** DISP_PipeDispToEle = new CPipe*[numElevator]; // !!! if only pointer, not double pointer, it says no constructor exists for CPipe. why?
	CPipe** DISP_PipeEleToDisp = new CPipe*[numElevator];

	for (int i = 0; i < numElevator ; i++) {
		DISP_PipeDispToEle[i] = new CPipe(DISP_PipeStringArray_DispToEle_Full[i]); //!!! why is new CPipe here fine? and not above?
		DISP_PipeEleToDisp[i] = new CPipe(DISP_PipeStringArray_EleToDisp_Full[i]); 
	}

	/*-------------------------------------------------------------------------------------------------------------*/
	/* Initializing Datapool Semaphores - Dispatcher act as a consumer for all the datapools*/
	/*-------------------------------------------------------------------------------------------------------------*/	

	string DISP_PS_Begin = "PS_DP_";
	string DISP_CS_Begin = "CS_DP_";
	string* DISP_PSStringArray_Full = new string[numElevator];
	string* DISP_CSStringArray_Full = new string[numElevator];

	for (int i = 0; i < numElevator ; i++) {
		DISP_PSStringArray_Full[i] = DISP_PS_Begin + to_string(static_cast<long long>(i));
		DISP_CSStringArray_Full[i] = DISP_CS_Begin + to_string(static_cast<long long>(i));
	}

	CSemaphore** DISP_PSArray_DP = new CSemaphore*[numElevator];
	CSemaphore** DISP_CSArray_DP = new CSemaphore*[numElevator];

	for (int i = 0; i < numElevator ; i++) {
		DISP_PSArray_DP[i] = new CSemaphore(DISP_PSStringArray_Full[i], 0 , 1);
		DISP_CSArray_DP[i] = new CSemaphore(DISP_CSStringArray_Full[i], 0 , 1);
	}
	
	/*--------------------------------------------------------------------------------------------------------------*/
	/* Dispatcher Logic */
	/*--------------------------------------------------------------------------------------------------------------*/	

	printf("%d rendC Dispatcher\n", rendezvousCount);
	DISP_r1.Wait();

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Terminate Arrays */
	/*--------------------------------------------------------------------------------------------------------------*/	
	
	// Datapool arrays
	//-----------------------
	delete[] DISP_DPStringArray_Full; // does this require multiple deletes?
	delete[] DISP_ElevatorStructArray_Local;
	delete[] DISP_ElevatorStructArray_DataPool;

	// Pipe arrays
	//-----------------------
	delete[] DISP_PipeStringArray_DispToEle_Full;
	delete[] DISP_PipeStringArray_EleToDisp_Full;
	delete[] DISP_PipeDispToEle;
	delete[] DISP_PipeEleToDisp;

	// Semaphore arrays
	//-----------------------
	delete[] DISP_PSStringArray_Full;
	delete[] DISP_CSStringArray_Full;
	delete[] DISP_PSArray_DP;
	delete[] DISP_CSArray_DP;
	
	DISP_r2.Wait();
	return 0;
	}
} ;
