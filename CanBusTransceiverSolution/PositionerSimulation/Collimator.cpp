// This is the main DLL file.

#include "stdafx.h"

#include "Collimator.h"


namespace PositionerSimulationNS {

	/*******************************  OTCXYZControllerCLS Implement*******************************/

	/********************private methods******************/
	/*List<AxisCtlProtoStuct> ^ OTCXYZControllerCLS::GetConfigurationRsp()
	{
		
	}*/

	//AxisCtlProtoStuct OTCXYZControllerCLS::GetStatusMsg()
	//{
	//	
	//}

	/*AxisCtlProtoStuct OTCXYZControllerCLS::GetHeartBeatMsg(OTCXYZHBUpdateReason reason)
	{
		
	}
*/

	//data receive event handler
	void CollimatorCLS::OnCanDataReceivedEvent(List<AxisCtlProtoStuct> ^ receivedQueue)
	{
		Monitor::Enter(receiveLockObject);
		collimatorReceiveQueue->AddRange(receivedQueue);
		Monitor::Exit(receiveLockObject);
	}



	//can data handler threading function
	void CollimatorCLS::CanDataHandlerFunc()
	{
		while (true)
		{
			int frameCount = collimatorReceiveQueue->Count;
			if (frameCount > 0)
			{

				List<AxisCtlProtoStuct> ^ receivedFrameQueue = collimatorReceiveQueue->GetRange(0, frameCount);
				Monitor::Enter(receiveLockObject);
				collimatorReceiveQueue->RemoveRange(0, frameCount);
				Monitor::Exit(receiveLockObject);
				ProcessProtocalData(receivedFrameQueue);
			}
			else
			{
				Thread::Sleep(10);
			}
		}
	}

	//heartbeat threading function
	void CollimatorCLS::HeartBeatFunc()
	{
		while (true)
		{
			List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;

			// 0x7F0 status message update
			ASR2Status1MsgPS ^ response1 = gcnew ASR2Status1MsgPS;
			response1->V_SID = vSid;
			response1->CROSS_SIZE = crossBladeSize;
			response1->LONG_SIZE = longBladeSize;
			response1->LAMP_ON = lampOn;
			response1->Mode = opMode;
			sendBuffer->Add(response1->GetFrame());

			// 0x7F1 status message update
			ASR2Status2MsgPS ^ response2 = gcnew ASR2Status2MsgPS;
			response2->FILTER_TYPE = filterType;
			response2->L_SID = lSid;
			sendBuffer->Add(response2->GetFrame());

//		/*	AxisCtlProtoStuct frame;
//			frame.id = 0x7F2;
//			frame.size = 8;
//			frame.frameData = gcnew array<Byte>(frame.size);
//			frame.frameData[0] = 'S';
//			frame.frameData[1] = '/';
//			frame.frameData[2] = 'N';
//			frame.frameData[3] = 0x30;
//			frame.frameData[4] = 0x30;
//			frame.frameData[5] = '6';
//			frame.frameData[6] = '5';
//			frame.frameData[7] = '1';
//*/
			AxisCtlProtoStuct frame1;
			frame1.id = 0x100;
			frame1.size = 8;
			frame1.frameData = gcnew array<Byte>(frame1.size);
			frame1.frameData[0] = 0x11;
			frame1.frameData[1] = 0x00;
			frame1.frameData[2] = 0x00;
			frame1.frameData[3] = 0x00;
			frame1.frameData[4] = 0x00;
			frame1.frameData[5] = 0x00;
			frame1.frameData[6] = 0x00;
			frame1.frameData[7] = 0x00;
			sendBuffer->Add(frame1);

			//// 0x7F9
			//AxisCtlProtoStuct frame2;
			//frame.id = 0x7F9;
			//frame.size = 8;
			//frame.frameData = gcnew array<Byte>(frame.size);
			//frame.frameData[0] = 0x20;
			//frame.frameData[1] = 0x04;
			//frame.frameData[2] = 0x2B;
			//frame.frameData[3] = 0x00;
			//frame.frameData[4] = 0x1E;
			//frame.frameData[5] = 0x00;
			//frame.frameData[6] = 0x00;
			//frame.frameData[7] = 0x00;
			//sendBuffer->Add(frame2);
			canBus->Send(sendBuffer);

			Thread::Sleep(250);
		}
	}



	void CollimatorCLS::RunBootCode()
	{
		
	}

	void CollimatorCLS::RunAppCode()
	{

		//Start to handle SCB/other controllers' message
		canDataHandlerThreadStart = gcnew ThreadStart(this, &CollimatorCLS::CanDataHandlerFunc);
		canDataHanlerThread = gcnew Thread(canDataHandlerThreadStart);

		heartBeatThreadStart = gcnew ThreadStart(this, &CollimatorCLS::HeartBeatFunc);
		heartBeatThread = gcnew Thread(heartBeatThreadStart);

		canDataHanlerThread->IsBackground = true;
		heartBeatThread->IsBackground = true;

		canBus->CanDataReceivedEvent += gcnew CanBusChannelNS::CanDataReceivedDelegate(this, &CollimatorCLS::OnCanDataReceivedEvent);
		canDataHanlerThread->Start();
		heartBeatThread->Start();
	}

	void CollimatorCLS::ExitAppCode()
	{
		canBus->CanDataReceivedEvent -= gcnew CanBusChannelNS::CanDataReceivedDelegate(this, &CollimatorCLS::OnCanDataReceivedEvent);

		heartBeatThread->Abort();
		canDataHanlerThread->Abort();
		collimatorReceiveQueue->Clear();
	}



	void CollimatorCLS::ProcessProtocalData(List<AxisCtlProtoStuct> ^ dataQueue)
	{
		for each(AxisCtlProtoStuct protocalData in dataQueue)
		{
			if (protocalData.id >= 0x700)
			{
				if (protocalData.id != 0x700 && protocalData.id != 0x703 && protocalData.id != 0x100 && protocalData.id != 0x7A0 && protocalData.id != 0x7F0 && protocalData.id != 0x7F1 && protocalData.id != 0x7F2)
				{
					//Console::WriteLine("collimator {0}", protocalData.id);
				}
			}

			switch (protocalData.id)
			{
			case 0x700:
			{
				 if (protocalData.isRemote)
				 {
					 List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
					 ASR1ModelMsgPS ^ response = gcnew ASR1ModelMsgPS;
					 response->MODEL = asr1Model;
					 sendBuffer->Add(response->GetFrame());
					 canBus->Send(sendBuffer);
				 }
			}
				break;

			case 0x703:
			{
				 if (protocalData.isRemote)
				{
					 List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
					 ASR1SNMsgPS ^ response = gcnew ASR1SNMsgPS;
					 response->SN = asr1SN;
					 AxisCtlProtoStuct frame1 = response->GetFrame();
					 sendBuffer->Add(response->GetFrame());
					 canBus->Send(sendBuffer);


				}
			}
				break;

			case 0x7A6:
			{
						  if (protocalData.isRemote)
						  {
							  List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
							  ASR2ModelMsgPS ^ response = gcnew ASR2ModelMsgPS;
							  response->MODEL = asr2Model;
							  sendBuffer->Add(response->GetFrame());
							  canBus->Send(sendBuffer);
						  }
			}
				break;

			case 0x7A0:
			{
						  ASR2CtlMsgPS ^ requeset = gcnew ASR2CtlMsgPS(protocalData);
						  opMode = requeset->MODE;
						  crossBladeSize = requeset->CROSS_SIZE;
						  longBladeSize = requeset->LONG_SIZE;
						  lampOn = requeset->LAMP_ON;
						  vSid = requeset->V_SID;
			}
				break;

			case 0x702:
			//	Console::WriteLine("{0}", protocalData.id);
				{
					ASR2Ctl2MsgPS ^ request = gcnew ASR2Ctl2MsgPS(protocalData);

					if (request->LASER_ON)
					{
						lampOn = true;
					}
					if (request->LASER_OFF)
					{
						lampOn = false;
					}
					if (request->IS_LAMPCYCLE)
					{
						ThreadStart ^ lampThreadStart = gcnew ThreadStart(this,&CollimatorCLS::LampCycleFunc);
						Thread ^ lampThread = gcnew Thread(lampThreadStart);
						lampThread->IsBackground = true;
						lampThread->Start();
					}
				}
			default:
				break;
			}
		}
	}


	////send collimator update to scb
	//void CollimatorCLS::HBUpdateToSCB()
	//{
	//	while (true)
	//	{
	//		
	//	
	//	}
	//}

	//send staus update to scb
	void CollimatorCLS::StatusUpdateToSCB()
	{
		
	}

	//lamp cycle function
	void CollimatorCLS::LampCycleFunc()
	{
		Thread::Sleep(rampTime * 1000);
		lampOn = true;
		Thread::Sleep(lampTime * 1000);
		lampOn = false;
	}

	/********************public methods******************/

	//ctor
	CollimatorCLS::CollimatorCLS(CanBusChannelCls ^ bus)
	{
		canBus = bus;
		collimatorReceiveQueue = gcnew List<AxisCtlProtoStuct>;
		opMode = CollimatorMode::Auto;
		lampOn = false;
		vSid = lSid = 0;
		crossBladeSize = 346;
		longBladeSize = 294;
		filterType = 1;
	}



	//Power Up
	void CollimatorCLS::PowerUp()
	{
		this->RunBootCode();
		this->RunAppCode();
		isPowOn = true;
	}

	//Power Off
	void CollimatorCLS::PowerOff()
	{
		this->ExitAppCode();
		isPowOn = false;
	}

	//Collimator Power Status
	bool CollimatorCLS::POW_ON::get()
	{
		return isPowOn;
	}
}