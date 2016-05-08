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
	to_string(static_cast<long long>(numElevator));
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
	/* - IO has control over lines 50 to 60 of the console.															*/
	/*--------------------------------------------------------------------------------------------------------------*/

	bool flag = TRUE;
	char KeyData, KeyData1;
	int iKeyData = 0;
	UINT state = 0;
	//strings
	string str_MainInstructions = "\r\nLOCATION: MAIN MENU\r\n\nTo control OUTSIDE of the elevators, first select the direction ('u' or 'd'), and then select your current floor number (0 - 9). \r\nExample: u4, d3 \r\n\nTo control INSIDE of the elevator, select the elevator (1 - " + str_numElevator + "), and then desired level.\r\nExample: To go to select elevator 1 to go to floor 4, input 14.";

	string str_OutsideElevator_DirectionSelect_Restate = "\r\nLOCATION: OUTSIDE ELEVATORS\r\nThe direction you selected is: ";
	string str_OutsideElevator_FloorSelect_Prompt = "\r\nSelect your current floor by pressing a number between 0 - 9.";
	string str_OutsideElevator_FloorSelect_Restate = "\r\nYour current floor is: ";
	
	string str_InsideElevator_SelectElevator_Restate = "\r\nYou are inside elevator: ";
	string str_InsideElevator_FloorSelect_Prompt = "\r\nSelect the desired floor you wish to travel towards.";
	string str_InsideElevator_FloorSelect_Restate = "\r\nYour target floor is: ";

	string str_Admin_MainInstructions = "\r\nYou may press 'x' at any time to enter ADMIN mode, which allows you to: \r\n\tpause/continue an elevator, \r\t\nor terminate the program.";
	string str_Admin_Prompt = "\r\nADMIN MODE \r\n\nPress 'q' to terminate the program, or \r\nEnter a number between 1 and " + str_numElevator + " to freeze/resume that elevator. \r\n\nPress any other key to exit ADMIN mode without giving any instruction."; // don't forget to use str_RequestPressEnter
	string str_Admin_Halting = "\r\nYou are halting elevator: ";
	string str_Admin_Resuming = "\r\nYou are resuming elevator: ";
	
	string str_RecentInstructions = "\r\nMost recent instruction inputs: ";
	string str_WrongCommand = "\r\nYou have inputed an incorrect command. \r\nYou are now returned to the main menu.";
	string str_UserMainMenu = "\r\nPress:\n\r\t 1: Select Elevator.\n\r 9: Terminate program.";
	string str_RequestElevator = "\r\nYou have a total of " + str_numElevator + " elevators. Type a number between 1 and " + str_numElevator +'.';



#define IO_LocationUnknown 0
#define IO_Inside 1
#define IO_Outside 2
#define IO_UP 1
#define IO_DOWN 2
int IO_UserLocation = 0;
int IO_UserData = 0;

	while( flag ) {

		switch (state) {

		case IO_state_PrintMainMenu: //prints main menu and enters it

			IO_mutex->Wait();
			MOVE_CURSOR(0,50);
			cout << str_MainInstructions << endl;
			fflush(stdout);
			MOVE_CURSOR(0,58);
			cout <<str_Admin_MainInstructions << endl;
			fflush(stdout);
			IO_mutex->Signal();
			
			state = IO_state_MainMenu;
			break;

		case IO_state_Outside_PrintFirstPrompt: //asking for floor number, and printing received direction.
			
			IO_mutex->Wait();
			MOVE_CURSOR(0,50);
			cout << str_OutsideElevator_DirectionSelect_Restate;
			if (IO_UserData % 10 == 1) {
				cout << "UP" << endl;
			}
			else if (IO_UserData % 10 == 2) {
				cout << "DOWN" << endl;
			}
			else {
				cout << "ERROR (outside print first prompt)" << endl;
			}
			fflush(stdout);
			MOVE_CURSOR(0,58);
			cout << str_Admin_MainInstructions << endl;
			fflush(stdout);
			IO_mutex->Signal();

			state = IO_state_Outside_WaitForSecondButton;
			break;

		case IO_state_Outside_WaitForSecondButton: //waiting for user input of current floor number.

			if (TEST_FOR_KEYBOARD() != 0) {
				KeyData = getch() ;					// read next character from keyboard

				if (KeyData == 'x') {
					state = IO_state_Admin_PrintFirstPrompt;
				}

				else if (unsigned int((KeyData) -'0') < 10) {
					state = IO_state_Outside_PrintSecondPrompt;
				}

				else {
					state = IO_state_PrintReceivedWrongCommand;
				}

			}
			break;

		case IO_state_Outside_PrintSecondPrompt: //restates the received current floor number.

			IO_mutex->Wait();
			MOVE_CURSOR(0,50);
			cout << str_OutsideElevator_FloorSelect_Restate;
			if (IO_UserData % 10 == 1) {
				cout << "UP" << endl;
			}
			else if (IO_UserData % 10 == 2) {
				cout << "DOWN" << endl;
			}
			else {
				cout << "ERROR (outside print first prompt)" << endl;
			}
			fflush(stdout);
			MOVE_CURSOR(0,58);
			cout << str_Admin_MainInstructions << endl;
			fflush(stdout);
			IO_mutex->Signal();
			break;

		case IO_state_Inside_PrintFirstPrompt: //restates the received elevator number, ask for target floor number.
			break;

		case IO_state_Inside_WaitForSecondButton: //waiting for user input of desired floor number.
			break;

		case IO_state_Inside_PrintSecondPrompt: //restates the received desired floor number
			break;

		case IO_state_DisplayTwoSeconds: //displays the completed instruction for two seconds before returning to main menu
			break;

		case IO_state_Admin_PrintFirstPrompt: //prints first prompt of ADMIN
			break;

		case IO_state_PrintReceivedWrongCommand: //tells the user that a wrong command has been received.
			break;

		case IO_state_PipeToDispatcher:
			break;

		default: //waits for first character

			if (TEST_FOR_KEYBOARD() != 0) {
				KeyData = getch() ;					// read next character from keyboard

				if (KeyData == 'x') {
					state = IO_state_Admin_PrintFirstPrompt;
				}

				else if (KeyData == 'u' || KeyData == 'd') {
					state = IO_state_Outside_PrintFirstPrompt;
				}

				else if (unsigned int((KeyData) -'1') < 9) {
					state = IO_state_Inside_PrintFirstPrompt;
				}

				else {
					state = IO_state_PrintReceivedWrongCommand;
				}

			}
			break;



			while (TEST_FOR_KEYBOARD() == 0) {}
			KeyData1 = getch();
			IO_mutex->Wait();
			MOVE_CURSOR(0,51);
			printf("second character = %c           \n", KeyData1);
			printf("command entered is: %c%c        \n", KeyData, KeyData1);

			//Command Organization.

			if (KeyData == 'e' && KeyData1 == 'e'){
				flag = FALSE;
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
			IO_mutex->Signal();
			KeyData = '0';
			KeyData1 = '0';
			iKeyData = 0;
		}
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
