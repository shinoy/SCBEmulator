// CanBusTransceiver.h : Defines the Can Bus Transceiver class
//

#ifdef DLL_EXPORT
#define DLL_FUNC_API _declspec(dllexport)
#else
#define DLL_FUNC_API _declspec(dllimport)
#endif

#ifndef _CBUS_TRANS_
#define _CBUS_TRANS_
#include "ControlCAN.h"
#include <Windows.h>
#include <exception>
#include <string>

using namespace std;

namespace CanTransNS
{
	enum CanErrorType
	{
		CAN_OPEN_ERROR,
		CAN_CLOSE_ERROR,
		CAN_INIT_ERROR,
		CAN_START_ERROR,
		CAN_SEND_ERROR,
		CAN_RECEIVE_ERROR,
		CAN_NO_DEVICE,
		CAN_UNKNOWN
	};

	//Can communication exception definition
	class DLL_FUNC_API CanException : public exception
	{
	public:
		CanErrorType errorCode;

		CanException(const char* const &message, CanErrorType error) :exception(message)
		{
			errorCode = error;
		}
		CanException(CanErrorType error)
		{
			errorCode = error;
		}

		
	};


	//Can channel class definition
	class DLL_FUNC_API TransChannel
	{
	

	public:
		const DWORD channelNum;
		TransChannel(DWORD type, DWORD index, DWORD channel);
		TransChannel(DWORD type, DWORD index, DWORD channel, VCI_INIT_CONFIG config);
		void InitChannel();
		void StartChannel();
		void SendOnChannel(PVCI_CAN_OBJ frameBuffer, DWORD frameLength);
	    DWORD  ReadOnChannel(PVCI_CAN_OBJ frameBuffer, DWORD frameCount);
		DWORD ReadFrameCount();

		

	private:
		const DWORD deviceIndex;
		const DWORD deviceType;
		VCI_INIT_CONFIG initConfig;


	};


	//Can transceiver board definition
	class DLL_FUNC_API CanTransceiver
	{
	public:

		TransChannel *channel1;
		TransChannel *channel2;

		CanTransceiver();
		void OpenDevice();
		void CloseDevice();
		void GetChannel(DWORD channelNum);
		void GetChannel(DWORD channelNum, VCI_INIT_CONFIG config);

	private:
		//private members
		static const DWORD deviceIndex = 0;
		static const DWORD deviceType = VCI_USBCAN2;


	};

}



#endif