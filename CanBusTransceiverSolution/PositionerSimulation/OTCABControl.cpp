// OTC AB Control implement

#include "stdafx.h"

#include "OTCABControl.h"

namespace PositionerSimulationNS {

	/*******************************  OTCABControllerCLS Implement*******************************/

	/********************private methods******************/
	List<AxisCtlProtoStuct> ^ OTCABControllerCLS::GetConfigurationRsp()
	{
		CtlConfigRespPS ^ bootConfigResponse = gcnew CtlConfigRespPS(ControlType::OTCAB);
		bootConfigResponse->HARDWARE_VERSION = hardwareVersion;
		bootConfigResponse->BoardType = this->hardwareType;
		bootConfigResponse->MODE = runMode;
		bootConfigResponse->SWITCH_CODE = swCode;
		bootConfigResponse->BOOT_TAG = bootTagVersion;
		bootConfigResponse->BOOT_REV = bootRevision;
		bootConfigResponse->FPGA_TAG = fpgaTagVersion;
		bootConfigResponse->FPGA_REV = fpgaRevision;
		CtlAppVerRspPS^ bootCtlAppResponse = gcnew CtlAppVerRspPS(ControlType::OTCAB);
		bootCtlAppResponse->APP_TAG_VER = appTagVersion;
		bootCtlAppResponse->APP_REVISION = appRevision;
		bootCtlAppResponse->MT_TAG_VER = motoParaTagVersion;
		bootCtlAppResponse->MT_PARA_REV = motoParaRevision;
		if (hardwareType == ControlHardType::Invalid)
		{
			bootCtlAppResponse->OPTIONS = 0xFF;
		}
		else
		{
			bootCtlAppResponse->OPTIONS = 0x00;
		}
		CtlBootStrapVerRspPS^ bootCtlBootStrapResponse = gcnew CtlBootStrapVerRspPS(ControlType::OTCAB);
		bootCtlBootStrapResponse->BOOTSTRAP_TAG_VER = bootStrapTagVersion;
		bootCtlBootStrapResponse->BOOTSTRAP_REV = bootStrapRevision;

		AxisCtlProtoStuct frame1 = bootConfigResponse->GetFrame();
		AxisCtlProtoStuct frame2 = bootCtlAppResponse->GetFrame();
		AxisCtlProtoStuct frame3 = bootCtlBootStrapResponse->GetFrame();

		List<AxisCtlProtoStuct> ^ tempFrameBuffer = gcnew List<AxisCtlProtoStuct>;
		tempFrameBuffer->Add(frame1);
		tempFrameBuffer->Add(frame2);
		tempFrameBuffer->Add(frame3);
		return tempFrameBuffer;
	}

	AxisCtlProtoStuct OTCABControllerCLS::GetStatusMsg(ABStaUpdateReason reason)
	{
		ABStatusMsgPS ^ statusUpdateMsg = gcnew ABStatusMsgPS;
		statusUpdateMsg->OP_MODE = operateMode;
		statusUpdateMsg->A_STA = aStatus;
		statusUpdateMsg->B_STA = bStatus;

		statusUpdateMsg->UPDATE_REASON = reason;

		return statusUpdateMsg->GetFrame();
	}

	AxisCtlProtoStuct OTCABControllerCLS::GetHeartBeatMsg(OTCABHBUpdateReason reason)
	{
		OTCABHBMsgPS ^ msg = gcnew OTCABHBMsgPS;
		//if (operateMode == ControlOpMode::MMReq || operateMode == ControlOpMode::Idle || operateMode == ControlOpMode::MLock)
		if (operateMode != ControlOpMode::NRmCal)
		{
			msg->A_POSITION = aPosition;
			msg->B_POSITION = bPosition;
		}
		else
		{
			msg->A_POSITION = -3276.8;
			msg->B_POSITION = -3276.8;
		}
	
		OTCABCalStatus calStatus;
		calStatus.AMaxSet = AmaxSet;
		calStatus.AMinSet = AminSet;
		calStatus.BMaxSet = BmaxSet;
		calStatus.BMinSet = BminSet;
		calStatus.PinCalSet = PinCalSet;
		msg->CAL_STA = calStatus;
		msg->UPDATE_REASON = reason;

		return msg->GetFrame();
	}

	// Automove function
	void OTCABControllerCLS::AutoMoveFunc()
	{
		int aStepCount = 0, bStepCount = 0;
		double aLastStep = 0, bLastStep = 0;
		bool aFinish = false, bFinish = false;

		bool aUp, bUp;

		if (aDestination > aPosition)
		{
			aUp = true;
			aStepCount = (int)(aDestination * 10 - aPosition * 10) / (int)(autoMoveStep * 10);
			aLastStep = (aDestination - aPosition) - autoMoveStep*aStepCount;
		}
		else
		{
			aUp = false;
			aStepCount = (int)(aPosition * 10 - aDestination * 10) / (int)(autoMoveStep * 10);
			aLastStep = (aPosition - aDestination) - autoMoveStep*aStepCount;
		}

		if (bDestination > bPosition)
		{
			bUp = true;
			bStepCount = (int)(bDestination * 10 - bPosition * 10) / (int)(autoMoveStep * 10);
			bLastStep = (bDestination - bPosition) - autoMoveStep*bStepCount;
		}
		else
		{
			bUp = false;
			bStepCount = (int)(bPosition * 10 - bDestination * 10) / (int)(autoMoveStep * 10);
			bLastStep = (bPosition - bDestination) - autoMoveStep*bStepCount;
		}

		Thread::Sleep(1000);
		if (autoContinueNeed)
		{
			while (!aFinish || !bFinish)
			{
				if (automoveEvent->WaitOne())
				{
					if (!aFinish)
					{
						if (aUp)
						{
							if (aStatus.brakeEn)
							{
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								ABStaUpdateReason reason;
								reason.aStatusChange = true;
								reason.scbRequest = true;
								aStatus.mtRun = true;
								aStatus.brakeEn = false;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);

							}
							if (aStepCount > 0)
							{
								A_POS += autoMoveStep;
								aStepCount--;
							}
							if (aStepCount == 0)
							{
								A_POS += aLastStep;
								aFinish = true;
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								ABStaUpdateReason reason;
								reason.aStatusChange = true;
								reason.scbRequest = true;
								aStatus.mtRun = false;
								aStatus.brakeEn = true;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);
							}
						}
						else
						{
							if (aStatus.brakeEn)
							{
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								ABStaUpdateReason reason;
								reason.aStatusChange = true;
								reason.scbRequest = true;
								aStatus.mtRun = true;
								aStatus.brakeEn = false;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);

							}
							if (aStepCount > 0)
							{
								A_POS -= autoMoveStep;
								aStepCount--;
							}
							if (aStepCount == 0)
							{
								A_POS -= aLastStep;
								aFinish = true;
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								ABStaUpdateReason reason;
								reason.aStatusChange = true;
								reason.scbRequest = true;
								aStatus.mtRun = false;
								aStatus.brakeEn = true;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);
							}
						}  // end aUP if
					}
					
					if (aFinish && (!bFinish))
					{
						if (bUp)
						{
							if (!bStatus.mtRun)
							{
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								ABStaUpdateReason reason;
								reason.bStatusChange = true;
								reason.scbRequest = true;
								bStatus.mtRun = true;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);

							}
							if (bStepCount > 0)
							{
								B_POS += autoMoveStep;
								bStepCount--;
							}
							if (bStepCount == 0)
							{
								B_POS += bLastStep;
								bFinish = true;
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								ABStaUpdateReason reason;
								reason.bStatusChange = true;
								reason.scbRequest = true;
								bStatus.mtRun = false;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);
							}
						}
						else
						{
							if (!bStatus.mtRun)
							{
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								ABStaUpdateReason reason;
								reason.bStatusChange = true;
								reason.scbRequest = true;
								bStatus.mtRun = true;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);

							}
							if (bStepCount > 0)
							{
								B_POS -= autoMoveStep;
								bStepCount--;
							}
							if (bStepCount == 0)
							{
								B_POS -= bLastStep;
								bFinish = true;
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								ABStaUpdateReason reason;
								reason.bStatusChange = true;
								reason.scbRequest = true;
								bStatus.mtRun = false;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);
							}
						}// end bUp if
					} // end aFinish if
				} // end if autoMoveEvent

			} //end while
		} // end if autoContinueNeed

		// send the finish response to SCB and controllers
		AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::OTCAB);
		response->AUTOMOVE_STATUS = AMoveStatus::Complete;
		response->SW_SETTING = swCode;
		List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;
		sendbuffer->Add(response->GetFrame());
		canBus->Send(sendbuffer);
		sendbuffer->Clear();

		// wait for other controllers' finish response and send the status update response
		if (moveFinishEvent->WaitOne())
		{
			operateMode = ControlOpMode::Idle;

			ABStaUpdateReason reason;
			reason.scbRequest = true;
			reason.opModeChange = true;
			sendbuffer->Add(GetStatusMsg(reason));
			canBus->Send(sendbuffer);
		}
	//	ExitAutoMove();

		xyzAutomoveFlag = WSDAutoMoveFlag = TBLAutoMoveFlag = false;
		xyzAutoFinishFlag = WSDAutoFinishFlag = TBLAutoFinishFlag = false;
		return;

	}


	void OTCABControllerCLS::RunBootCode()
	{
		runMode = ControlHWMode::BOOT;
		Console::WriteLine("OTC AB booting ....");

		List<AxisCtlProtoStuct> ^ configResponse = GetConfigurationRsp();
		canBus->Send(configResponse);

		//boot code will take about 3s to run into application code.
	}

	void OTCABControllerCLS::RunAppCode()
	{
		runMode = ControlHWMode::APP;
		Console::WriteLine("OTC AB APP initalizing ....");

		//calibrate the OTC A B
		AminSet = AmaxSet = BminSet = BmaxSet = PinCalSet = true;

		//set the ab position
		aPosition = 0;
		bPosition = 0;

		aMinPos = -136;
		aMaxPos = 136;
		bMinPos = -181;
		bMaxPos = 181;

		aDevicePos= aMotorPos = aPosition;
		bDevicePos= bMotorPos = bPosition;

		aServErr = bServErr = 0;
		aCurrent = bCurrent = 0.02;
		aVoltage = bVoltage = -0.25;

		aVel = bVel = 0;
		aAep = bAep = aPot = bPot = aLLiPos = bLLiPos = 0;


		operateMode = ControlOpMode::Idle;
		Console::WriteLine("OTC AB is now in IDLE mode");
		List<AxisCtlProtoStuct> ^ initResponse = GetConfigurationRsp();


		//engage the brake
		aStatus.brakeEn = true;
		aStatus.mtRun = false;

		bStatus.mtRun = false;


		ABStaUpdateReason reason;
		reason.opModeChange = true;
		reason.aStatusChange = true;
		reason.bStatusChange = true;
		

		initResponse->Add(GetStatusMsg(reason));

		//send the init heartbeat message
		OTCABHBUpdateReason heartbeatReason;
		heartbeatReason.axisMove = true;
		heartbeatReason.CalStaChange = true;
		heartbeatReason.scbRequest = true;

		initResponse->Add(GetHeartBeatMsg(heartbeatReason));


		//send the error response update
		CtlErrStatRespMsgPS ^ errorResponse = gcnew CtlErrStatRespMsgPS(ControlType::OTCAB);
		errorResponse->ERROR_CODE = 0;
		errorResponse->UPDATEREASON = CtlErrStatRespUpdateReason::PeriodicUpdate;
		initResponse->Add(errorResponse->GetFrame());

		//App init update
		canBus->Send(initResponse);

		//Start to handle SCB/other controllers' message
		canDataHandlerThreadStart = gcnew ThreadStart(this, &OTCABControllerCLS::CanDataHandlerFunc);
		canDataHanlerThread = gcnew Thread(canDataHandlerThreadStart);

		heartBeatThreadStart = gcnew ThreadStart(this, &OTCABControllerCLS::HeartBeatFunc);
		heartBeatThread = gcnew Thread(heartBeatThreadStart);

		canDataHanlerThread->IsBackground = true;
		heartBeatThread->IsBackground = true;

		canBus->CanDataReceivedEvent += gcnew CanBusChannelNS::CanDataReceivedDelegate(this, &OTCABControllerCLS::OnCanDataReceivedEvent);
		canDataHanlerThread->Start();
		heartBeatThread->Start();
	}

	void OTCABControllerCLS::ExitAppCode()
	{
		canBus->CanDataReceivedEvent -= gcnew CanBusChannelNS::CanDataReceivedDelegate(this, &OTCABControllerCLS::OnCanDataReceivedEvent);

		heartBeatThread->Abort();
		canDataHanlerThread->Abort();

		runMode = ControlHWMode::BOOT;
		operateMode = ControlOpMode::NoInit;

		aStatus.brakeEn = aStatus.hmSensor = aStatus.inDetent = aStatus.mtRun = false;
		bStatus.dtEn = bStatus.dtSoEn = bStatus.mtRun = false;

		AminSet = AmaxSet = BminSet = BmaxSet = PinCalSet = false;

		otcABReceiveQueue->Clear();
	}

	void OTCABControllerCLS::ExitAutoMove()
	{
		if (moveFinishEvent->WaitOne())
		{
			List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;
			operateMode = ControlOpMode::Idle;
			ABStaUpdateReason reason;
			reason.opModeChange = true;
			reason.scbRequest = true;
			sendbuffer->Add(GetStatusMsg(reason));
			canBus->Send(sendbuffer);
			xyzAutomoveFlag = WSDAutoMoveFlag = TBLAutoMoveFlag = false;
			xyzAutoFinishFlag = WSDAutoFinishFlag = TBLAutoFinishFlag = false;
		}
	}

	void OTCABControllerCLS::ProcessProtocalData(List<AxisCtlProtoStuct> ^ dataQueue)
	{
		for each(AxisCtlProtoStuct protocalData in dataQueue)
		{
			/*if (protocalData.id != 0x461 && protocalData.id != 0x410 && protocalData.id != 0x210 && protocalData.id != 0x310 && protocalData.id != 0x300 && protocalData.id != 0x310 && protocalData.id != 0x220 && protocalData.id != 0x320)
			{
				Console::WriteLine("{0:x} on OTCAB", protocalData.id);
			}*/
			switch (protocalData.id)
			{
			case 0x461:
			//	Console::WriteLine("SCB Configuration Request Received on AB control");
				{
					SCBConfigReqPS ^ request = gcnew SCBConfigReqPS(protocalData);
					if (request->OTCAB_REQ == true)
					{
						List<AxisCtlProtoStuct> ^ otcabConfigMsg = GetConfigurationRsp();
						canBus->Send(otcabConfigMsg);
					}
				}
				break;
			case 0x410:
			//	Console::WriteLine("SCB Controller Status Request Received on AB control");
				{
					SCBCtrlStatusReqPS^ request = gcnew SCBCtrlStatusReqPS(protocalData);
					if (request->OTCAB_RT == true)
					{
						List<AxisCtlProtoStuct> ^ sendBUffer = gcnew List<AxisCtlProtoStuct>;
						if (request->POSUPDATE_REQ == true)
						{
							OTCABHBUpdateReason reason;
							reason.scbRequest = true;
							sendBUffer->Add(GetHeartBeatMsg(reason));
						}
						if (request->CONTSTA_REQ == true)
						{
							ABStaUpdateReason reason;
							reason.scbRequest = true;
							sendBUffer->Add(GetStatusMsg(reason));
						}
						if (request->ERRSTA_REQ == true)
						{
							CtlErrStatRespMsgPS ^ response = gcnew CtlErrStatRespMsgPS(ControlType::OTCAB);
							response->ERROR_CODE = 0;
							response->UPDATEREASON = CtlErrStatRespUpdateReason::SCBRequest;
							sendBUffer->Add(response->GetFrame());
						}
						

						canBus->Send(sendBUffer);
					}
				}
				break;

			case 0X2E0:
			{
						  AutoMoveRespMsgPS ^ xyzResponse = gcnew AutoMoveRespMsgPS(protocalData);
						  if (xyzResponse->AUTOMOVE_STATUS == AMoveStatus::AModeAccepted)
						  {
							  xyzAutomoveFlag = true;
							
						  }
						  if ((xyzResponse->AUTOMOVE_STATUS == AMoveStatus::Complete || xyzResponse->AUTOMOVE_STATUS == AMoveStatus::Abort || xyzResponse->AUTOMOVE_STATUS == AMoveStatus::Error)&& xyzAutomoveFlag)
						  {
							  xyzAutoFinishFlag = true;

							  if ((xyzAutoFinishFlag == xyzAutomoveFlag) && (WSDAutoMoveFlag == WSDAutoFinishFlag) && (TBLAutoMoveFlag == TBLAutoFinishFlag))
							  {
								  moveFinishEvent->Set();
							  }
						  }
			}
				break;
			case 0x2E2:
			{
						  AutoMoveRespMsgPS ^ wsdResponse = gcnew AutoMoveRespMsgPS(protocalData);
						  if (wsdResponse->AUTOMOVE_STATUS == AMoveStatus::AModeAccepted)
						  {
							  WSDAutoMoveFlag = true;
						  }
						  if ((wsdResponse->AUTOMOVE_STATUS == AMoveStatus::Complete || wsdResponse->AUTOMOVE_STATUS == AMoveStatus::Abort || wsdResponse->AUTOMOVE_STATUS == AMoveStatus::Error) && WSDAutoMoveFlag)
						  {
							  WSDAutoFinishFlag = true;

							  if ((xyzAutoFinishFlag == xyzAutomoveFlag) && (WSDAutoMoveFlag == WSDAutoFinishFlag) && (TBLAutoMoveFlag == TBLAutoFinishFlag))
							  {
								  moveFinishEvent->Set();
							  }
						  }
			}
				break;
			case 0x2E3:
			{
						  AutoMoveRespMsgPS ^ tblResponse = gcnew AutoMoveRespMsgPS(protocalData);
						  if (tblResponse->AUTOMOVE_STATUS == AMoveStatus::AModeAccepted)
						  {
							  TBLAutoMoveFlag = true;
						  }
						  if ((tblResponse->AUTOMOVE_STATUS == AMoveStatus::Complete || tblResponse->AUTOMOVE_STATUS == AMoveStatus::Abort || tblResponse->AUTOMOVE_STATUS == AMoveStatus::Error) && TBLAutoMoveFlag)
						  {
							  TBLAutoFinishFlag = true;

							  if ((xyzAutoFinishFlag == xyzAutomoveFlag) && (WSDAutoMoveFlag == WSDAutoFinishFlag) && (TBLAutoMoveFlag == TBLAutoFinishFlag))
							  {
								  moveFinishEvent->Set();
							  }
						  }
			}
				break;

			case 0x401:
				Console::WriteLine("OTC AB Auto Move Destination received");
				{
					SCBABSetAutoDesMsgPS ^ request = gcnew SCBABSetAutoDesMsgPS(protocalData);
					if (operateMode == ControlOpMode::Idle)
					{
						if (request->A_POS != -32768)
						{
							aDestination = request->A_POS;
						}
						else
						{
							aDestination = aPosition;
						}
						if (request->B_POS != -32768)
						{
							bDestination = request->B_POS;
						}
						else
						{
							bDestination = bPosition;
						}
						

						List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

						//change the opmode to AMTP
						operateMode = ControlOpMode::AMTP;
						ABStaUpdateReason reason;
						reason.opModeChange = true;
						reason.scbRequest = true;
						aStatus.mtRun = true;
						bStatus.mtRun = true;
						sendbuffer->Add(GetStatusMsg(reason));
						canBus->Send(sendbuffer);

						//Accepte the destination
						AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::OTCAB);
						response->AUTOMOVE_STATUS = AMoveStatus::AModeAccepted;
						response->SW_SETTING = swCode;
						sendbuffer->Clear();
						sendbuffer->Add(response->GetFrame());
						canBus->Send(sendbuffer);
					}
				}
				break;

			case 0x101:
				
			{
						  if (autoMoveThread != nullptr)
						  {
							  if (autoMoveThread->IsAlive)
							  {
								  autoMoveThread->Abort();
							  }
						  }

						  xyzAutomoveFlag = WSDAutoMoveFlag = TBLAutoMoveFlag = false;
						  xyzAutoFinishFlag = WSDAutoFinishFlag = TBLAutoFinishFlag = false;

						  List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

						  // send abort response to SCB
						  AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::OTCAB);
						  response->AUTOMOVE_STATUS = AMoveStatus::Abort;
						  response->SW_SETTING = swCode;
						  sendbuffer->Add(response->GetFrame());

						  if (operateMode == ControlOpMode::AMTP)
						  {
							  //change status to idle
							  operateMode = ControlOpMode::Idle;
							  ABStaUpdateReason reason;
							  reason.opModeChange = true;
							  reason.scbRequest = true;
							  sendbuffer->Add(GetStatusMsg(reason));
						  }
						  canBus->Send(sendbuffer);
			}
				break;

			case 0x102:
			{
						  SCBAutoEnPS ^ request = gcnew SCBAutoEnPS(protocalData);
						  if (operateMode == ControlOpMode::AMTP)
						  {
							  if (request->CONTINUE_NEED)
							  {
								  autoContinueNeed = true;
							  }
							  else
							  {
								  autoContinueNeed = false;
							  }

							  List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

							  // accept enable signal
							  // add destination verification here later ?
							  AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::OTCAB);
							  response->AUTOMOVE_STATUS = AMoveStatus::EnAccepted;
							  response->SW_SETTING = swCode;
							  sendbuffer->Add(response->GetFrame());
							  canBus->Send(sendbuffer);

							  autoMoveThread = gcnew Thread(gcnew ThreadStart(this, &OTCABControllerCLS::AutoMoveFunc));
							  autoMoveThread->IsBackground = true;
							  autoMoveThread->Start();
						  }
			}
				break;

			case 0x103:
			{
						  if (operateMode == ControlOpMode::AMTP)
						  {

							  List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

							  // need to add destination verification here ?

							  AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::OTCAB);
							  response->AUTOMOVE_STATUS = AMoveStatus::Complete;
							  response->SW_SETTING = swCode;
							  sendbuffer->Add(response->GetFrame());
							  canBus->Send(sendbuffer);
							
							  // wait for other controllers' finish flag and update status
							  Thread ^ exitAutomoveThread = gcnew Thread(gcnew ThreadStart(this, &OTCABControllerCLS::ExitAutoMove));
							  exitAutomoveThread->IsBackground = true;
							  exitAutomoveThread->Start();
						   
						  }
			}
			break;

			case 0x104:
			{
						  if (operateMode == ControlOpMode::AMTP)
						  {
							  automoveEvent->Set();
						  }
			}
				break;

			case 0x456:
				if (protocalData.frameData[0] = 0x01)
				{
					cmdLength = protocalData.frameData[1] - 3;
					receivedCMD = 0;
					diagCMD = gcnew array<wchar_t>(cmdLength);
					if (cmdLength <= 5)
					{
						for (int i = 0; i < cmdLength; i++)
						{
							diagCMD[i] = protocalData.frameData[i + 2];
							receivedCMD++;
						}
						// process the command here
						RemoteDiagHandler();
					}
					else
					{
						for (int i = 0; i < 6; i++)
						{
							diagCMD[i] = protocalData.frameData[i + 2];
							receivedCMD++;
						}
					}
				}
				else
				{
					for (int i = 0; i < 8; i++)
					{
						if (protocalData.frameData[i] == 0x0D)
						{
							//process the command here
							RemoteDiagHandler();
						}
						else
						{
							diagCMD[receivedCMD] = protocalData.frameData[i];
							receivedCMD++;
						}
					}
				}
				break;
			case 0x120:
				{
						  Thread::Sleep(delayTime + sweepTime);
						  List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
						  AxisSyncRespMsgPS ^ response = gcnew AxisSyncRespMsgPS(ControlType::OTCAB);
						  response->ERROR_CODE = 0x00;
						  response->SYNC_STATUS = SyncStatus::SyncComplete;
						  sendBuffer->Add(response->GetFrame());
						  canBus->Send(sendBuffer);
				}
				break;


			case 0x123:
				{
						  List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
						  XYZSyncMoveParaPS^ request = gcnew XYZSyncMoveParaPS(protocalData);
						  delayTime = request->SYNC_DELAY;
						  ltDirection = request->TBLMOVE_DICT;

						  AxisSyncRespMsgPS ^ response = gcnew AxisSyncRespMsgPS(ControlType::OTCAB);
						  response->ERROR_CODE = 0x00;
						  response->SYNC_STATUS = SyncStatus::SyncACK;
						  sendBuffer->Add(response->GetFrame());
						  canBus->Send(sendBuffer);
				}
				break;

			case 0x124:
				{
						  List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
						  SCBTomoParaPS ^ request = gcnew SCBTomoParaPS(protocalData);
						  if (request->TOMO_TRUE)
						  {
							  layerHeight = request->LAYER_HEIGHT;
							  sweepAngle = request->SWEEP_ANGLE;
							  sweepTime = request->SWEEP_TIME;
							  delayTime = request->DELAY_TIME;

							  operateMode = ControlOpMode::LT;
							  ABStaUpdateReason reason;
							  reason.opModeChange = true;
							  sendBuffer->Add(GetStatusMsg(reason));
							  canBus->Send(sendBuffer);

						  }
						  else
						  {
							  operateMode = ControlOpMode::Idle;
							  ABStaUpdateReason reason;
							  reason.opModeChange = true;
							  sendBuffer->Add(GetStatusMsg(reason));
							  canBus->Send(sendBuffer);
						  }
			
				}

				break;
			default:
				break;
			}
		}
	}


	// convert the diagnostic command response string to can frames list
	List<AxisCtlProtoStuct>^ OTCABControllerCLS::ConvertDiagResponse(String^ response)
	{
		int i = 2;
		List<AxisCtlProtoStuct> ^ responseFrameList = gcnew List<AxisCtlProtoStuct>;
		AxisCtlProtoStuct tempFrame;
		tempFrame.id = 0x2C6;
		tempFrame.frameData = gcnew array<Byte>(8);
		tempFrame.frameData[0] = 0x01;
		tempFrame.frameData[1] = response->Length + 3;
		for each (char c in response)
		{
			if (i % 8 == 7)
			{
				tempFrame.frameData[7] = c;
				tempFrame.size = 8;
				responseFrameList->Add(tempFrame);
				tempFrame.frameData = gcnew array<Byte>(8);
				i++;
			}
			else
			{
				tempFrame.frameData[i % 8] = c;
				i++;
			}
		}
		if (i % 8 == 0)
		{
			tempFrame.frameData = gcnew array<Byte>(1);
			tempFrame.frameData[0] = 0x0D;
			tempFrame.size = 1;
			responseFrameList->Add(tempFrame);
		}
		else
		{
			tempFrame.frameData[i % 8] = 0x0D;
			tempFrame.size = i % 8 + 1;
			responseFrameList->Add(tempFrame);
		}

		return responseFrameList;
	}


	// receive diagnostic command and send response to scb
	void OTCABControllerCLS::RemoteDiagHandler()
	{
		if (cmdLength > 0)
		{
			String ^ test = gcnew String(diagCMD);
			if (test->Equals("u?v"))
			{
				String ^responseStr = "C?v Sensor5V=1 I2CPwr=1 PotV=1 Cbd1Pwr=1";
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
				canBus->Send(frameList);
			}
			if (test->Equals("U:1?d"))
			{
				String ^responseStr = "C:0?d p=" + (aPosition * 10).ToString() + " vel=" + aVel.ToString() + " err=" + (aServErr * 10).ToString() + " mp=" + (aMotorPos * 10).ToString() + " dp="+(aDevicePos*10).ToString()+" aep=" + (aAep * 10).ToString() + " pot=" + (aPot * 10).ToString() + " v=" + (aVoltage * 4).ToString() + " i=" + (aCurrent * 256).ToString() + " Lp="+(aLLiPos*10000).ToString();
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
				canBus->Send(frameList);
			}
			if (test->Equals("U:0?d"))
			{
				String ^responseStr = "C:0?d p=" + (bPosition * 10).ToString() + " vel=" + bVel.ToString() + " err=" + (bServErr * 10).ToString() + " mp=" + (bMotorPos * 10).ToString() + " dp=" +(bDevicePos*10).ToString()+ " aep=" + (bAep * 10).ToString() + " pot=" + (bPot * 10).ToString() + " v=" + (bVoltage * 4).ToString() + " i=" + (bCurrent * 256).ToString() + " Lp="+(bLLiPos*10000).ToString();
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
				canBus->Send(frameList);
			}
			
			if (test->Equals("U:1?L"))
			{
				String ^responseStr = "C:0?l st=" + (aMinPos * 10).ToString() + " et=" + (aMaxPos * 10).ToString();
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
				canBus->Send(frameList);
			}
			if (test->Equals("U:0?L"))
			{
				String ^responseStr = "C:0?l st=" + (bMinPos * 10).ToString() + " et=" + (bMaxPos * 10).ToString();
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
				canBus->Send(frameList);
			}

		}
		receivedCMD = 0;
		cmdLength = 0;
		diagCMD = nullptr;
	}

	//send position update to scb
	void OTCABControllerCLS::HBUpdateToSCB(OTCABHBUpdateReason reason)
	{
		List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
		AxisCtlProtoStuct updateMsg = GetHeartBeatMsg(reason);
		sendBuffer->Add(updateMsg);
		canBus->Send(sendBuffer);
	}

	//update status to scb
	void OTCABControllerCLS::StatusUpdateToSCB(ABStaUpdateReason reason)
	{
		List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
		AxisCtlProtoStuct updateMsg = GetStatusMsg(reason);
		sendBuffer->Add(updateMsg);
		canBus->Send(sendBuffer);
	}


	//data receive event handler
	void OTCABControllerCLS::OnCanDataReceivedEvent(List<AxisCtlProtoStuct> ^ receivedQueue)
	{
		Monitor::Enter(receiveLockObject);
		otcABReceiveQueue->AddRange(receivedQueue);
		Monitor::Exit(receiveLockObject);
	}



	//can data handler threading function
	void OTCABControllerCLS::CanDataHandlerFunc()
	{
		while (true)
		{
			int frameCount = otcABReceiveQueue->Count;
			if (frameCount > 0)
			{

				List<AxisCtlProtoStuct> ^ receivedFrameQueue = otcABReceiveQueue->GetRange(0, frameCount);
				Monitor::Enter(receiveLockObject);
				otcABReceiveQueue->RemoveRange(0, frameCount);
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
	void OTCABControllerCLS::HeartBeatFunc()
	{
		while (true)
		{
			OTCABHBUpdateReason reason;
			reason.scbRequest = false;
			reason.axisMove = false;
			reason.hbTimeOut = true;
			AxisCtlProtoStuct msg = GetHeartBeatMsg(reason);
			List<AxisCtlProtoStuct> ^sendBuffer = gcnew List<AxisCtlProtoStuct>;
			sendBuffer->Add(msg);
			canBus->Send(sendBuffer);

		//	Console::WriteLine("AB Heartbeat");
		//	Console::WriteLine("AB Mode:" + operateMode.ToString());
			Thread::Sleep(800);
		}
	}



	/********************public methods******************/

	//ctor
	OTCABControllerCLS::OTCABControllerCLS(CanBusChannelCls ^ bus)
	{

		hardwareVersion = 0x01;
		bootTagVersion = 0x34;
		bootRevision = 0x18F8;
		appTagVersion = 0x34;
		appRevision = 0x18F8;
		motoParaTagVersion = 0x34;
		motoParaRevision = 0x18F8;
		bootStrapTagVersion = 0x34;
		bootStrapRevision = 0x18F8;

		swCode = OTCTYPE4;

		aStatus.brakeEn = aStatus.hmSensor = aStatus.inDetent = aStatus.mtRun= false;
		bStatus.dtEn = bStatus.dtSoEn = bStatus.mtRun = false;

		moveStepLength = 1;
		canBus = bus;
		otcABReceiveQueue = gcnew List<AxisCtlProtoStuct>;

	}

	// A B position property 
	double OTCABControllerCLS::A_POS::get()
	{
		return aPosition;
	}

	void OTCABControllerCLS::A_POS::set(double value)
	{
		aPosition = value;
		OTCABHBUpdateReason reason;
		reason.axisMove = true;
		HBUpdateToSCB(reason);
	
	}

	double OTCABControllerCLS::B_POS::get()
	{
		return bPosition;
	}

	void OTCABControllerCLS::B_POS::set(double value)
	{
		bPosition = value;
		OTCABHBUpdateReason reason;
		reason.axisMove = true;
		HBUpdateToSCB(reason);
		
	}

	double OTCABControllerCLS::MOVE_STEP::get()
	{
		return moveStepLength;
	}
	
	void OTCABControllerCLS::MOVE_STEP::set(double value)
	{
		moveStepLength = value;
	}

	bool OTCABControllerCLS::A_REL_BTN::get()
	{
		return aRelBtnStatus;
	}

	void OTCABControllerCLS::A_REL_BTN::set(bool value)
	{
		if (value == true)
		{
			aRelBtnStatus = true;
			aStatus.brakeEn = false;
			ABStaUpdateReason reason;
			reason.aStatusChange = true;
			if (operateMode == ControlOpMode::Idle)
			{
				operateMode = ControlOpMode::MMReq;
				reason.opModeChange = true;
			}
			StatusUpdateToSCB(reason);
			
		}
		else
		{
			aRelBtnStatus = false;
			aStatus.brakeEn = true;
			ABStaUpdateReason reason;
			reason.aStatusChange = true;
			if (operateMode == ControlOpMode::MMReq && (!bRelBtnStatus))
			{
				operateMode = ControlOpMode::Idle;
				reason.opModeChange = true;
			}
			StatusUpdateToSCB(reason);
		}
	}

	bool OTCABControllerCLS::B_REL_BTN::get()
	{
		return bRelBtnStatus;
	}

	void OTCABControllerCLS::B_REL_BTN::set(bool value)
	{
		if (value == true)
		{
			bRelBtnStatus = true;
			ABStaUpdateReason reason;
			reason.bStatusChange = true;
			if (operateMode == ControlOpMode::Idle)
			{
				operateMode = ControlOpMode::MMReq;
				reason.opModeChange = true;
			}
			StatusUpdateToSCB(reason);

		}
		else
		{
			bRelBtnStatus = false;
			ABStaUpdateReason reason;
			reason.bStatusChange = true;
			if (( operateMode == ControlOpMode::MMReq) && (!aRelBtnStatus))
			{
				operateMode = ControlOpMode::Idle;
				reason.opModeChange = true;
			}
			StatusUpdateToSCB(reason);
		}
	}

	ControlOpMode OTCABControllerCLS::OP_MODE::get()
	{
		return operateMode;
	}

	void OTCABControllerCLS::OP_MODE::set(ControlOpMode value)
	{
		operateMode = value;
	}

	

	//Power Up
	void OTCABControllerCLS::PowerUp()
	{
		this->RunBootCode();
		this->RunAppCode();
	}

	//Power Off
	void OTCABControllerCLS::PowerOff()
	{
		this->ExitAppCode();
	}



}