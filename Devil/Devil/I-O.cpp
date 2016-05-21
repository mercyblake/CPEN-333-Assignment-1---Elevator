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
#include "DisplayUpdate_ActiveClass.h"


 


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
	string str_numElevator = to_string(static_cast<long long>(numElevator));


	/*--------------------------------------------------------------------------------------------------------------*/
	/* Initializing Rendezvous */
	/*--------------------------------------------------------------------------------------------------------------*/	
	int rendezvousCount = numElevator + 2;
	int rendezvousCount2 = numElevator + 1;

	CRendezvous	IO_r1	("Rendezvous_InitiateActiveClasses_DispEle", rendezvousCount); 
	CRendezvous IO_r2	("Rendezvous_TerminateClasses_DispEle", rendezvousCount);
	
	
	CRendezvous IO_r1_1 ("Rendezvous_InitiateActiveClasses_DU", rendezvousCount2);
	CRendezvous IO_r2_1 ("Rendezvous_TerminateClasses_DU", rendezvousCount2);

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Initializing Pipelines, and data storage for pipeline data*/
	/* ( originally also used pipeline semaphores, but with our one-directional 1-to-1 method, where consumption    */
	/*   rate greatly exceeds production, there is no need.)														*/
	/*--------------------------------------------------------------------------------------------------------------*/
	CPipe	pipe1	("PipeIOToDispatcher", 1024) ; //defaulting to int.
	CPipe	pipe2	("PipeDispatcherToIO", 1024) ;
	

	string IO_Pipe_StringBegin_IOToDU = "Pipe_IOToDU_"; // !!!is it a bad idea to start initializations after performing loops?
	string IO_Pipe_StringBegin_DUToIO = "Pipe_DUToIO_";
	string* IO_Pipe_StringArray_Full_IOToDU = new string[numElevator];
	string* IO_Pipe_StringArray_Full_DUToIO = new string[numElevator];

	for (int i = 0; i < numElevator ; i++) {
		IO_Pipe_StringArray_Full_IOToDU[i] = IO_Pipe_StringBegin_IOToDU + to_string(static_cast<long long>(i));
		IO_Pipe_StringArray_Full_DUToIO[i] = IO_Pipe_StringBegin_DUToIO + to_string(static_cast<long long>(i));
	}


	CPipe** IO_Pipe_IOToDU = new CPipe*[numElevator]; // !!! if only pointer, not double pointer, it says no constructor exists for CPipe. why?
	CPipe** IO_Pipe_DUToIO = new CPipe*[numElevator];

	for (int i = 0; i < numElevator ; i++) {
		IO_Pipe_IOToDU[i] = new CPipe(IO_Pipe_StringArray_Full_IOToDU[i], 1024); //!!! why is new CPipe here fine? 
		IO_Pipe_DUToIO[i] = new CPipe(IO_Pipe_StringArray_Full_DUToIO[i], 1024); 
	}

	delete[] IO_Pipe_StringArray_Full_IOToDU;
	delete[] IO_Pipe_StringArray_Full_DUToIO;
	delete[] IO_Pipe_IOToDU;
	delete[] IO_Pipe_DUToIO;


	int pipe1data = 0;
	int pipe2data = 0;

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Initializing Mutexes */
	/*--------------------------------------------------------------------------------------------------------------*/	
	CMutex* IO_mutex = new CMutex ("Mutex_Console");

	
	/*--------------------------------------------------------------------------------------------------------------*/
	/* Instantiating ActiveClasses */
	/*--------------------------------------------------------------------------------------------------------------*/
	
	//Initializing Elevator
	Elevator ** ele;
	ele = new Elevator*[numElevator];
	
	for (int i = 0; i < numElevator; i++) {
		ele[i] = new Elevator(i,rendezvousCount);
		//cout << "Creating elevator " << i << "\t" << ele[i] << "\t" << *ele << "\t" << ele << endl;
	}

	
	for (int i = 0; i < numElevator; i++) {
		ele[i]->Resume();
	}
	//Initializing Display(console) Updates

	DisplayUpdate ** display = new DisplayUpdate*[numElevator];
	
	for (int i = 0; i < numElevator; i++) {
		display[i] = new DisplayUpdate(i,rendezvousCount2);
		//cout << "Creating display " << i << "\t" << display[i] << "\t" << *display << "\t" << display << endl;
	}
	
	for (int i = 0; i < numElevator; i++) {
		display[i]->Resume();
	}
	//Initializing Dispatcher
	Dispatcher disp(numElevator,rendezvousCount);
	disp.Resume();

	

	Sleep(10);
	
	/*--------------------------------------------------------------------------------------------------------------*/
	/* Start Rendezvous with main thread, elevators and dispatcher ActiveClasses */
	/*                                                                                                              */
	/*--------------------------------------------------------------------------------------------------------------*/

	IO_r1.Wait();
	

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Elevator Grid Print */
	/* draw a grid around |-|                                                                                                              */
	/*--------------------------------------------------------------------------------------------------------------*/

	IO_mutex -> Wait();
	MOVE_CURSOR(0,0);
	for (int i = 0; i < 30; i++)
		cout << "                                                                               " << endl;
	MOVE_CURSOR(1,0);
	cout << "=========";
	for (int i = 0; i < numElevator; i++)
			cout << "=======";
	for (int i = 0; i < 10; i++) {
		MOVE_CURSOR(1,(i*4)+2);
		cout << "Floor " << 9 - i << flush;

		MOVE_CURSOR(1,(i*4)+4);
		cout << "=========";
		for (int j = 0; j < numElevator; j++)
			cout << "=======";
		cout << flush;

		for (int j = 0; j < numElevator; j++) {
			MOVE_CURSOR(j*7+10,i*4+1);
			cout <<"||   ||"<<flush;
			MOVE_CURSOR(j*7+10,i*4+2);
			cout <<"||   ||"<<flush;
			MOVE_CURSOR(j*7+10,i*4+3);
			cout <<"||   ||"<<flush;
		}
	}
	MOVE_CURSOR(1, 41);
	cout << "Elevator" << flush;
	for (int i = 1; i <= numElevator; i++) {
		MOVE_CURSOR(i*7+6, 41);
		cout << i;
	}
	
	MOVE_CURSOR(1,44);
	cout <<"  Inside"<< endl << " Buttons";
	for (int i = 0; i < numElevator; i++) {
		MOVE_CURSOR(i*7+10,42);
		cout << " _____ ";
		MOVE_CURSOR(i*7+10,43);
		cout << "|7 8 9|";
		MOVE_CURSOR(i*7+10,44);
		cout << "|4 5 6|";
		MOVE_CURSOR(i*7+10,45);
		cout << "|1 2 3|";
		MOVE_CURSOR(i*7+10,46);
		cout << "|  0  |";
		MOVE_CURSOR(i*7+10,47);
		cout << "'-----'";
	}
	MOVE_CURSOR(0,49);
	cout << "===============================================================================" << flush;

	fflush(stdout);
	IO_mutex -> Signal(); 

	/*--------------------------------------------------------------------------------------------------------------*/
	/* Start Rendezvous with main thread and DU ActiveClasses */
	/*                                                                                                              */
	/*--------------------------------------------------------------------------------------------------------------*/

	IO_r1_1.Wait();

	/*--------------------------------------------------------------------------------------------------------------*/
	/* User Input Logic */
	/* - Goal is to send all keyboard instructions to the dispatcher to decide what to do                           */
	/* - IO has control over lines 50 to 67 of the console.															*/
	/*--------------------------------------------------------------------------------------------------------------*/

	bool flag = TRUE;
	char KeyData;
	UINT state = 1;
	int IO_UserData = 0;
	int TimerTwoSeconds = 2000;
	//strings
	string str_MainInstructions = 
		string(	
		  "\r\n MAIN MENU" // 50
	   )+
		"\r\n\n To control OUTSIDE of the elevators, first select direction 'u' or 'd', " //51, 52
		+ 
		  "\r\n and then select your current floor number (0 - 9). Examples: u4; d3." //53
		+ 
		  "\r\n To control elevator from INSIDE, select the elevator (1 - " //54
		+ str_numElevator +                                              ")," 
		  "\r\n and then desired level." //55
		+
		  "\r\n Example: To go to select elevator 1 to go to floor 4, input 14." //56
		  ;

	string str_OutsideElevator_DirectionSelect_Restate = 
		string(
		  "\r\n YOU ARE CURRENTLY: OUTSIDE ELEVATORS" //50
		)+
		"\r\n\n You want to head " //51, 52 
			;
	string str_OutsideElevator_FloorSelect_Prompt = 
		"\r\n\n Select your current floor by pressing a number between 0 - 9: "; //54
	string str_OutsideElevator_FloorSelect_Restate = 
		  "\r\n Your current floor is: "; //55 
	
	string str_InsideElevator_SelectElevator_Restate = 
		string(
		  "\r\n YOU ARE CURRENTLY: INSIDE AN ELEVATOR" //50
		)+
		"\r\n\n You are inside elevator "; //51, 52
	string str_InsideElevator_FloorSelect_Prompt = 
		"\r\n\n Select the desired floor you wish to travel towards: "; //54
	string str_InsideElevator_FloorSelect_Restate = 
		  "\r\n You want to head to floor "; //55 

	string str_Admin_MainInstructions = 
		string(  
		  "\r\n You may press 'x' at any time to enter ADMIN mode, which allows you to:" // 65
		  )+
		  "\r\n     1. pause/continue an elevator" // 66
		  +
		  "\r\n     2. terminate the program."; //67
	string str_Admin_Prompt = 
		string(  
		  "\r\n ADMIN MODE " //50
		)+
		"\r\n\n 1. Press 'q' to terminate the program, or" //51, 52
		+
		  "\r\n 2. Press a number between 1 and " 
		+ str_numElevator +                    " to freeze/resume that elevator." //53
		+
		"\r\n\nPress any other key to exit ADMIN mode without giving any instruction." //54,55
		; 
	string str_Admin_InitiatingProgramTermination = 
		  "\r\n Initiating Program Termination...";
	string str_Admin_HaltingOrResuming = 
		  "\r\n You are halting/resuming elevator: "; //52
	//string str_Admin_Resuming = 
	//	  "\r\nYou are resuming elevator: "; //52 
	
	string str_RecentInstructions = 
		  "\r\n Most recent instruction inputs: "; //58
	string str_WrongCommand_Line1 = 
		  "\r\n You have inputed an incorrect command: " //50
		  ;
	string str_WrongCommand_Line2 =
		  "\r\n You are now returned to the main menu." //51
		  ;
	string str_ElevatorDoesNotExist = 
		  "\r\n Elevator Does Not Exist. Select an elevator between 1 - " 
		 + str_numElevator +                                           ".";

	//string str_UserMainMenu = "\r\nPress:\n\r\t 1: Select Elevator.\n\r 9: Terminate program.";
	//string str_RequestElevator = "\r\nYou have a total of " + str_numElevator + " elevators. Type a number between 1 and " + str_numElevator +'.';




	while( flag ) {

		switch (state) {

		case IO_state_PrintMainMenu: //prints main menu and enters it

			IO_mutex->Wait();

			MOVE_CURSOR(0,50);
			for (int i = 0; i < 11; i++)
				cout << "                                                                               " << endl;
			cout <<     "                                                                               " << flush;
			fflush(stdout);

			MOVE_CURSOR(0,50);
			cout << str_MainInstructions << endl;
			fflush(stdout);

			MOVE_CURSOR(0,65);
			cout <<str_Admin_MainInstructions << endl;
			fflush(stdout);

			MOVE_CURSOR(1,58);
			IO_mutex->Signal();
			
			state = IO_state_MainMenu;
			break;

		case IO_state_Outside_PrintFirstPrompt: //asking for floor number, and printing received direction.
			
			IO_mutex->Wait();

			MOVE_CURSOR(0,50);
			for (int i = 0; i < 11; i++)
				cout << "                                                                               " << endl;
			cout <<     "                                                                               " << flush;
			fflush(stdout);

			MOVE_CURSOR(0,50);
			cout << str_OutsideElevator_DirectionSelect_Restate << flush;

			if (IO_UserData % 1000 == 100)
				cout << "UP";
			else if (IO_UserData % 1000 == 200)
				cout << "DOWN";
			else 
				cout << "ERROR (outside print first prompt)";

			cout << str_OutsideElevator_FloorSelect_Prompt << endl;
			fflush(stdout);

			//MOVE_CURSOR(0,65);
			//cout << str_Admin_MainInstructions << endl;
			//fflush(stdout);
			
			IO_mutex->Signal();

			state = IO_state_Outside_WaitForSecondButton;
			break;

		case IO_state_Outside_WaitForSecondButton: //waiting for user input of current floor number.

			if (TEST_FOR_KEYBOARD() != 0) {
				KeyData = getch() ;					// read next character from keyboard

				if (KeyData == 'x') {
					IO_UserData = 1000;
					state = IO_state_Admin_PrintFirstPrompt;
				}

				else if (unsigned int((KeyData) -'0') < 10) {
					IO_UserData += int((KeyData) -'0');
					state = IO_state_Outside_PrintSecondPrompt;
				}

				else {
					IO_UserData = 0;
					state = IO_state_PrintReceivedWrongCommand;
				}

			}
			break;

		case IO_state_Outside_PrintSecondPrompt: //restates the received current floor number.

			IO_mutex->Wait();
			MOVE_CURSOR(1,55);
			cout << str_OutsideElevator_FloorSelect_Restate << IO_UserData % 10 << flush;
			fflush(stdout);

			//MOVE_CURSOR(0,65);
			//cout << str_Admin_MainInstructions << endl;
			//fflush(stdout);

			IO_mutex->Signal();

			state = IO_state_PipeToDispatcher;
			break;

		case IO_state_Inside_PrintFirstPrompt: //restates the received elevator number, ask for target floor number.
			
			IO_mutex->Wait();

			MOVE_CURSOR(0,50);
			for (int i = 0; i < 11; i++)
				cout << "                                                                               " << endl;
			cout <<     "                                                                               " << flush;
			fflush(stdout);

			MOVE_CURSOR(1,50);
			cout << str_InsideElevator_SelectElevator_Restate << IO_UserData % 100 << str_InsideElevator_FloorSelect_Prompt << flush;
			fflush(stdout);

			//MOVE_CURSOR(0,65);
			//cout <<str_Admin_MainInstructions << endl;
			//fflush(stdout);

			IO_mutex->Signal();
			
			state = IO_state_Inside_WaitForSecondButton;
			break;

		case IO_state_Inside_WaitForSecondButton: //waiting for user input of desired floor number.

			if (TEST_FOR_KEYBOARD() != 0) {
				KeyData = getch() ;					// read next character from keyboard

				if (KeyData == 'x') {
					IO_UserData = 1000;
					state = IO_state_Admin_PrintFirstPrompt;
				}

				else if (unsigned int((KeyData) -'0') < 10) {
					IO_UserData += int((KeyData) -'0');
					state = IO_state_Inside_PrintSecondPrompt;
				}

				else {
					IO_UserData = 0;
					state = IO_state_PrintReceivedWrongCommand;
				}

			}
			break;

		case IO_state_Inside_PrintSecondPrompt: //restates the received desired floor number

			IO_mutex->Wait();
			MOVE_CURSOR(1,55);
			cout << str_InsideElevator_FloorSelect_Restate << IO_UserData % 10 << flush;
			fflush(stdout);

			//MOVE_CURSOR(0,65);
			//cout << str_Admin_MainInstructions << endl;
			//fflush(stdout);

			IO_mutex->Signal();

			state = IO_state_PipeToDispatcher;
			break;

		case IO_state_DisplayTwoSeconds: //displays the completed instruction for two seconds before returning to main menu

			if (TEST_FOR_KEYBOARD() != 0)  {
				TimerTwoSeconds = 2000;
				goto MainMenuSelections;
			}
			
			else if (TimerTwoSeconds == 0) {
				TimerTwoSeconds = 2000;
				state = IO_state_PrintMainMenu;
			}
			
			else 
				TimerTwoSeconds--;
			
			break;

		case IO_state_Admin_PrintFirstPrompt: //prints first prompt of ADMIN
			
			IO_mutex->Wait();

			MOVE_CURSOR(0,50);
			for (int i = 0; i < 17; i++)
				cout << "                                                                               " << endl;
			cout <<     "                                                                               " << flush;
			fflush(stdout);

			MOVE_CURSOR(1,50);
			cout << str_Admin_Prompt << flush;
			fflush(stdout);

			IO_mutex->Signal();
			
			state = IO_state_Admin_WaitForSecondButton;
			
			break;

		case IO_state_Admin_WaitForSecondButton:
			
			if (TEST_FOR_KEYBOARD() != 0) {
				KeyData = getch() ;					// read next character from keyboard

				if (KeyData == 'q') {
					IO_UserData += 999;
					flag = FALSE;
				}

				else if (unsigned int((KeyData) -'1') < 9) {
					if (int((KeyData) -'0') > numElevator) {
						IO_UserData = 0;
						state = IO_state_PrintElevatorDoesNotExist;
					}
					else {
						IO_UserData += (int((KeyData) -'0')) * 10;
						state = IO_state_Admin_PrintSecondPrompt;
					}
				}

				else {
					IO_UserData = 0;
					state = IO_state_PrintReceivedWrongCommand;
				}

			}
			break;

		case IO_state_Admin_PrintSecondPrompt:

			IO_mutex -> Wait();

			MOVE_CURSOR(0,52);
			for (int i = 0; i < 5; i++)
				cout << "                                                                               " << endl;
			cout <<     "                                                                               " << flush;
			fflush(stdout);

			MOVE_CURSOR(1,52);
			cout << str_Admin_HaltingOrResuming << IO_UserData % 10 << flush;
			fflush(stdout);

			IO_mutex -> Signal();
			state = IO_state_PipeToDispatcher;
			break;

		case IO_state_PrintReceivedWrongCommand: //tells the user that a wrong command has been received.

			IO_mutex -> Wait();

			MOVE_CURSOR(0,50);
			for (int i = 0; i < 11; i++)
				cout << "                                                                               " << endl;
			cout <<     "                                                                               " << flush;
			fflush(stdout);

			MOVE_CURSOR(1,50);
			cout << str_WrongCommand_Line1 << KeyData << str_WrongCommand_Line2 << flush;
			fflush(stdout);

			IO_mutex -> Signal();
			state = IO_state_DisplayTwoSeconds;

			break;

		case IO_state_PipeToDispatcher:

			/*pipe1.Write(&IO_UserData, sizeof(int));*/
			state = IO_state_DisplayTwoSeconds;

			break;

		case IO_state_PrintElevatorDoesNotExist:

			IO_mutex -> Wait();

			cout << str_ElevatorDoesNotExist << flush;

			IO_mutex -> Signal();
			state = IO_state_DisplayTwoSeconds;

			break;

		default: //waits for first character

			if (TEST_FOR_KEYBOARD() != 0) {
				MainMenuSelections:
				KeyData = getch() ;					// read next character from keyboard

				if (KeyData == 'x') {
					IO_UserData = 1000;
					state = IO_state_Admin_PrintFirstPrompt;
				}

				else if (KeyData == 'u' || KeyData == 'd') {										
					if (KeyData == 'u')
						IO_UserData = 100; //up = 100;
					else
						IO_UserData = 200; //down = 200;

					state = IO_state_Outside_PrintFirstPrompt;
				}

				else if (unsigned int((KeyData) -'1') < 9) {

					if (int((KeyData) -'0') > numElevator) {
						IO_UserData = 0;
						state = IO_state_PrintElevatorDoesNotExist;
					}
					else {
						IO_UserData = (int((KeyData) -'0')) * 10;
						state = IO_state_Inside_PrintFirstPrompt;
					}

				}

				else {
					IO_UserData = 0;
					state = IO_state_PrintReceivedWrongCommand;
				}

			}
			break;
		}

		
		/*pipe1.Write(&IO_UserData, sizeof(int));*/
		Sleep(1);
	}		

	IO_mutex -> Wait();

	MOVE_CURSOR(0,52);
	for (int i = 0; i < 17; i++)
		cout << "                                                                               " << endl;
	cout <<     "                                                                               " << flush;
	fflush(stdout);

	MOVE_CURSOR(0,52);
	cout << str_Admin_InitiatingProgramTermination << flush;
	fflush(stdout);

	IO_mutex -> Signal();
	/*pipe1.Write(&IO_UserData, sizeof(int));*/


	/*--------------------------------------------------------------------------------------------------------------*/
	/* End Rendezvous with main thread and ALL ActiveClasses */
	/*                                                                                                              */
	/*--------------------------------------------------------------------------------------------------------------*/
	IO_r2.Wait();
	IO_r2_1.Wait();


	/*--------------------------------------------------------------------------------------------------------------*/
	/* Closing Threads */
	/*                                                                                                              */
	/*--------------------------------------------------------------------------------------------------------------*/
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
