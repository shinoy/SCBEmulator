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

			if (this->manSwitchStatus == false)
			{
			
				// 0x7F0 status message update
				ASR2Status1MsgPS ^ response1 = gcnew ASR2Status1MsgPS;
				response1->V_SID = vSid;
				response1->CROSS_SIZE = CROSS_BLADE;
				response1->LONG_SIZE = LONG_BLADE;
				response1->LAMP_ON = lampOn;
				response1->Mode = opMode = CollimatorMode::Auto;
				sendBuffer->Add(response1->GetFrame());

				//-------------------------------------------------//
				//Console::WriteLine("{0:x4}\n{1:x2}\n{2:x2}\n{3:x2}\n{4:x2}\n{5:x2}\n{6:x2}\n{7:x2}\n{8:x2}\n"
				//	, response1->GetFrame().id, response1->GetFrame().frameData[0], response1->GetFrame().frameData[1], response1->GetFrame().frameData[2], response1->GetFrame().frameData[3]
				//	, response1->GetFrame().frameData[4], response1->GetFrame().frameData[5], response1->GetFrame().frameData[6], response1->GetFrame().frameData[7]);
				//------------------------------------------------//

				// 0x7F1 status message update
				ASR2Status2MsgPS ^ response2 = gcnew ASR2Status2MsgPS;
				response2->FILTER_TYPE = filterType;
				response2->L_SID = lSid;
				sendBuffer->Add(response2->GetFrame());

				//-------------------------------------------------//
				//Console::WriteLine("{0:x4}\n{1:x2}\n{2:x2}\n{3:x2}\n{4:x2}\n{5:x2}\n{6:x2}\n{7:x2}\n{8:x2}\n"
				//	, response2->GetFrame().id, response2->GetFrame().frameData[0], response2->GetFrame().frameData[1], response2->GetFrame().frameData[2], response2->GetFrame().frameData[3]
				//	, response2->GetFrame().frameData[4], response2->GetFrame().frameData[5], response2->GetFrame().frameData[6], response2->GetFrame().frameData[7]);
				//------------------------------------------------//

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
				frame1.frameData[0] = 0x11;//0x15;
				frame1.frameData[1] = 0x00;
				frame1.frameData[2] = 0x00;
				frame1.frameData[3] = 0x00;
				frame1.frameData[4] = 0x00;
				frame1.frameData[5] = 0x00;
				frame1.frameData[6] = 0x00;
				frame1.frameData[7] = 0x00;
				sendBuffer->Add(frame1);

				//----------------------------------------------------//
				//Console::WriteLine("{0:x4}\n{1:x2}\n{2:x2}\n{3:x2}\n{4:x2}\n{5:x2}\n{6:x2}\n{7:x2}\n{8:x2}\n"
				//	, frame1.id, frame1.frameData[0], frame1.frameData[1], frame1.frameData[2], frame1.frameData[3]
				//	, frame1.frameData[4], frame1.frameData[5], frame1.frameData[6], frame1.frameData[7]);
				//---------------------------------------------------//

				AxisCtlProtoStuct frame2;
				frame2.id = 0x7F9;
				frame2.size = 8;
				frame2.frameData = gcnew array<Byte>(frame2.size);
				frame2.frameData[0] = 0x20;
				frame2.frameData[1] = 0x04;
				frame2.frameData[2] = 0x2B;
				frame2.frameData[3] = 0x00;
				frame2.frameData[4] = 0x1E;
				frame2.frameData[5] = 0x00;
				frame2.frameData[6] = 0x00;
				frame2.frameData[7] = 0x00;
				sendBuffer->Add(frame2);
			}
			else 
			{
				CROSS_BLADE = 0;
				LONG_BLADE = 0;
				ASR2Status1MsgPS ^ response1 = gcnew ASR2Status1MsgPS;
				response1->V_SID = vSid;
				response1->CROSS_SIZE = CROSS_BLADE; //crossBladeSize=0;
				response1->LONG_SIZE = LONG_BLADE; //longBladeSize=0;
				response1->LAMP_ON = lampOn;
				response1->Mode = opMode = CollimatorMode::Manual;
				sendBuffer->Add(response1->GetFrame());

				ASR2Status2MsgPS ^ response2 = gcnew ASR2Status2MsgPS;
				response2->FILTER_TYPE = filterType;
				response2->L_SID = lSid;
				sendBuffer->Add(response2->GetFrame());

				AxisCtlProtoStuct frame1;
				frame1.id = 0x100;
				frame1.size = 8;
				frame1.frameData = gcnew array<Byte>(frame1.size);
				frame1.frameData[0] = 0x11;//0x15;
				frame1.frameData[1] = 0x00;
				frame1.frameData[2] = 0x00;
				frame1.frameData[3] = 0x00;
				frame1.frameData[4] = 0x00;
				frame1.frameData[5] = 0x00;
				frame1.frameData[6] = 0x00;
				frame1.frameData[7] = 0x00;
				sendBuffer->Add(frame1);

				//// 0x7F9
				AxisCtlProtoStuct frame2;
				frame2.id = 0x7F9;
				frame2.size = 8;
				frame2.frameData = gcnew array<Byte>(frame2.size);
				frame2.frameData[0] = 0x20;
				frame2.frameData[1] = 0x09;//0x04;//0x01;
				frame2.frameData[2] = 0x2B;
				frame2.frameData[3] = 0x00;
				frame2.frameData[4] = 0x1E;
				frame2.frameData[5] = 0x00;
				frame2.frameData[6] = 0x00;
				frame2.frameData[7] = 0x00;
				sendBuffer->Add(frame2);
			}
					
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
		
		heartBeatThread->Start();
		canDataHanlerThread->Start();
	}

	void CollimatorCLS::ExitAppCode()
	{
		//power off, send initial starting position, cross 0 long 0, or it will receive ctrl message 7a0  
		//to alter next time starting position
		List<AxisCtlProtoStuct> ^ sendEndingBuffer = gcnew List<AxisCtlProtoStuct>;
		CROSS_BLADE = 0;
		LONG_BLADE = 0;
		ASR2Status1MsgPS ^ response1 = gcnew ASR2Status1MsgPS;
		response1->V_SID = vSid;
		response1->CROSS_SIZE = CROSS_BLADE; //crossBladeSize=0;
		response1->LONG_SIZE = LONG_BLADE; //longBladeSize=0;
		response1->LAMP_ON = lampOn;
		response1->Mode = opMode = CollimatorMode::Manual;
		sendEndingBuffer->Add(response1->GetFrame());

		canBus->Send(sendEndingBuffer);
		//

		Thread::Sleep(200);

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
						  //Console::WriteLine("Auto Center .....{0:x4}\n{1:x2}\n{2:x2}\n{3:x2}\n{4:x2}\n{5:x2}\n{6:x2}\n{7:x2}\n{8:x2}\n"
						  //, protocalData.id, protocalData.frameData[0], protocalData.frameData[1], protocalData.frameData[2], protocalData.frameData[3]
						  //, protocalData.frameData[4], protocalData.frameData[5], protocalData.frameData[6], protocalData.frameData[7]);
						  ASR2CtlMsgPS ^ requeset = gcnew ASR2CtlMsgPS(protocalData);
						  opMode = requeset->MODE;
						  if (this->manSwitchStatus == true)
						  {
							  CROSS_BLADE = 0;
							  LONG_BLADE = 0;
						  }
						  else
						  {
							  CROSS_BLADE = requeset->CROSS_SIZE;
							  LONG_BLADE = requeset->LONG_SIZE;
						  }
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
			case 0x7F0:
			{

						  // when OTC position remove from atuo center, the values of cross and long are going to change to zero
						  if (protocalData.frameData[3]== 0x00)
						  {
							  CROSS_BLADE = 0;
							  LONG_BLADE = 0;
						  }

						  //Console::WriteLine("{0:x4}\n{1:x2}\n{2:x2}\n{3:x2}\n{4:x2}\n{5:x2}\n{6:x2}\n{7:x2}\n{8:x2}\n"
						  //  , protocalData.id, protocalData.frameData[0], protocalData.frameData[1], protocalData.frameData[2], protocalData.frameData[3]
						  //  , protocalData.frameData[4], protocalData.frameData[5], protocalData.frameData[6], protocalData.frameData[7]);

			} break;
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
		opMode = CollimatorMode::Manual;//CollimatorMode::Manual; //CollimatorMode::Auto;
		lampOn = false;
		vSid = lSid = 0;
		crossBladeSize = 0;//346;
		longBladeSize = 0;//294;
		filterType = 3;
		manSwitchStatus = false;
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

	//Collimator manual switch on
	void CollimatorCLS::MAN_SWITCH_ON::set(bool value)
	{
		this->manSwitchStatus = value;
	}

	//Collimator mannual switch status
	bool CollimatorCLS::MAN_SWITCH_ON::get()
	{
		return this->manSwitchStatus;
	}

	//
	int CollimatorCLS::LONG_BLADE::get()
	{
		return longBladeSize;
	}

	//
	int CollimatorCLS::CROSS_BLADE::get()
	{
		return crossBladeSize;
	}

	//
	void CollimatorCLS::LONG_BLADE::set(int value)
	{
		longBladeSize = value;
		BladePositionUpdateEvent();
	}

	//
	void CollimatorCLS::CROSS_BLADE::set(int value)
	{
		crossBladeSize = value;
		BladePositionUpdateEvent();
	}

	//
	void CollimatorCLS::FILTER_POSITION::set(byte value)
	{
		filterType = value;
	}

	//
	byte CollimatorCLS::FILTER_POSITION::get()
	{
		return filterType;
	}
}