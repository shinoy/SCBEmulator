// ProtocalTester.cpp : main project file.

#include "stdafx.h"

using namespace System;
using namespace SCBProtocalWrapper;

int main(array<System::String ^> ^args)
{
    
	OTCXYZHBMsgPS ^ heartBT = gcnew OTCXYZHBMsgPS;
	heartBT->X_POSITION = -1000;
	Console::WriteLine("");
    return 0;
}
