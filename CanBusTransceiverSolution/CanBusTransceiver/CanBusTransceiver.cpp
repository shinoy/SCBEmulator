// CanBusTransceiver.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "CanBusTransceiver.h"

using namespace std;

/* The defination of TransChannel members*/

//cotr
CanTransNS::TransChannel::TransChannel(DWORD type, DWORD index, DWORD channel) :deviceType(type), deviceIndex(index), channelNum(channel)
{
	//set the initialize parameters

	initConfig.AccCode = 0x80000008;
	initConfig.AccMask = 0xffffffff;
	initConfig.Filter = 0;
	initConfig.Mode = 0;
	initConfig.Timing0 = 0x00;
	initConfig.Timing1 = 0x1C;
	initConfig.Reserved = 0;
}//end of TransChannel_1

CanTransNS::TransChannel::TransChannel(DWORD type, DWORD index, DWORD channel, VCI_INIT_CONFIG config) :deviceType(type), deviceIndex(index), channelNum(channel)
{
	initConfig = config;
}


//Channel Initialize Function
void CanTransNS::TransChannel::InitChannel()
{
	switch (VCI_InitCAN(deviceType, deviceIndex, channelNum, &initConfig))
	{
	case 1:
		return;
		break;
	case 0:
		throw CanException(CAN_INIT_ERROR);
		break;
	case -1:
		throw CanException(CAN_NO_DEVICE);
		break;
	default:
		throw CanException(CAN_UNKNOWN);
		break;
	}
}//end of InitChannel

//Channel Start Function
void CanTransNS::TransChannel::StartChannel()
{
	switch (VCI_StartCAN(deviceType,deviceIndex,channelNum))
	{
	case 1:
		return;
		break;
	case 0:
		throw CanException(CAN_START_ERROR);
		break;
	case -1:
		throw CanException(CAN_NO_DEVICE);
		break;
	default:
		throw CanException(CAN_UNKNOWN);
		break;
	}
}

//Channel Function
void CanTransNS::TransChannel::SendOnChannel(PVCI_CAN_OBJ frameBuffer, DWORD frameLength)
{
	//cout << "send now" << endl;
	LONG sendFrameCount = 0;
	LONG FrameCount = frameLength;
	PVCI_CAN_OBJ frameStart = frameBuffer;

	//sending frames, will resend if not send completely
	while ( sendFrameCount < FrameCount && sendFrameCount >= 0)
	{
		FrameCount -= sendFrameCount;
		frameStart += sendFrameCount;
		if (frameStart != frameBuffer)
		{
			Sleep(5);
		}
		sendFrameCount = VCI_Transmit(deviceType, deviceIndex, channelNum, frameStart, FrameCount);
	}
	if (sendFrameCount < 0)
	{
		throw CanException(CAN_SEND_ERROR);
	}
	else
	{
		return;
	}
}

//Get frame count in receiving buffer
DWORD CanTransNS::TransChannel::ReadFrameCount()
{
	return VCI_GetReceiveNum(deviceType, deviceIndex, channelNum);
}

//Channel Receive Function
DWORD CanTransNS::TransChannel::ReadOnChannel(PVCI_CAN_OBJ readBuffer, DWORD frameCount)
{

	PVCI_CAN_OBJ bufferStart = readBuffer;
	LONG actualCount = 0;

	//will continue to read and fill buffer until there're no frames in controller receive buffer and return the actual frame numbers 
		actualCount = VCI_Receive(deviceType, deviceIndex, channelNum, bufferStart,frameCount,20);
		//cout << "now receive " << (int)actualCount << endl;

	if (actualCount < 0)
	{
		throw CanException(CAN_RECEIVE_ERROR);
	}
	else
	{
		return actualCount;
	}
}



//DWORD CanTransNS::TransChannel::ReadOnChannel(PVCI_CAN_OBJ readBuffer, DWORD frameCount)
//{
//
//	PVCI_CAN_OBJ bufferStart = readBuffer;
//    LONG remainCount = frameCount;
//	LONG actualCount = 1;
//
//	//will continue to read and fill buffer until there're no frames in controller receive buffer and return the actual frame numbers 
//	while (remainCount != 0 && actualCount > 0)
//	{
//		if (remainCount != frameCount)
//		{
//			Sleep(10);
//		}
//		actualCount = VCI_Receive(deviceType, deviceIndex, channelNum, bufferStart,remainCount,20);
//		remainCount -= actualCount;
//		cout << "now receive " << (int)actualCount << endl;
//		bufferStart += (frameCount - remainCount);
//	}
//
//	if (actualCount < 0)
//	{
//		throw CanException(CAN_RECEIVE_ERROR);
//	}
//	else
//	{
//		return frameCount-remainCount;
//	}
//}

/*******************************************************************************************/

/* The defination of CanTransceiver members*/

//ctor
CanTransNS::CanTransceiver::CanTransceiver()
{
	this->channel1 = NULL;
	this->channel2 = NULL;
}

//Connect to CAN board
void CanTransNS::CanTransceiver::OpenDevice()
{
	LONG openResult = VCI_OpenDevice(deviceType, deviceIndex, 0);
	switch (openResult)
	{
	case 1:
		return;
		break;
	case 0:
		throw CanException(CAN_OPEN_ERROR);
		break;
	case -1:
		throw CanException(CAN_NO_DEVICE);
		break;
	default:
		throw CanException(CAN_UNKNOWN);
		break;
	}
}

//Disconnect with CAN board
void CanTransNS::CanTransceiver::CloseDevice()
{
	LONG closeResult = VCI_CloseDevice(deviceType, deviceIndex);
	switch (closeResult)
	{
	case 1:
		return;
		break;
	case 0:
		throw CanException(CAN_CLOSE_ERROR);
		break;
	case -1:
		throw CanException(CAN_NO_DEVICE);
		break;
	default:
		throw CanException(CAN_UNKNOWN);
		break;
	}

	delete channel1;
	channel1 = NULL;
	delete channel2;
	channel2 = NULL;
}

//Get the can channel,only 0 and 1 could be used
void CanTransNS::CanTransceiver::GetChannel(DWORD channelNum)
{
	switch (channelNum)
	{
	case 0:
		channel1 = new TransChannel(deviceType, deviceIndex, channelNum);
		channel1->InitChannel();
		Sleep(100);
		channel1->StartChannel();
		Sleep(100);
		break;
	case 1:
		channel2 = new TransChannel(deviceType, deviceIndex, channelNum);
		channel2->InitChannel();
		Sleep(100);
		channel2->StartChannel();
		Sleep(100);
		break;
	default:
		throw CanException(CAN_NO_DEVICE);
		break;
	}
}

//Get the can channel,only 0 and 1 could be used,custom can channel parameters could be used
void CanTransNS::CanTransceiver::GetChannel(DWORD channelNum, VCI_INIT_CONFIG config)
{
	switch (channelNum)
	{
	case 0:
		channel1 = new TransChannel(deviceType, deviceIndex, channelNum,config);
		channel1->InitChannel();
		Sleep(100);
		channel1->StartChannel();
		break;
	case 1:
		channel2 = new TransChannel(deviceType, deviceIndex, channelNum,config);
		channel2->InitChannel();
		Sleep(100);
		channel2->StartChannel();
		break;
	default:
		throw CanException(CAN_NO_DEVICE);
		break;
	}
}


