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
	/* Initializing Pipelines to IO */
	/*--------------------------------------------------------------------------------------------------------------*/	
	CPipe	DISP_pipe1	("Pipe_IOToDispatcher", 1024) ;
	CPipe	DISP_pipe2	("Pipe_DispatcherToIO", 1024) ;
	
	/*--------------------------------------------------------------------------------------------------------------*/
	/* Initializing Mutex */
	/*--------------------------------------------------------------------------------------------------------------*/	
	CMutex* DISP_mutex = new CMutex ("Mutex_General"); // mutex to give writing access.
	
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

	CPipe** DISP_PipeToEle = new CPipe*[numElevator]; // !!! if only pointer, not double pointer, it says no constructor exists for CPipe. why?
	CPipe** DISP_PipeFromEle = new CPipe*[numElevator];

	for (int i = 0; i < numElevator ; i++) {
		DISP_PipeToEle[i] = new CPipe(DISP_PipeStringArray_DispToEle_Full[i]); //!!! why is new CPipe here fine? and not above?
		DISP_PipeFromEle[i] = new CPipe(DISP_PipeStringArray_EleToDisp_Full[i]); 
	}

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Dispatcher Logic */
	/*--------------------------------------------------------------------------------------------------------------*/	

	printf("%d rendC Dispatcher\n", rendezvousCount);
	DISP_r1.Wait();

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Terminate Arrays */
	/*--------------------------------------------------------------------------------------------------------------*/	
	delete[] DISP_DPStringArray_Full; // does this require multiple deletes?
	delete[] DISP_ElevatorStructArray_Local;
	delete[] DISP_ElevatorStructArray_DataPool;
	
	DISP_r2.Wait();
	return 0;
	}
} ;
