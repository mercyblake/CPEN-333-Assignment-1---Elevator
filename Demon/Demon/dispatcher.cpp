//for use in case any self-created fxns needed.
#include <stdio.h>
#include "dispatcher.h"

CSemaphore		ps2("PS2", 0, 1) ;    // semaphore with initial value 0 and max value 1. What is the purpose of initial value?
CSemaphore		cs2("CS2", 1, 1) ;    // semaphore with initial value 1 and max value 1
CSemaphore		ps4("PS4", 0, 1) ;    // semaphore with initial value 0 and max value 1
CSemaphore		cs4("CS4", 1, 1) ;    // semaphore with initial value 1 and max value 1

myDpData ele[eleCount];

int flag = 1;


void MsgStart() {
	//Sleep(3000);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("\n\n\n\n\n\n\n\nSystem IO online.");
	printf("\nThis building has: 2 elevators.");
	//Sleep(2000);
	//printf("\nWhich elevator(s) would you like to activate?");
	//printf("\n(Press the number for the one you would like to activate, or press 'a' for activating all, 'n' will exit):\n");
	printf("\nPress any key to start...");
	getch();
	printf("\n");
}

UINT __stdcall DispatcherToElevator1(void *args)			
{
	CDataPool	dp1("Ele1", sizeof(struct myDpData)) ;
	struct		myDpData *Ele1DP = (struct myDpData *)(dp1.LinkDataPool());

	while(flag) {	
		if (ps2.Read()>0) {
			ps2.Wait();

			ele[0] = *Ele1DP;

			cs2.Signal();
		}
		Sleep(5);
	}
	return 0 ;									
}

UINT __stdcall DispatcherToElevator2(void *args)			
{
	CDataPool	dp2("Ele2", sizeof(struct myDpData)) ;
	struct		myDpData *Ele2DP = (struct myDpData *)(dp2.LinkDataPool()) ;

	while(flag) {
		if (ps4.Read()>0) {
			ps4.Wait();

			ele[1] = *Ele2DP;

			cs4.Signal();
		}
		Sleep(5);
	}
	return 0 ;									
}
