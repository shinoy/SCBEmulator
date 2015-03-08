// This is the main DLL file.

#include "stdafx.h"

#include "CanBusChannel.h"

// the test helper methods
void CanBusChannelNS::CanBusChannelCls::TestSend()
{
	Console::WriteLine("start to send now");
	VCI_CAN_OBJ frames[20];
	for (int i = 0; i < 20; i++)
	{
		frames[i].Data[0] = 0x32;
		frames[i].Data[2] = 0xff;
		frames[i].DataLen = 3;
		frames[i].ExternFlag = 0;
		frames[i].ID = 0x310;
		frames[i].RemoteFlag = 0;
	}

	//countFrame += 20;
	sendChannel->SendOnChannel(frames, 20);
}

void CanBusChannelNS::CanBusChannelCls::TestSend(bool dip)
{
	Console::WriteLine("start to send 2 now");
	List<AxisCtlProtoStuct> ^ sendDataList = gcnew List<AxisCtlProtoStuct>;
	for (int i = 0; i < 20; i++)
	{
		AxisCtlProtoStuct data;
		data.size = 4;
		data.frameData = gcnew array<Byte>(data.size);
		for (int j = 0; j < 4; j++)
		{
			data.frameData[j] = 0x10 + j;
		}
		data.id = 0x461;
		sendDataList->Add(data);
	}
	
	sendQueue->AddRange(sendDataList);
}

long CanBusChannelNS::CanBusChannelCls::GetRceiveQueSize()
{
	return this->receiveQueue->Count;
}
long CanBusChannelNS::CanBusChannelCls::GetSendQueSize()
{
	return this->sendQueue->Count;
}

/*************************** private methods *****************************/

// Convert sendQueue data into can protocal array,could only be used by the send thread function and need to release memory manually
PVCI_CAN_OBJ CanBusChannelNS::CanBusChannelCls::ConvertToCanFrame(Int32 ^ frameCount)
{
	int queueLength = sendQueue->Count;
	if (queueLength == 0)
	{
		*frameCount = 0;
		return NULL;
	}
	//we only send at most 500 frame at a time
	if (queueLength > BUFFER_SIZE)
	{
		queueLength = BUFFER_SIZE;
	}

		PVCI_CAN_OBJ dataArray = new VCI_CAN_OBJ[queueLength];
		for (int i = 0; i < queueLength; i++)
		{
			if (sendQueue[i].id == 0)
			{
				continue;
			}
			dataArray[i].DataLen = sendQueue[i].size;
			for (int j = 0; j < dataArray[i].DataLen; j++)
			{
				dataArray[i].Data[j] = sendQueue[i].frameData[j];
			}
			
			dataArray[i].ID = sendQueue[i].id;
			dataArray[i].RemoteFlag = sendQueue[i].isRemote ? 1 : 0;
			dataArray[i].ExternFlag = 0;
		}
		*frameCount = queueLength;
		return dataArray;
}

//sending threading method to send data on can bus
void CanBusChannelNS::CanBusChannelCls::SendThreadFunc()
{
	INT32 ^ frameCount = gcnew INT32(0);
	PVCI_CAN_OBJ sendBuffer;
	while (true)
	{
	//	Console::WriteLine("sending");
		sendBuffer = ConvertToCanFrame(frameCount);
		if (*frameCount > 0)
		{
			sendChannel->SendOnChannel(sendBuffer, *frameCount);
			Monitor::Enter(sendLockObject);
			sendQueue->RemoveRange(0, *frameCount);
			Monitor::Exit(sendLockObject);
			delete[] sendBuffer;
		}

		Thread::Sleep(10);
	}

}


//receive threading method to receive data from can bus and storing in receive queue
void CanBusChannelNS::CanBusChannelCls::ReceiveThreadFunc()
{
	PVCI_CAN_OBJ receiveBuffer = new VCI_CAN_OBJ[BUFFER_SIZE];
	while (true)
	{
		//Console::WriteLine("reading");
		LONG frameCount = receiveChannel->ReadOnChannel(receiveBuffer, BUFFER_SIZE);

		if (frameCount <= 0)
		{
			continue;
		}

		List<AxisCtlProtoStuct> ^ tempQueue = gcnew List<AxisCtlProtoStuct>;
		for (LONG i = 0; i < frameCount; i++)
		{
			AxisCtlProtoStuct protocalData;
			protocalData.id = receiveBuffer[i].ID;
			protocalData.size = receiveBuffer[i].DataLen;
			protocalData.frameData = gcnew array<Byte>(protocalData.size);
			protocalData.isRemote = receiveBuffer[i].RemoteFlag == 1 ? true:false;
			for (UINT j = 0; j < protocalData.size; j++)
			{
				protocalData.frameData[j] = receiveBuffer[i].Data[j];
			}
			tempQueue->Add(protocalData);
		}

		Monitor::Enter(receiveLockObject);
		receiveQueue->AddRange(tempQueue);
		Monitor::Exit(receiveLockObject);
	}
	delete[] receiveBuffer;

}

//distribute threading method to distribute can bus data to devices' buffer on the bus.
void CanBusChannelNS::CanBusChannelCls::DistThreadFunc()
{
	while (true)
	{
		int currentCount = receiveQueue->Count;
		if (currentCount > 0)
		{
			Monitor::Enter(receiveLockObject);
			List<AxisCtlProtoStuct> ^ tempQueue = receiveQueue->GetRange(0, currentCount);
			receiveQueue->RemoveRange(0, currentCount);
			Monitor::Exit(receiveLockObject);
			CanDataReceivedEvent(tempQueue);
		}
		else
		{
			Thread::Sleep(20);
		}
	}

}

/*************************** public methods *****************************/
//default constructor
CanBusChannelNS::CanBusChannelCls::CanBusChannelCls()
{
	board = new CanTransceiver;
}


// the connection method will init channels
void CanBusChannelNS::CanBusChannelCls::Connect()
{
	try
	{
		board->OpenDevice();
		board->GetChannel(0);
		board->GetChannel(1);
		// can adapter channel 1 will be used as receiveing and channel 2 sending
		this->receiveChannel = board->channel1;
		this->sendChannel = board->channel2;
	}
	catch (CanException ex)
	{
		System::Windows::Forms::MessageBox::Show("Can bus connection error");
		throw ex;
	}
	catch (exception ex)
	{
		Console::WriteLine("Unknown error occurs when init board channel!");
	}
}

// the disconnect method will uninit channels
void CanBusChannelNS::CanBusChannelCls::Disconnect()
{
	try
	{
		board->CloseDevice();
	}
	catch (CanException ex)
	{
		throw ex;
	}
	catch (exception ex)
	{
		Console::WriteLine("Unknown error occurs when close board channel!");
	}
}
	
void CanBusChannelNS::CanBusChannelCls::StartBus()
{
	sendThreadStart = gcnew ThreadStart(this, &CanBusChannelCls::SendThreadFunc);
	receiveThreadStart = gcnew ThreadStart(this, &CanBusChannelCls::ReceiveThreadFunc);
	distThreadStart = gcnew ThreadStart(this, &CanBusChannelCls::DistThreadFunc);

	sendThread = gcnew Thread(sendThreadStart);
	receiveThread = gcnew Thread(receiveThreadStart);
	distThread = gcnew Thread(distThreadStart);

	sendThread->IsBackground = true;
	receiveThread->IsBackground = true;
	distThread->IsBackground = true;

	receiveThread->Start();
	sendThread->Start();
	distThread->Start();
}

void CanBusChannelNS::CanBusChannelCls::StopBus()
{
	receiveThread->Abort();
	sendThread->Abort();
	distThread->Abort();
}

void CanBusChannelNS::CanBusChannelCls::Send(List<AxisCtlProtoStuct> ^ sendBuffer)
{
	if (sendBuffer->Count > 0)
	{
		Monitor::Enter(sendLockObject);
		sendQueue->AddRange(sendBuffer);
		Monitor::Exit(sendLockObject);
	}
}