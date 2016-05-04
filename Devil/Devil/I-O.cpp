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

#include "Dispatcher_ActiveClass.h"
#include "Elevator_ActiveClass.h"
#include "MonitorUpdate_ActiveClass.h"


 



/******************
*** MAIN - I/O ****
******************/

int main() {

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Request User Input for Total Number of Elevators */
	/*--------------------------------------------------------------------------------------------------------------*/		
	int numElevator = 0;
	
	cout << "Input the number of elevators you want: " << endl;
	cin >> numElevator;
	cout << "user entered: " << numElevator << endl;
	Elevator ** ele;
	ele = new Elevator*[numElevator];
	

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Initializing Rendezvous */
	/*--------------------------------------------------------------------------------------------------------------*/	
	int rendezvousCount = numElevator + 2;

	CRendezvous	IO_r1	("Rendezvous_InitiateActiveClasses", rendezvousCount); 
	CRendezvous IO_r2	("Rendezvous_TerminateClasses", rendezvousCount);

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Initializing Pipelines, pipeline semaphores, and data storage for pipeline data*/
	/*--------------------------------------------------------------------------------------------------------------*/
	CPipe	pipe1	("PipeIOToDispatcher", 1024) ; //defaulting to int.
	CPipe	pipe2	("PipeDispatcherToIO", 1024) ;
	
	CSemaphore PS_pipe1 ("PS_pipe_1", 0 , 1);
	CSemaphore CS_pipe1 ("CS_pipe_1", 1 , 1);
	CSemaphore PS_pipe2 ("PS_pipe_2", 0 , 1);
	CSemaphore CS_pipe2 ("CS_pipe_2", 1 , 1);

	int pipe1data = 0;
	int pipe2data = 0;

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Initializing Mutexes */
	/*--------------------------------------------------------------------------------------------------------------*/	
	CMutex* IO_mutex = new CMutex ("Mutex_Monitor");

	///*--------------------------------------------------------------------------------------------------------------*/
	///* Initializing Datapools */
	///*--------------------------------------------------------------------------------------------------------------*/	
	//string IO_DPStringBegin = "Datapool_";
	//string* IO_DPStringArray_Full = new string[numElevator];
	//for (int i = 0; i < numElevator ; i++) {
	//	IO_DPStringArray_Full[i] = IO_DPStringBegin + to_string(static_cast<long long>(i));
	//}

	//CDataPool** IO_ElevatorStructArray_DataPool = new CDataPool*[numElevator];
	//struct elevatorStatus** IO_ElevatorStructArray_Local = new elevatorStatus*[numElevator];
	//for (int i = 0; i < numElevator ; i++) {
	//	IO_ElevatorStructArray_DataPool[i] = new CDataPool(IO_DPStringArray_Full[i], sizeof(struct elevatorStatus));
	//	IO_ElevatorStructArray_Local[i] = (elevatorStatus*)(IO_ElevatorStructArray_DataPool[i]->LinkDataPool()); 
	//}

	
	/*--------------------------------------------------------------------------------------------------------------*/
	/* Instantiating ActiveClasses */
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
	
	/*--------------------------------------------------------------------------------------------------------------*/
	/* Start Rendezvous with main thread and all ActiveClasses */
	/*                                                                                                              */
	/*--------------------------------------------------------------------------------------------------------------*/

	IO_r1.Wait();
	
	cout << "let's go!" << endl;
	

	/*--------------------------------------------------------------------------------------------------------------*/
	/* IO Logic */
	/* - Goal is to send all keyboard instructions to the dispatcher to decide what to do                           */
	/*--------------------------------------------------------------------------------------------------------------*/

	bool flag = TRUE;
	char KeyData;

	while( flag )
		if (TEST_FOR_KEYBOARD() != 0) {
			KeyData = getch() ;					// read next character from keyboard
		}
	
	/*--------------------------------------------------------------------------------------------------------------*/
	/* Closing Threads */
	/*                                                                                                              */
	/*--------------------------------------------------------------------------------------------------------------*/
	
	//delete[] IO_DPStringArray_Full;
	//delete[] IO_ElevatorStructArray_Local;
	//delete[] IO_ElevatorStructArray_DataPool;

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
