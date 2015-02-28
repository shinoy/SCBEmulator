// CanTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ControlCAN.h"
#include "CanBusTransceiver.h"

using namespace CanTransNS;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	CanTransceiver board;
//	board.CloseDevice();
	board.OpenDevice();
	board.GetChannel(0);
	board.GetChannel(1);
	TransChannel * channel1 = board.channel1;
	TransChannel * channel2 = board.channel2;

	VCI_CAN_OBJ frames[2000];
	for (int i = 0; i < 2000; i++)
	{
		frames[i].Data[0] = 0x32;
		frames[i].Data[2] = 0xff;
		frames[i].DataLen = 3;
		frames[i].ExternFlag = 0;
		frames[i].ID = 0x310;
		frames[i].RemoteFlag = 0;
	}
	for (int i = 0; i < 1; i++)
	{
		try
		{

			channel1->SendOnChannel(frames, 2000);
		}
		catch (CanException ex)
		{
			cout << ex.errorCode << endl;
		}
	}
	Sleep(500);
	VCI_CAN_OBJ receiveBuffer[2000];

	try
	{
		cout << "channel2 get " << channel2->ReadFrameCount() << " frames" << endl;

		cout << (int)channel2->ReadOnChannel(receiveBuffer, 2002) << endl;
		cout << (int)receiveBuffer[0].DataLen << endl;
		cout << (int)receiveBuffer[1].Data[2] << endl;
		Sleep(100);
	/*	cout << "channel2 get " << channel2->ReadFrameCount() << " frames" << endl;
		cout << (int)channel2->ReadOnChannel(receiveBuffer, 500) << endl;
		cout << "channel2 get " << channel2->ReadFrameCount() << " frames" << endl;*/
	}
	catch (CanException ex)
	{
		cout << "catched" << endl;
		board.CloseDevice();
	}
	
	board.CloseDevice();
	
	
	system("PAUSE");
	return 0;
}

