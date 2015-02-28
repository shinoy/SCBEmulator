// ConsoleApplication2.cpp : main project file.

#include "stdafx.h"


using namespace System;
using namespace CanBusChannelNS;
//using namespace SCBProtocalWrapper;
//using namespace PositionerSimulationNS;

int main(array<System::String ^> ^args)
{
	CanBusChannelCls ^ bus = gcnew CanBusChannelCls;
	bus->Connect();

	/*OTCXYZControllerCLS ^ otcxyz = gcnew OTCXYZControllerCLS(bus);

	otcxyz->PowerUp();*/
	
}
