#include "..\..\Devil\rt.h"
#include <iostream>

int main() {
	int numElevator;
	cin >> numElevator;
	string IO_PipeStringBegin_IOToMU = "Pipe_IOToMU_"; // !!!is it a bad idea to start initializations after performing loops?
	string IO_PipeStringBegin_MUToIO = "Pipe_MU_";
	string IO_PipeStringEnd_MUToIO = "_ToIO";
	string* IO_PipeStringArray_IOToMU_Full = new string[numElevator];
	string* IO_PipeStringArray_MUToIO_Full = new string[numElevator];

	for (int i = 0; i < numElevator ; i++) {
		IO_PipeStringArray_IOToMU_Full[i] = IO_PipeStringBegin_IOToMU + to_string(static_cast<long long>(i));
		IO_PipeStringArray_MUToIO_Full[i] = IO_PipeStringBegin_MUToIO + to_string(static_cast<long long>(i)) + IO_PipeStringEnd_MUToIO;
	}

	CPipe** IO_Pipe_IOToMU = new CPipe*[numElevator]; // !!! if only pointer, not double pointer, it says no constructor exists for CPipe. why?
	CPipe** IO_Pipe_MUToIO = new CPipe*[numElevator];

	for (int i = 0; i < numElevator ; i++) {
		IO_Pipe_IOToMU[i] = new CPipe(IO_PipeStringArray_IOToMU_Full[i]); //!!! why is new CPipe here fine? 
		IO_Pipe_MUToIO[i] = new CPipe(IO_PipeStringArray_MUToIO_Full[i]); 
	}

	delete[] IO_PipeStringArray_IOToMU_Full;
	delete[] IO_PipeStringArray_MUToIO_Full;
	delete[] IO_Pipe_IOToMU;
	delete[] IO_Pipe_MUToIO;
	return 0;
}