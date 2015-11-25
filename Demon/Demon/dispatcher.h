#ifndef __DISPATCHER__
#define __DISPATCHER__

#include "elevator.h"
#include "..\..\..\..\RTexample\rt.h"

void MsgStart();

UINT __stdcall DispatcherToElevator1(void *args);
UINT __stdcall DispatcherToElevator2(void *args);

static const int eleCount =	2;

extern int			flag;
extern myDpData		ele[eleCount];

extern CSemaphore	ps2;    // USED BY: DISPATCHER, ELE1
extern CSemaphore	cs2;    // USED BY: DISPATCHER, ELE1
extern CSemaphore	ps4;    // USED BY: DISPATCHER, ELE2
extern CSemaphore	cs4;    // USED BY: DISPATCHER, ELE2

//INIT: SEMAPHORES - NEEDS REWRITING (generic)


//need gatekeeper for the dp.

#endif;