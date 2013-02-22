#include "str.h"
#include "inout.h"

int main(int argc, char *argv[])
{ 
	char* _runCmd = "D:\\Run.cmd";
	char* _readMessage = ReadFromFile(_runCmd);

	WriteToFile(_runCmd, "I am a new message");
}

