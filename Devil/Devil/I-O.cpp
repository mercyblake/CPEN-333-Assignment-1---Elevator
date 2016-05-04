/*                                                                                                              */

/* WORKLOG
 * 2015-12-31
 * Starting I/O coding.
 *
 */

/* I-O.cpp
 *
 *
 */

/* === LIBRARIES === */


#include "I-O.h"

// I-O.h :
// Headers for I/O fxns.
 


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
			printf("this is Fxn1  -> %d <- \n", myNumber) ;
			Sleep(5);
		}
		return 0;
	}

	int Fxn2(void *threaddata1) {
		//Testing.
		for (int i = 0; i < 3; i++) {
			printf("this is Fxn2  -> %d <- -> %d <-\n", rand()%100 + 100, myNumber ) ;
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
		CMutex* ELE_mutex = new CMutex ("Mutex_General");
		
		/*-------------------------------------------------------------------------------------------------------------*/
		/* Initializing Datapools */
		/*-------------------------------------------------------------------------------------------------------------*/	
		string ELE_DPString = "Datapool_" + to_string(static_cast<long long>(myNumber));

		CDataPool ELE_ElevatorStruct_DataPool(ELE_DPString, sizeof(struct elevatorStatus));
		elevatorStatus* ELE_ElevatorStruct_Local = (struct elevatorStatus*)(ELE_ElevatorStruct_DataPool.LinkDataPool());

		/*-------------------------------------------------------------------------------------------------------------*/
		/* Initializing Pipes */
		/*-------------------------------------------------------------------------------------------------------------*/	

		CPipe ELE_Pipe_EleToDisp = "Pipe_DispatcherToEle_" + to_string(static_cast<long long>(myNumber));
		CPipe ELE_Pipe_DisptoEle = "Pipe_Ele_" + to_string(static_cast<long long>(myNumber)) + "_ToDispatcher";

		/*-------------------------------------------------------------------------------------------------------------*/
		/*  */
		/*-------------------------------------------------------------------------------------------------------------*/	
		printf("%d rendEle %d \n", rendezvousCount,myNumber);
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

/******************
*** MAIN - I/O ****
******************/

int main() {
	
	int numElevator = 0;
	
	cout << "Input the number of elevators you want: " << endl;
	cin >> numElevator;
	cout << "user entered: " << numElevator << endl;;
	Elevator ** ele;
	ele = new Elevator*[numElevator];
	int rendezvousCount = numElevator + 2;

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Initializing Rendezvous */
	/*--------------------------------------------------------------------------------------------------------------*/	
	CRendezvous	IO_r1	("Rendezvous_InitiateActiveClasses", rendezvousCount); 
	CRendezvous IO_r2	("Rendezvous_TerminateClasses", rendezvousCount);

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Initializing Pipelines */
	/*--------------------------------------------------------------------------------------------------------------*/
	CPipe	pipe1	("PipeIOToDispatcher", 1024) ;
	CPipe	pipe2	("PipeDispatcherToIO", 1024) ;

	int pipe1data = 0;
	int pipe2data = 0;

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Initializing Mutexes */
	/*--------------------------------------------------------------------------------------------------------------*/	
	CMutex* IO_mutex = new CMutex ("Mutex_General");

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Initializing Datapools */
	/*--------------------------------------------------------------------------------------------------------------*/	
	string IO_DPStringBegin = "Datapool_";
	string* IO_DPStringArray_Full = new string[numElevator];
	for (int i = 0; i < numElevator ; i++) {
		IO_DPStringArray_Full[i] = IO_DPStringBegin + to_string(static_cast<long long>(i));
	}

	CDataPool** IO_ElevatorStructArray_DataPool = new CDataPool*[numElevator];
	struct elevatorStatus** IO_ElevatorStructArray_Local = new elevatorStatus*[numElevator];
	for (int i = 0; i < numElevator ; i++) {
		IO_ElevatorStructArray_DataPool[i] = new CDataPool(IO_DPStringArray_Full[i], sizeof(struct elevatorStatus));
		IO_ElevatorStructArray_Local[i] = (elevatorStatus*)(IO_ElevatorStructArray_DataPool[i]->LinkDataPool()); 
	}

	
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! gotta do smae thing for pipes

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Instantiating Classes */
	/*--------------------------------------------------------------------------------------------------------------*/
	for (int i = 0; i < numElevator; i++) {
		ele[i] = new Elevator(i,rendezvousCount);
		cout << "Creating elevator " << i << "\t" << ele[i] << "\t" << *ele << "\t" << ele << endl;
	}

	for (int i = 0; i < numElevator; i++) {
		ele[i]->Resume();
	}

	Dispatcher disp(numElevator,rendezvousCount);
	disp.Resume();

	Sleep(10);
	
	IO_r1.Wait();
	
	cout << "let's go!" << endl;
	

	/*--------------------------------------------------------------------------------------------------------------*/
	/* IO Logic */
	/* - Goal is to send all keyboard instructions to the dispatcher to decide what to do                           */
	/*--------------------------------------------------------------------------------------------------------------*/
	bool flag = false;
	char KeyData;

	while( flag )
		if (TEST_FOR_KEYBOARD() != 0) {
			KeyData = getch() ;					// read next character from keyboard

	
	/*--------------------------------------------------------------------------------------------------------------*/
	/* Closing Threads */
	/*                                                                                                              */
	/*--------------------------------------------------------------------------------------------------------------*/
	
	delete[] IO_DPStringArray_Full;
	delete[] IO_ElevatorStructArray_Local;
	delete[] IO_ElevatorStructArray_DataPool;

	IO_r2.Wait();

	for (int k = 0; k < numElevator; k++) {
		ele[k]->WaitForThread();
	}
	disp.WaitForThread();
	delete ele;

	cout << "Terminated correctly!" << endl;


	//double ** var = new double*[numElevator];
	//for (int i = 0; i < numElevator; i++) {
	//	
	//	var[i]= new double; //0x025f5e40
	//	*var[i]= i + 100;
	//	cout << *var[i] << "\t" << var[i] << "\t" << *var << "\t" << var << endl;
	//	//var[1]= new int; //
	//	//var[2]= new int; //0x00e35df0
	//}
	//delete var;

	//int * ptr = new int[numElevator];
	//for (int m = 100; m < numElevator + 100; m++) {
	//	*ptr = m;
	//	ptr++;
	//}
	//for (int n = 0; n < numElevator; n++) {
	//	printf("\n%d \n",*(--ptr));
	//}

	//int *ptr2 = new int[10];

	return 0;
}
