// CanBusChannel.h


#pragma once

#include "ControlCAN.h"
#include "CanBusTransceiver.h"



#define BUFFER_SIZE 500
using namespace System;
using namespace System::Collections::Generic;
using namespace CanTransNS;
using namespace System::Threading;

namespace CanBusChannelNS {

	public value struct AxisCtlProtoStuct
	{
		UINT id;
		array<Byte> ^ frameData;
		UINT size;
		bool isRemote = false;
	};

	public delegate void CanDataReceivedDelegate(List<AxisCtlProtoStuct> ^ dataQueue);

	public ref class CanBusChannelCls
	{
	private:
		CanTransceiver * board;

		static Object ^ receiveLockObject = gcnew Object();
		static Object ^ sendLockObject = gcnew Object();

		// the can channel
		TransChannel * receiveChannel;
		TransChannel * sendChannel;

		// the protocal data queue
		List<AxisCtlProtoStuct> ^ receiveQueue = gcnew List<AxisCtlProtoStuct>;
		List<AxisCtlProtoStuct> ^ sendQueue = gcnew List<AxisCtlProtoStuct>;

		//The reading and sending thread
		ThreadStart ^sendThreadStart;
		ThreadStart ^receiveThreadStart;

		Thread ^ sendThread;
		Thread ^ receiveThread;

		//The distributor thread
		ThreadStart ^distThreadStart;
		Thread ^distThread;

		//receive threading method to receive data from can bus and storing in receive queue
		void ReceiveThreadFunc();

		//sending threading method to send data on can bus
		void SendThreadFunc();

		//distribute threading method to distribute can bus data to devices' buffer on the bus.
		void DistThreadFunc();

		//the convert method from sendQueue to Can data array
		PVCI_CAN_OBJ  ConvertToCanFrame(Int32 ^);

	public:
		UInt64 countFrame = 0;

		//the received event will trigger devices connected with bus and send data into devices' buffer.
		event CanDataReceivedDelegate^ CanDataReceivedEvent;
		//ctor
		CanBusChannelCls();
	
		//test helper
		void TestSend();
		void TestSend(bool);
		long GetRceiveQueSize();
		long GetSendQueSize();

		//connect to scb
		void Connect();

		//disconnect with scb
		void Disconnect();
	
		//start the can bus transmit
		void StartBus();

		//stop the can bus transmit
		void StopBus();

		//send data on the bus
		void Send(List<AxisCtlProtoStuct> ^ sendBuffer);

	};
}
