// OTC WSD Control implement

#include "stdafx.h"

#include "TableControl.h"

namespace PositionerSimulationNS {

	/*******************************  TBL ControllerCLS Implement*******************************/

	/********************private methods******************/
	List<AxisCtlProtoStuct> ^ TBLControllerCLS::GetConfigurationRsp()
	{
		CtlConfigRespPS ^ bootConfigResponse = gcnew CtlConfigRespPS(ControlType::TBL);
		bootConfigResponse->HARDWARE_VERSION = hardwareVersion;
		bootConfigResponse->BoardType = this->hardwareType;
		bootConfigResponse->MODE = runMode;
		bootConfigResponse->SWITCH_CODE = swCode;
		bootConfigResponse->BOOT_TAG = bootTagVersion;
		bootConfigResponse->BOOT_REV = bootRevision;
		// tbl has no FPGA
		bootConfigResponse->FPGA_TAG = 0x00;
		bootConfigResponse->FPGA_REV = 0x00;
		CtlAppVerRspPS^ bootCtlAppResponse = gcnew CtlAppVerRspPS(ControlType::TBL);
		bootCtlAppResponse->APP_TAG_VER = appTagVersion;
		bootCtlAppResponse->APP_REVISION = appRevision;
		bootCtlAppResponse->MT_TAG_VER = motoParaTagVersion;
		bootCtlAppResponse->MT_PARA_REV = motoParaRevision;
		switch (detectorType)
		{
		case DetectorType::DRX1:
			bootCtlAppResponse->OPTIONS &= 0x1F;
			bootCtlAppResponse->OPTIONS |= 0x60;
			break;
		case DetectorType::Trixell:
			bootCtlAppResponse->OPTIONS &= 0x1F;
			break;
		case DetectorType::Varian:
			bootCtlAppResponse->OPTIONS &= 0x1F;
			bootCtlAppResponse->OPTIONS |= 0x20;
			break;
		default:
			bootCtlAppResponse->OPTIONS &= 0x1F;
			bootCtlAppResponse->OPTIONS |= 0xE0;
			break;
		}
		
		if (keyPadInstalled)
		{
			bootCtlAppResponse->OPTIONS &= 0xFE;
			bootCtlAppResponse->OPTIONS |= 0x01;
		}
		else
		{
			bootCtlAppResponse->OPTIONS &= 0xFE;
			bootCtlAppResponse->OPTIONS |= 0x00;
		}

		CtlBootStrapVerRspPS^ bootCtlBootStrapResponse = gcnew CtlBootStrapVerRspPS(ControlType::TBL);
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

	AxisCtlProtoStuct TBLControllerCLS::GetStatusMsg(TBLStatusUpdateReason reason)
	{
		TBLStatusMsgPS ^ statusUpdateMsg = gcnew TBLStatusMsgPS;
		statusUpdateMsg->SW_STATUS = swStatus;
		statusUpdateMsg->MTCONTROLS = motionControl;
		statusUpdateMsg->MTSENSORS = sensors;
		statusUpdateMsg->BUCKY_DT = buckyDetent;
		statusUpdateMsg->Z_DT = zDetent;
		statusUpdateMsg->OP_MODE = operateMode;
		statusUpdateMsg->KEYPAD_STATUS = keypadStatus;
		statusUpdateMsg->INDICATOR_STATUS = indicatorStatus;
		statusUpdateMsg->DETECTOR_ID = detectorID;

		statusUpdateMsg->UPDATE_REASON = reason;
	
		return statusUpdateMsg->GetFrame();
	}

	AxisCtlProtoStuct TBLControllerCLS::GetHeartBeatMsg(TBLHBUpdateReason reason)
	{
		TBLHBMsgPS ^ msg = gcnew TBLHBMsgPS;
	//	if (operateMode == ControlOpMode::MMReq || operateMode == ControlOpMode::Idle || operateMode == ControlOpMode::MLock)
		if (operateMode != ControlOpMode::NRmCal)
		{
			msg->X_POS = xPosition;
			msg->Y_POS = yPosition;
			msg->Z_POS = zPosition;
		}
		else
		{
			msg->X_POS = -8192;
			msg->Y_POS = -8192;
			msg->Z_POS = -8192;
		}

		msg->LAYOUT = layout;

		TBLCalStatus calStatus;
		calStatus.longMaxSet = longMaxSet;
		calStatus.longMinSet = longMinSet;
		calStatus.pinCalDone = pinCalDone;
		calStatus.rmCalDone = rmCalDone;
		calStatus.tblLocEst = tblocEst;
		calStatus.zMaxSet = zMaxSet;
		calStatus.zMinSet = zMinSet;

		msg->CALSTATUS = calStatus;
		msg->UPDATEREASON = reason;

		return msg->GetFrame();
	}


	void TBLControllerCLS::RunBootCode()
	{
		runMode = ControlHWMode::BOOT;
		Console::WriteLine("TBL booting ....");

		List<AxisCtlProtoStuct> ^ configResponse = GetConfigurationRsp();
		canBus->Send(configResponse);

	}

	void TBLControllerCLS::RunAppCode()
	{
		runMode = ControlHWMode::APP;
		Console::WriteLine("TBL APP initalizing ....");

		//calibrate the TBL
		zMinSet = longMinSet = zMaxSet = longMaxSet = tblocEst = rmCalDone = pinCalDone = true;
		
		//set the tbl position
		xPosition = 839;
		yPosition = 416;
		zPosition = 524;
		
		xmotorPos = xabsEncoder = xdevicePos = xPosition;
		zmotorPos = zabsEncoder = zdevicePos = zPosition;

		xPosMinLim = 824;
		xPosMaxLim = 1500;

		zPosMinLim = 500;
		zPosMaxLim = 600;

		xpot = xserErr = xvel = 0;
		zpot = zserErr = zvel = 0;

		xlliPos = xPosition;
		zlliPos = zPosition;


		//set the detector type
		detectorType = DetectorType::DRX1;

		//set the detector id
		detectorID = 0x02;

		//set the motion controller status
		motionControl.floatSolenAssert = motionControl.longMotorRun = motionControl.zMotorRun = sensors.floatCollison = false;

		//set the motion sensors
		sensors.dtIntable = true;
		sensors.longBuckySensor = false;
	    sensors.buckyInPortrait = true;
		sensors.buckyInLand = false;
		sensors.gridPresent = sensors.dtExtend =  false;

		//TBL Detents status
		buckyDetent = 0;
		zDetent = 0;

		//set the room layout
		layout = TBLRoomLayout::Layout2;



		operateMode = ControlOpMode::Idle;
		Console::WriteLine("WSD is now in IDLE mode");
		List<AxisCtlProtoStuct> ^ initResponse = GetConfigurationRsp();

	   // Controller Status Message
		TBLStatusUpdateReason reason;
		reason.detectorIdCHANGE = true;
		reason.opModeChange = true;
		reason.mtSensorChange = true;
		//reason.scbRequest = true;
		reason.softDtChange = true;
		reason.swStatusChange = true;
		reason.xAxisStatusChange = true;

		initResponse->Add(GetStatusMsg(reason));

		// Init heartbeat message
		TBLHBUpdateReason heartbeatReason;
		heartbeatReason.calStatusChange = true;
		heartbeatReason.movementOccur = true;
		heartbeatReason.scbRequest = true;

		initResponse->Add(GetHeartBeatMsg(heartbeatReason));

		// Error Message
		CtlErrStatRespMsgPS ^ errorResponse = gcnew CtlErrStatRespMsgPS(ControlType::TBL);
		errorResponse->ERROR_CODE = 0;
		errorResponse->UPDATEREASON = CtlErrStatRespUpdateReason::PeriodicUpdate;
		initResponse->Clear();
		initResponse->Add(errorResponse->GetFrame());

		//App init update
		canBus->Send(initResponse);

		//Start to handle SCB/other controllers' message
		canDataHandlerThreadStart = gcnew ThreadStart(this, &TBLControllerCLS::CanDataHandlerFunc);
		canDataHanlerThread = gcnew Thread(canDataHandlerThreadStart);

		heartBeatThreadStart = gcnew ThreadStart(this, &TBLControllerCLS::HeartBeatFunc);
		heartBeatThread = gcnew Thread(heartBeatThreadStart);

		zMotoMoveThreadStart = gcnew ThreadStart(this, &TBLControllerCLS::MotoZMove);
		zMotoMoveThread = gcnew Thread(zMotoMoveThreadStart);

		canDataHanlerThread->IsBackground = true;
		heartBeatThread->IsBackground = true;
		zMotoMoveThread->IsBackground = true;

		canBus->CanDataReceivedEvent += gcnew CanBusChannelNS::CanDataReceivedDelegate(this, &TBLControllerCLS::OnCanDataReceivedEvent);
		canDataHanlerThread->Start();
		heartBeatThread->Start();
		zMotoMoveThread->Start();
	}

	void TBLControllerCLS::ExitAppCode()
	{
		canBus->CanDataReceivedEvent -= gcnew CanBusChannelNS::CanDataReceivedDelegate(this, &TBLControllerCLS::OnCanDataReceivedEvent);

		heartBeatThread->Abort();
		canDataHanlerThread->Abort();

		runMode = ControlHWMode::BOOT;
		operateMode = ControlOpMode::NoInit;


		//set the motion controller status
		motionControl.floatSolenAssert = motionControl.longMotorRun = motionControl.zMotorRun = false;

		//set the motion sensors
		sensors.longBuckySensor = sensors.dtIntable = sensors.buckyInPortrait = true;
		sensors.gridPresent = sensors.dtExtend = sensors.buckyInLand = sensors.floatCollison = false;

		zMinSet = longMinSet = zMaxSet = longMaxSet = tblocEst = rmCalDone = pinCalDone = false;

		tblReceiveQueue->Clear();
	}


	// convert the diagnostic command response string to can frames list
	List<AxisCtlProtoStuct>^ TBLControllerCLS::ConvertDiagResponse(String^ response)
	{
		int i = 2;
		List<AxisCtlProtoStuct> ^ responseFrameList = gcnew List<AxisCtlProtoStuct>;
		AxisCtlProtoStuct tempFrame;
		tempFrame.id = 0x2C5;
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
	void TBLControllerCLS::RemoteDiagHandler()
	{
		if (cmdLength > 0)
		{
			String ^ test = gcnew String(diagCMD);
			if (test->Equals("u?v"))
			{
				String ^responseStr = "C?v Sensor5V=1 I2CPwr=1 PotV=1 Cbd1Pwr=1 ";
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
				canBus->Send(frameList);
			}
			if (test->Equals("U:0?d"))
			{
				String ^responseStr = "C:0?d p=" + (zPosition * 4).ToString() + " vel=" + zvel.ToString() + " err=" + (zserErr * 4).ToString() + " mp=" + (zmotorPos * 4).ToString() + " dp=" + (zdevicePos * 4).ToString() + " aep=" + (zabsEncoder * 4).ToString() + " pot=" + (zpot * 4).ToString() + " v=" + (zvoltage * 4).ToString() + " i=" + (zcurrent * 256).ToString() + " Lp=" + (zlliPos * 1000).ToString();
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
				canBus->Send(frameList);
			}
			if (test->Equals("U:1?d"))
			{
				String ^responseStr = "C:0?d p=" + (xPosition * 4).ToString() + " vel=" + xvel.ToString() + " err=" + (xserErr * 4).ToString() + " mp=" + (xmotorPos * 4).ToString() + " dp=" + (xdevicePos * 4).ToString() + " aep=" + (xabsEncoder * 4).ToString() + " pot=" + (xpot * 4).ToString() + " v=" + (xvoltage * 4).ToString() + " i=" + (xcurrent * 256).ToString() + " Lp=" + (xlliPos * 1000).ToString();
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
				canBus->Send(frameList);
			}
			if (test->Equals("U:0?L"))
			{
				String ^responseStr = "C:0?l st=" + (zPosMinLim * 4).ToString() + " et=" + (zPosMaxLim * 4).ToString();
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
				canBus->Send(frameList);
			}
			if (test->Equals("U:1?L"))
			{
				String ^responseStr = "C:0?l st=" + (xPosMinLim * 4).ToString() + " et=" + (xPosMaxLim * 4).ToString();
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
				canBus->Send(frameList);
			}

		}
		receivedCMD = 0;
		cmdLength = 0;
		diagCMD = nullptr;
	}


	// Automove function
	void TBLControllerCLS::AutoMoveFunc()
	{
		int xStepCount = 0, yStepCount = 0, zStepCount = 0;
		double xLastStep = 0, yLastStep = 0, zLastStep = 0;
		bool xFinish = false, yFinish = false, zFinish = false;

		bool xUp, yUp, zUp;

		if (xDestination > xPosition)
		{
			xUp = true;
			xStepCount = (int)((xDestination - xPosition) / autoMoveStep);
			xLastStep = (xDestination - xPosition) - xStepCount*autoMoveStep;
		}
		else
		{
			xUp = false;
			xStepCount = (int)((xPosition - xDestination) / autoMoveStep);
			xLastStep = (xPosition - xDestination) - xStepCount*autoMoveStep;
		}

		if (yDestination > yPosition)
		{
			yUp = true;
			yStepCount = (int)((yDestination - yPosition) / autoMoveStep);
			yLastStep = (yDestination - yPosition) - yStepCount*autoMoveStep;
		}
		else
		{
			yUp = false;
			yStepCount = (int)((yPosition - yDestination) / autoMoveStep);
			yLastStep = (yPosition - yDestination) - yStepCount*autoMoveStep;
		}

		if (zDestination > zPosition)
		{
			zUp = true;
			zStepCount = (int)((zDestination - zPosition) / autoMoveStep);
			zLastStep = (zDestination - zPosition) - zStepCount * autoMoveStep;
		}
		else
		{
			zUp = false;
			zStepCount = (int)((zPosition - zDestination) / autoMoveStep);
			zLastStep = (zPosition - zDestination) - zStepCount * autoMoveStep;
		}

		Thread::Sleep(1000);

		if (autoContinueNeed)
		{
			while (!xFinish || !yFinish || !zFinish)
			{
				if (automoveEvent->WaitOne())
				{
					if (!zFinish)
					{
						if (zUp)
						{
							if (zStepCount > 0)
							{
								Z_POS += autoMoveStep;
								zStepCount--;
							}
							if (zStepCount == 0)
							{
								Z_POS += zLastStep;
								zFinish = true;
							}

						}
						else
						{
							if (zStepCount > 0)
							{
								Z_POS -= autoMoveStep;
								zStepCount--;
							}
							if (zStepCount == 0)
							{
								Z_POS -= zLastStep;
								zFinish = true;
							}
						}
					}
					

					if (zFinish && (!yFinish))
					{
						if (yUp)
						{
							if (yStepCount > 0)
							{
								Y_POS += autoMoveStep;
								yStepCount--;
							}
							if (yStepCount == 0)
							{
								Y_POS += yLastStep;
								yFinish = true;
							}
						}
						else
						{
							if (yStepCount > 0)
							{
								Y_POS -= autoMoveStep;
								yStepCount--;
							}
							if (yStepCount == 0)
							{
								Y_POS -= yLastStep;
								yFinish = true;
							}
						} // end of yUp
					}// end of zFinish

					if (zFinish && yFinish && (!xFinish))
					{
						if (xUp)
						{
							if (xStepCount > 0)
							{
								X_POS += autoMoveStep;
								xStepCount--;
							}
							if (xStepCount == 0)
							{
								X_POS += xLastStep;
								xFinish = true;
							}
						}
						else
						{
							if (xStepCount > 0)
							{
								X_POS -= autoMoveStep;
								xStepCount--;
							}
							if (xStepCount == 0)
							{
								X_POS -= xLastStep;
								xFinish = true;
							}
						} // end of xUp if
					}// end of z/y finish if
				} // end of if automoveEvent
			}// end of while
		}// end of if autoContinueNeed

		// send the finish response to SCB and controllers
		AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::TBL);
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

			TBLStatusUpdateReason reason;
			reason.scbRequest = true;
			reason.opModeChange = true;
			sendbuffer->Add(GetStatusMsg(reason));
			canBus->Send(sendbuffer);
		}
		//ExitAutoMove();

		ABAutoMoveFlag = WSDAutoMoveFlag = XYZAutoMoveFlag = false;
		ABAutoFinishFlag = WSDAutoFinishFlag = XYZAutoFinishFlag = false;
		return;

	}// end of auto move function

	// exit auto movement function
	void TBLControllerCLS::ExitAutoMove()
	{
		if (moveFinishEvent->WaitOne())
		{
			List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;
			operateMode = ControlOpMode::Idle;
			TBLStatusUpdateReason reason;
			reason.opModeChange = true;
			reason.scbRequest = true;
			sendbuffer->Add(GetStatusMsg(reason));
			canBus->Send(sendbuffer);
			ABAutoMoveFlag = WSDAutoMoveFlag = XYZAutoMoveFlag = false;
			ABAutoFinishFlag = WSDAutoFinishFlag = XYZAutoFinishFlag = false;
		}
	}

	// LT movement thread function
	void TBLControllerCLS::LTMove()
	{
		if (waitForBuckyStart->WaitOne())
		{
			List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;
			TBLSyncMoveStartPS ^ startRequest = gcnew TBLSyncMoveStartPS;
			startRequest->AT_SPEED_DELAY = delayTime;
			sendbuffer->Add(startRequest->GetFrame());

			Thread::Sleep(delayTime + sweepTime);

			AxisSyncRespMsgPS ^ response = gcnew AxisSyncRespMsgPS(ControlType::TBL);
			response->ERROR_CODE = 0x00;
			response->SYNC_STATUS = SyncStatus::SyncComplete;
			sendbuffer->Add(response->GetFrame());
			canBus->Send(sendbuffer);
		}
	}

	void TBLControllerCLS::ProcessProtocalData(List<AxisCtlProtoStuct> ^ dataQueue)
	{
		for each(AxisCtlProtoStuct protocalData in dataQueue)
		{
			switch (protocalData.id)
			{
			case 0x461:
			//	Console::WriteLine("TBL Configuration Request Received on WSD control");
				{
					SCBConfigReqPS ^ request = gcnew SCBConfigReqPS(protocalData);
					if (request->TBL_REQ == true)
					{
						List<AxisCtlProtoStuct> ^ tblConfigMsg = GetConfigurationRsp();
						canBus->Send(tblConfigMsg);
					}
				}
				break;
			case 0x410:
				//Console::WriteLine("SCB Controller Status Request Received on TBL control");
				{
					SCBCtrlStatusReqPS^ request = gcnew SCBCtrlStatusReqPS(protocalData);
					if (request->TBL_RT == true)
					{
						List<AxisCtlProtoStuct> ^ sendBUffer = gcnew List<AxisCtlProtoStuct>;
						if (request->POSUPDATE_REQ == true)
						{
							TBLHBUpdateReason reason1;
							reason1.scbRequest = true;
							sendBUffer->Add(GetHeartBeatMsg(reason1));
						}
						if (request->CONTSTA_REQ == true)
						{
							TBLStatusUpdateReason reason;
							reason.scbRequest = true;
							sendBUffer->Add(GetStatusMsg(reason));
						}
						if (request->ERRSTA_REQ == true)
						{
							CtlErrStatRespMsgPS ^ response = gcnew CtlErrStatRespMsgPS(ControlType::TBL);
							response->ERROR_CODE = 0;
							response->UPDATEREASON = CtlErrStatRespUpdateReason::SCBRequest;
							sendBUffer->Add(response->GetFrame());
						}
						if (request->LLIPOS_REQ == true)
						{
							TBLLLIStatusMsgPS ^ response = gcnew TBLLLIStatusMsgPS;
							response->BUCKY_LLIPOS = (Int32)(xPosition * 1000);
							response->BUCKYTRAV_START = xPosMinLim;
							response->BUCKYTRAV_END = xPosMaxLim;
							sendBUffer->Add(response->GetFrame());
						}

						canBus->Send(sendBUffer);
					}
				}
				break;


			case 0x2E0:
			{
						  AutoMoveRespMsgPS ^ xyzResponse = gcnew AutoMoveRespMsgPS(protocalData);
						  if (xyzResponse->AUTOMOVE_STATUS == AMoveStatus::AModeAccepted)
						  {
							  XYZAutoMoveFlag = true;
						  }
						  if ((xyzResponse->AUTOMOVE_STATUS == AMoveStatus::Complete || xyzResponse->AUTOMOVE_STATUS == AMoveStatus::Abort || xyzResponse->AUTOMOVE_STATUS == AMoveStatus::Error) && XYZAutoMoveFlag)
						  {
							  XYZAutoFinishFlag = true;

							  if ((ABAutoFinishFlag == ABAutoMoveFlag) && (XYZAutoMoveFlag == XYZAutoFinishFlag) && (WSDAutoMoveFlag == WSDAutoFinishFlag))
							  {
								  moveFinishEvent->Set();
							  }
						  }
			}
				break;

			case 0x2E1:
			{
						  AutoMoveRespMsgPS ^ abResponse = gcnew AutoMoveRespMsgPS(protocalData);
						  if (abResponse->AUTOMOVE_STATUS == AMoveStatus::AModeAccepted)
						  {
							  ABAutoMoveFlag = true;
						  }
						  if ((abResponse->AUTOMOVE_STATUS == AMoveStatus::Complete || abResponse->AUTOMOVE_STATUS == AMoveStatus::Abort || abResponse->AUTOMOVE_STATUS == AMoveStatus::Error) && ABAutoMoveFlag)
						  {
							  ABAutoFinishFlag = true;

							  if ((ABAutoFinishFlag == ABAutoMoveFlag) && (WSDAutoMoveFlag == WSDAutoFinishFlag) && (XYZAutoMoveFlag == XYZAutoFinishFlag))
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

							  if ((ABAutoFinishFlag == ABAutoMoveFlag) && (WSDAutoMoveFlag == WSDAutoFinishFlag) && (XYZAutoMoveFlag == XYZAutoFinishFlag))
							  {
								  moveFinishEvent->Set();
							  }
						  }
			}
				break;
			
			//case 0x401:
			//{
			//			  List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

			//			  // Accept the destination
			//			  AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::TBL);
			//			  response->AUTOMOVE_STATUS = AMoveStatus::AModeAccepted;
			//			  response->SW_SETTING = swCode;
			//			  sendbuffer->Add(response->GetFrame());

			//			  // change opmode to AMTP
			//			  operateMode = ControlOpMode::AMTP;
			//			  TBLStatusUpdateReason reason;
			//			  reason.opModeChange = true;
			//			  reason.scbRequest = true;
			//			  sendbuffer->Add(GetStatusMsg(reason));

			//			  // send on bus
			//			  canBus->Send(sendbuffer);
			//}
			//	break;

			case 0x403:
				Console::WriteLine("TBL Auto Move Destination received");
				{
					SCBTBLSetAutoDesMsgPS ^ request = gcnew SCBTBLSetAutoDesMsgPS(protocalData);
					if (operateMode == ControlOpMode::Idle)
					{
						if (request->X_POS != -8192)
						{
							xDestination = request->X_POS;
						}
						else
						{
							xDestination = xPosition;
						}
						if (request->Y_POS != -8192)
						{
							yDestination = request->Y_POS;
						}
						else
						{
							yDestination = yPosition;
						}
						if (request->Z_POS != -8192)
						{
							zDestination = request->Z_POS;
						}
						else
						{
							zDestination = zPosition;
						}
						
						List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

						// Accept the destination
						AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::TBL);
						response->AUTOMOVE_STATUS = AMoveStatus::AModeAccepted;
						response->SW_SETTING = swCode;
						sendbuffer->Add(response->GetFrame());

						// change opmode to AMTP
						operateMode = ControlOpMode::AMTP;
						TBLStatusUpdateReason reason;
						reason.opModeChange = true;
						reason.scbRequest = true;
						sendbuffer->Add(GetStatusMsg(reason));

						// send on bus
						canBus->Send(sendbuffer);
					}
				}
				break;

			// SCB auto movement abort
			case 0x101:
			{
						  if (autoMoveThread != nullptr)
						  {

							  if (autoMoveThread->IsAlive)
							  {
								  autoMoveThread->Abort();
							  }
						  }

						  ABAutoMoveFlag = WSDAutoMoveFlag = XYZAutoMoveFlag = false;
						  ABAutoFinishFlag = WSDAutoFinishFlag = XYZAutoFinishFlag = false;

						  List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

						  // send abort response to SCB
						  AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::TBL);
						  response->AUTOMOVE_STATUS = AMoveStatus::Abort;
						  response->SW_SETTING = swCode;
						  sendbuffer->Add(response->GetFrame());

						  //change status to idle
						  if (operateMode == ControlOpMode::AMTP)
						  {
							  operateMode = ControlOpMode::Idle;
							  TBLStatusUpdateReason reason;
							  reason.opModeChange = true;
							  reason.scbRequest = true;
							  sendbuffer->Add(GetStatusMsg(reason));
						  }

						  canBus->Send(sendbuffer);
			}
				break;

				// automovement enable signal
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
							  AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::TBL);
							  response->AUTOMOVE_STATUS = AMoveStatus::EnAccepted;
							  response->SW_SETTING = swCode;
							  sendbuffer->Add(response->GetFrame());
							  canBus->Send(sendbuffer);

							  autoMoveThread = gcnew Thread(gcnew ThreadStart(this, &TBLControllerCLS::AutoMoveFunc));
							  autoMoveThread->IsBackground = true;
							  autoMoveThread->Start();
						  }
			}
				break;

				// SCB Auto movement verify
			case 0x103:
			{
						  if (operateMode == ControlOpMode::AMTP)
						  {

							  List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

							  // need to add destination verification here ?

							  AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::TBL);
							  response->AUTOMOVE_STATUS = AMoveStatus::Complete;
							  response->SW_SETTING = swCode;
							  sendbuffer->Add(response->GetFrame());
							  canBus->Send(sendbuffer);
				

							  // wait for other controllers' finish flag and update status
							  Thread ^ exitAutomoveThread = gcnew Thread(gcnew ThreadStart(this, &TBLControllerCLS::ExitAutoMove));
							  exitAutomoveThread->IsBackground = true;
							  exitAutomoveThread->Start();
						  }
			}
				break;


				// SCB Auto movement continue
			case 0x104:
			{
						  if (operateMode == ControlOpMode::AMTP)
						  {
							  automoveEvent->Set();
						  }
			}
				break;

			case 0x422:
				//Console::WriteLine("SCB Clear Calculation Received on TBL control");
				{
					SCBClrCalPS^ request = gcnew SCBClrCalPS(protocalData);
					if (request->TBL_Clear == true)
					{
						List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
						if (request->CAL_TYPE1 == true)
						{
							zMinSet = false;
						}
						if (request->CAL_TYPE2 == true)
						{
							longMinSet = false;
						}
						if (request->CAL_TYPE3 == true)
						{
							zMaxSet = false;
						}
						if (request->CAL_TYPE4 == true)
						{
							longMaxSet = false;
						}
						if (request->CAL_TYPE7 == true)
						{
							rmCalDone = false;
						}
						if (request->PIN_TYPE == true)
						{
							pinCalDone = false;
						}

						TBLHBUpdateReason reason1;
						reason1.scbRequest = true;

						TBLStatusUpdateReason reason;
						reason.scbRequest = true;

						if (operateMode != ControlOpMode::NRmCal)
						{
							if (!(zMinSet && zMaxSet && longMinSet && longMaxSet && rmCalDone && pinCalDone))
							{
								operateMode = ControlOpMode::NRmCal;
								
								reason.opModeChange = true;
								reason1.calStatusChange = true;
							}
						}
						sendBuffer->Add(GetHeartBeatMsg(reason1));
						sendBuffer->Add(GetStatusMsg(reason));

						canBus->Send(sendBuffer);
					}
				}
				break;

			case 0x700:
				if (protocalData.size != 7)
				{
					break;
				}
			//	Console::WriteLine("SCB TBL Set Room Cal Received on TBL control");
				{
					TBLSetRMCalMsgPS^ request = gcnew TBLSetRMCalMsgPS(protocalData);

					List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
					X_POS = request->X_POS;
					Y_POS = request->Y_POS;
					Z_POS = request->Z_POS;


					switch (request->ORIENTATION)
					{
					case TBLRMOrientation::LeftWall:
						layout = TBLRoomLayout::Layout1;
						break;
					case TBLRMOrientation::BackWall:
						layout = TBLRoomLayout::Layout2;
						break;
					case TBLRMOrientation::RightWall:
						layout = TBLRoomLayout::Layout3;
						break;
					case TBLRMOrientation::FrontWall:
						layout = TBLRoomLayout::Layout4;
						break;
					default:
						break;
					}

					TBLHBUpdateReason reason1;
					reason1.scbRequest = true;
					TBLStatusUpdateReason reason2;
					reason2.scbRequest = true;

					if (zMinSet && zMaxSet && longMinSet && longMaxSet && tblocEst && pinCalDone)
					{
						operateMode = ControlOpMode::MLock;
						reason2.opModeChange = true;	
						sendBuffer->Add(GetStatusMsg(reason2));

						operateMode = ControlOpMode::Idle;
						reason2.opModeChange = true;
						sendBuffer->Add(GetStatusMsg(reason2));

						reason1.calStatusChange = true;
					}
					else
					{
						reason2.opModeChange = false;
						sendBuffer->Add(GetStatusMsg(reason2));
					}
						sendBuffer->Add(GetHeartBeatMsg(reason1));
						canBus->Send(sendBuffer);
				}
				break;

		/*	case 0x701:
				if (protocalData.size > 1)
				{
					break;
				}
				Console::WriteLine("SCB TBL Set Axis Cal Received on TBL control");
				{
					TBLSetAxisCalMsgPS ^ request = gcnew TBLSetAxisCalMsgPS(protocalData);

					if (request->ZMIN_SET)
					{
						zMinSet = true;
					}
					if (request->LONGMIN_SET)
					{
						longMinSet = true;
					}
					if (request->ZMAX_SET)
					{
						zMaxSet = true;
					}
					if (request->LONGMAX_SET)
					{
						longMaxSet = true;
					}
					if (request->PIN_CAL)
					{
						pinCalDone = true;
					}

					List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
					TBLHBUpdateReason reason;
					reason.calStatusChange = true;
					reason.scbRequest = true;
					sendBuffer->Add(GetHeartBeatMsg(reason));
					canBus->Send(sendBuffer);
				}
				break;*/

			case 0x470:
				/*Console::WriteLine("SCB File Transfer Request Received on WSD control");
				{
					SCBFileTranReqMsgPS ^ request = gcnew SCBFileTranReqMsgPS(protocalData);
					List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
					if (request->DOWNLOAD_TARGET == SCBDownloadTarget::WSD && request->TRANSFER_TYPE == SCBTransferType::CalFileUp)
					{
						operateMode = ControlOpMode::MLock;

						if (request->SFCODE == SCBSFCode::TransferStart)
						{
							AxisFileTranRespMsgPS ^ response = gcnew AxisFileTranRespMsgPS();
							response->DOWNLOAD_TARGET = SCBDownloadTarget::WSD;
							response->DOWNLOAD_TYPE = SCBTransferType::CalFileUp;
							response->STATUSCODE = FILETRANSFERACCEPTED;
							sendBuffer->Add(response->GetFrame());

							AxisFileTranReqMsgPS ^ response1 = gcnew AxisFileTranReqMsgPS(ControlType::WSD);
							response1->TRANSFER_SIZE = 512;
							response1->SFCODE = SCBSFCode::TransferStart;
							sendBuffer->Add(response1->GetFrame());

							AxisFileTranCfgMsgPS ^ response2 = gcnew AxisFileTranCfgMsgPS(swCode);
							sendBuffer->Add(response2->GetFrame());
							Console::WriteLine(sendBuffer->Count);
							canBus->Send(sendBuffer);
						}
					}
				}*/
				break;
			case 0x490:
				/*Console::WriteLine("SCB File Transfer Response Received on WSD control");
				{
					if (operateMode == ControlOpMode::MLock)
					{
						SCBFileTranRespMsgPS ^ request = gcnew SCBFileTranRespMsgPS(protocalData);
						List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
						if (request->TRANS_SOURCE == SCBDownloadTarget::WSD && request->TRANS_TYPE == SCBTransferType::CalFileUp)
						{
							if (request->STATUS == FILETRANSFERACCEPTED)
							{
								CtlBLKUPReqMsgPS ^ response = gcnew CtlBLKUPReqMsgPS();
								response->TRANS_SOURCE = SCBDownloadTarget::WSD;
								response->TRANS_SIZE = 256;
								response->CHECKSUM = 0;
								response->BLOCK_SEQNUM = 0;
								currentUpSeqNum = 0;
								currentUpBlockSize = 255;
								lastUpSeqNum = 1;
								sendBuffer->Add(response->GetFrame());
							}
							if (request->STATUS == FILETRANSFERCOMPLETED)
							{
								operateMode = ControlOpMode::Idle;
							}
						}

						canBus->Send(sendBuffer);
					}
				}*/
				break;
			case 0x491:
			/*	Console::WriteLine("SCB Block Upload Response Received on WSD control");
				{
					if (operateMode == ControlOpMode::MLock)
					{
						SCBBLKTranRespMsgPS^ request = gcnew SCBBLKTranRespMsgPS(protocalData);
						List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
						if (request->TRAN_SOURCE == SCBDownloadTarget::WSD)
						{
							if (request->STATUS == FILETRANSFERACCEPTED)
							{
								AxisDataUpMsgPS ^ dataUpMsg = gcnew AxisDataUpMsgPS;
								dataUpMsg->RD_BYTE0 = 0;
								dataUpMsg->RD_BYTE1 = 0;
								dataUpMsg->RD_BYTE2 = 0;
								dataUpMsg->RD_BYTE3 = 0;
								dataUpMsg->RD_BYTE4 = 0;
								dataUpMsg->RD_BYTE5 = 0;
								dataUpMsg->RD_BYTE6 = 0;
								dataUpMsg->RD_BYTE7 = 0;

								AxisCtlProtoStuct sendData = dataUpMsg->GetFrame();

								for (UInt16 i = 0; i < currentUpBlockSize; i++)
								{
									sendBuffer->Add(sendData);
								}
							}

							if (request->STATUS == FILETRANSFERCOMPLETED)
							{
								if (currentUpSeqNum != lastUpSeqNum)
								{
									CtlBLKUPReqMsgPS ^ response = gcnew CtlBLKUPReqMsgPS();
									response->TRANS_SOURCE = SCBDownloadTarget::WSD;
									response->TRANS_SIZE = 256;
									response->CHECKSUM = 0;
									response->BLOCK_SEQNUM = 1;
									currentUpSeqNum = 1;
									currentUpBlockSize = 255;
									sendBuffer->Add(response->GetFrame());
								}
								else
								{
									AxisFileTranReqMsgPS ^ response1 = gcnew AxisFileTranReqMsgPS(ControlType::WSD);
									response1->TRANSFER_SIZE = 512;
									response1->SFCODE = SCBSFCode::TransferFinish;
									sendBuffer->Add(response1->GetFrame());
								}
							}
						}
						canBus->Send(sendBuffer);
					}
				}*/
				break;
			case 0x474:
				/*Console::WriteLine("clear ");*/
				break;

			case 0x458:
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
			case 0x123:
			{
						  List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
						  XYZSyncMoveParaPS^ request = gcnew XYZSyncMoveParaPS(protocalData);
						  delayTime = request->SYNC_DELAY;
						  ltDirection = request->TBLMOVE_DICT;

						  AxisSyncRespMsgPS ^ response = gcnew AxisSyncRespMsgPS(ControlType::TBL);
						  response->ERROR_CODE = 0x00;
						  response->SYNC_STATUS = SyncStatus::SyncACK;
						  sendBuffer->Add(response->GetFrame());
						  canBus->Send(sendBuffer);
						  ltMoveThread = gcnew Thread(gcnew ThreadStart(this, &TBLControllerCLS::LTMove));
						  ltMoveThread->IsBackground = true;
						  ltMoveThread->Start();
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
							  TBLStatusUpdateReason reason;
							  reason.opModeChange = true;
							  sendBuffer->Add(GetStatusMsg(reason));
							  canBus->Send(sendBuffer);

						  }
						  else
						  {
							  waitForBuckyStart->Reset();
							  if (ltMoveThread->IsAlive)
							  {
								  ltMoveThread->Abort();
							  }
							  operateMode = ControlOpMode::Idle;
							  TBLStatusUpdateReason reason;
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

	//send position update to scb
	void TBLControllerCLS::HBUpdateToSCB(TBLHBUpdateReason reason)
	{
		List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
		AxisCtlProtoStuct updateMsg = GetHeartBeatMsg(reason);
		sendBuffer->Add(updateMsg);
		canBus->Send(sendBuffer);
	}

	//update status to scb
	void TBLControllerCLS::StatusUpdateToSCB(TBLStatusUpdateReason reason)
	{
		List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
		AxisCtlProtoStuct updateMsg = GetStatusMsg(reason);
		sendBuffer->Add(updateMsg);
		canBus->Send(sendBuffer);
	}

	//motorized Z movement function
	void TBLControllerCLS::MotoZMove()
	{
		while (true)
		{
			if (operateMode == ControlOpMode::MMReq)
			{
				if (swStatus.zDownPedal)
				{
					Z_POS -= motoMoveStepLength;
				}
				if (swStatus.zUpPedal)
				{
					Z_POS += motoMoveStepLength;
				}
			}
			//the time interval will change if not proper
			Thread::Sleep(100);
		}
	}

	/********************public methods******************/

	//ctor
	TBLControllerCLS::TBLControllerCLS(CanBusChannelCls ^ bus)
	{

		XmlReader ^ reader = XmlReader::Create(System::AppDomain::CurrentDomain->BaseDirectory + "\\firmwareVer.xml");
		while (reader->Read())
		{
			if (reader->NodeType == XmlNodeType::Element)
			{
				String ^ nodeName = reader->Name;
				if (nodeName == "hardwareVersion")
				{
					if (reader->Read())
					{
						hardwareVersion = UINT8::Parse(reader->Value);
					}
				}
				if (nodeName == "bootTagVersion")
				{
					if (reader->Read())
					{
						bootTagVersion = UINT8::Parse(reader->Value);
					}
				}
				if (nodeName == "bootRevision")
				{
					if (reader->Read())
					{
						bootRevision = UINT16::Parse(reader->Value);
					}
				}
				if (nodeName == "appTagVersion")
				{
					if (reader->Read())
					{
						appTagVersion = UINT8::Parse(reader->Value);
					}
				}
				if (nodeName == "appRevision")
				{
					if (reader->Read())
					{
						appRevision = UINT16::Parse(reader->Value);
					}
				}
				if (nodeName == "motoParaTagVersion")
				{
					if (reader->Read())
					{
						motoParaTagVersion = UINT8::Parse(reader->Value);
					}
				}
				if (nodeName == "motoParaRevision")
				{
					if (reader->Read())
					{
						motoParaRevision = UINT16::Parse(reader->Value);
					}
				}
				if (nodeName == "bootStrapTagVersion")
				{
					if (reader->Read())
					{
						bootStrapTagVersion = UINT8::Parse(reader->Value);
					}
				}
				if (nodeName == "bootStrapRevision")
				{
					if (reader->Read())
					{
						bootStrapRevision = UINT16::Parse(reader->Value);
					}
				}

			}
		}
		reader->Close();



		//hardwareVersion = 0x01;
		//bootTagVersion = 0x34;
		//bootRevision = 0x18F8;
		//appTagVersion = 0x34;
		//appRevision = 0x18F8;
		//motoParaTagVersion = 0x34;
		//motoParaRevision = 0x18F8;
		//bootStrapTagVersion = 0x34;
		//bootStrapRevision = 0x18F8;

		swCode = TBLTYPE4;


		axisMoveStepLength = 10;
		motoMoveStepLength = 1;
		canBus = bus;
		tblReceiveQueue = gcnew List<AxisCtlProtoStuct>;

	}

	//detector id 
	int TBLControllerCLS::DET_ID::get()
	{
		return detectorID;
	}

	// position property 
	double TBLControllerCLS::X_POS::get()
	{
		return xPosition;
	}

	void TBLControllerCLS::X_POS::set(double value)
	{
		xPosition = value;
		TBLHBUpdateReason reason;
		reason.movementOccur = true;
		HBUpdateToSCB(reason);
	}

	double TBLControllerCLS::Y_POS::get()
	{
		return yPosition;
	}

	void TBLControllerCLS::Y_POS::set(double value)
	{
		yPosition = value;
		TBLHBUpdateReason reason;
		reason.movementOccur = true;
		HBUpdateToSCB(reason);
	}

	double TBLControllerCLS::Z_POS::get()
	{
		return zPosition;
	}

	void TBLControllerCLS::Z_POS::set(double value)
	{
		zPosition = value;
		TBLHBUpdateReason reason;
		reason.movementOccur = true;
		HBUpdateToSCB(reason);
	}

	ControlOpMode TBLControllerCLS::OP_MODE::get()
	{
		return operateMode;
	}

	double TBLControllerCLS::AXIS_MOVE_STEP::get()
	{
		return axisMoveStepLength;
	}

	void TBLControllerCLS::AXIS_MOVE_STEP::set(double value)
	{
		axisMoveStepLength = value;
	}



	// the switch status property
	bool TBLControllerCLS::AUTOCENTER_SW::get()
	{
		return false; // need to add auto center later ?
	}

	void TBLControllerCLS::AUTOCENTER_SW::set(bool value)
	{
		swStatus.autoCenterSW = value;
		swStatus.autoTrackSW = false;
		swStatus.estopSW = false;
		swStatus.floatPedal = false;
		swStatus.zDownPedal = false;
		swStatus.zUpPedal = false;

		TBLStatusUpdateReason reason;
		reason.swStatusChange = true;

		TBLStatusMsgPS ^ updateMsg = gcnew TBLStatusMsgPS;
		updateMsg->BUCKY_DT = buckyDetent;
		updateMsg->DETECTOR_ID = detectorID;
		updateMsg->INDICATOR_STATUS = indicatorStatus;
		updateMsg->KEYPAD_STATUS = keypadStatus;
		updateMsg->MTCONTROLS = motionControl;
		updateMsg->MTSENSORS = sensors;
		updateMsg->SW_STATUS = swStatus;
		updateMsg->Z_DT = zDetent;

		List<AxisCtlProtoStuct> ^ list = gcnew List<AxisCtlProtoStuct>;
		list->Add(updateMsg->GetFrame());
		canBus->Send(list);

	}

	bool TBLControllerCLS::AUTOTK_SW::get()
	{
		return false; // need to add auto track later?
	}

	void TBLControllerCLS::AUTOTK_SW::set(bool value)
	{
		// need to add auto track later?
	}

	bool TBLControllerCLS::ESTOP_SW::get()
	{
		return false;  // need to add estop?
	}

	void TBLControllerCLS::ESTOP_SW::set(bool value)
	{
		// need to add estop
	}

	bool TBLControllerCLS::FLOAT_SW::get()
	{
		return swStatus.floatPedal;
	}

	void TBLControllerCLS::FLOAT_SW::set(bool value)
	{
		if (value)
		{
			swStatus.floatPedal = true;
			TBLStatusUpdateReason reason;
			reason.swStatusChange = true;
			if (operateMode == ControlOpMode::Idle)
			{
				operateMode = ControlOpMode::MMReq;
				reason.opModeChange = true;
			}
			StatusUpdateToSCB(reason);
		}
		else
		{
			swStatus.floatPedal = false;
			TBLStatusUpdateReason reason;
			reason.swStatusChange = true;
			if ((operateMode == ControlOpMode::MMReq) && (!swStatus.zDownPedal) && (!swStatus.zUpPedal))
			{
				operateMode = ControlOpMode::Idle;
				reason.opModeChange = true;
			}

			StatusUpdateToSCB(reason);
		}
	}


	bool TBLControllerCLS::ZDOWN_SW::get()
	{
		return swStatus.zDownPedal;
	}

	void TBLControllerCLS::ZDOWN_SW::set(bool value)
	{
		if (value)
		{
			swStatus.zDownPedal = true;
			TBLStatusUpdateReason reason;
			reason.swStatusChange = true;
			if (operateMode == ControlOpMode::Idle)
			{
				operateMode = ControlOpMode::MMReq;
				reason.opModeChange = true;
			}
			StatusUpdateToSCB(reason);
		}
		else
		{
			swStatus.zDownPedal = false;
			TBLStatusUpdateReason reason;
			reason.swStatusChange = true;
			if ((operateMode == ControlOpMode::MMReq) && (!swStatus.floatPedal) && (!swStatus.zUpPedal))
			{
				operateMode = ControlOpMode::Idle;
				reason.opModeChange = true;
			}
			StatusUpdateToSCB(reason);
		}
	}

	bool TBLControllerCLS::ZUP_SW::get()
	{
		return swStatus.zUpPedal;
	}

	void TBLControllerCLS::ZUP_SW::set(bool value)
	{
		if (value)
		{
			swStatus.zUpPedal = true;
			TBLStatusUpdateReason reason;
			reason.swStatusChange = true;
			if (operateMode == ControlOpMode::Idle)
			{
				operateMode = ControlOpMode::MMReq;
				reason.opModeChange = true;
			}
			StatusUpdateToSCB(reason);
		}
		else
		{
			swStatus.zUpPedal = false;
			TBLStatusUpdateReason reason;
			reason.swStatusChange = true;
			if ((operateMode == ControlOpMode::MMReq) && (!swStatus.floatPedal) && (!swStatus.zDownPedal))
			{
				operateMode = ControlOpMode::Idle;
				reason.opModeChange = true;
			}
			StatusUpdateToSCB(reason);
		}
	}

	// the Z up button pressed
	void TBLControllerCLS::ZUpBtnDown()
	{
		ZUP_SW = true;
		ZDOWN_SW = false;
	}

	// the Z up button released
	void TBLControllerCLS::ZUpBtnUp()
	{
		ZUP_SW = false;
	}

	// the Z down button pressed
	void TBLControllerCLS::ZDownBtnDown()
	{
		ZDOWN_SW = true;
		ZUP_SW = false;
	}

	// the Z down button released
	void TBLControllerCLS::ZDownBtnUp()
	{
		ZDOWN_SW = false;
	}


	//Press Auto Center Button
	void TBLControllerCLS::AutoCenterBtnDown()
	{
		//need to add
	}

	//Release Auto Center Button
	void TBLControllerCLS::AutoCenterBtnUp()
	{
		//need to add
	}

	//Press Auto Track Button
	void TBLControllerCLS::AutoTKBtnDown()
	{
		//need to add
	}

	//Release Auto Track Button
	void TBLControllerCLS::AutoTKBtnUp()
	{
		//need to add
	}

	//Press Estop Button
	void TBLControllerCLS::EstopBtnDown()
	{
		//need to add
	}

	//Release Estop Button
	void TBLControllerCLS::EstopBtnUp()
	{
		//need to add
	}

	//Press Float Button
	void TBLControllerCLS::FloatBtnDown()
	{
		FLOAT_SW = true;
	}

	//Release Flat Button
	void TBLControllerCLS::FloatBtnUp()
	{
		FLOAT_SW = false;
	}

	//TBL X forward
	void TBLControllerCLS::XStepUp()
	{
		if (FLOAT_SW && (operateMode == ControlOpMode::MMReq))
		{
			X_POS += axisMoveStepLength;
		}
	}

	//TBL X backward
	void TBLControllerCLS::XStepDown()
	{
		if (FLOAT_SW && (operateMode == ControlOpMode::MMReq))
		{
			X_POS -= axisMoveStepLength;
		}
	}

	//TBL Y forward
	void TBLControllerCLS::YStepUp()
	{
		if (FLOAT_SW && (operateMode == ControlOpMode::MMReq))
		{
			Y_POS += axisMoveStepLength;
		}
	}

	//TBL Y backward
	void TBLControllerCLS::YStepDown()
	{
		if (FLOAT_SW && (operateMode == ControlOpMode::MMReq))
		{
			Y_POS -= axisMoveStepLength;
		}
	}

	//data receive event handler
	void TBLControllerCLS::OnCanDataReceivedEvent(List<AxisCtlProtoStuct> ^ receivedQueue)
	{
		Monitor::Enter(receiveLockObject);
		tblReceiveQueue->AddRange(receivedQueue);
		Monitor::Exit(receiveLockObject);
	}



	//can data handler threading function
	void TBLControllerCLS::CanDataHandlerFunc()
	{
		while (true)
		{
			int frameCount = tblReceiveQueue->Count;
			if (frameCount > 0)
			{

				List<AxisCtlProtoStuct> ^ receivedFrameQueue = tblReceiveQueue->GetRange(0, frameCount);
				Monitor::Enter(receiveLockObject);
				tblReceiveQueue->RemoveRange(0, frameCount);
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
	void TBLControllerCLS::HeartBeatFunc()
	{
		while (true)
		{
			TBLHBUpdateReason reason;
			reason.scbRequest = false;
			reason.hbTimeOut = true;
			reason.movementOccur = false;
			AxisCtlProtoStuct msg = GetHeartBeatMsg(reason);
			List<AxisCtlProtoStuct> ^sendBuffer = gcnew List<AxisCtlProtoStuct>;
			sendBuffer->Add(msg);
			canBus->Send(sendBuffer);

		//	Console::WriteLine("TBL Heartbeat");
		//	Console::WriteLine("TBL Mode:" + operateMode.ToString());
			Thread::Sleep(800);
		}
	}

	//Power Up
	void TBLControllerCLS::PowerUp()
	{
		this->RunBootCode();
		this->RunAppCode();
	}

	//Power Off
	void TBLControllerCLS::PowerOff()
	{
		this->ExitAppCode();
	}

	//Insert Detector 
	void TBLControllerCLS::InsertDetector(Byte id)
	{
		detectorID = id;
		List<AxisCtlProtoStuct> ^ updateResponse = gcnew List<AxisCtlProtoStuct>;

		// Controller Status Message
		TBLStatusUpdateReason reason;
		reason.detectorIdCHANGE = true;
	//	reason.opModeChange = false;
	//	reason.mtSensorChange = true;
	//	reason.scbRequest = true;
	//	reason.softDtChange = false;
	//	reason.swStatusChange = false;
	//	reason.xAxisStatusChange = false;

		updateResponse->Add(GetStatusMsg(reason));
		canBus->Send(updateResponse);
	}

	// eject the detector
	void TBLControllerCLS::EjectDetector()
	{
		//detectorType = DetectorType::Undefined;
		detectorID = 0x00;
		//sensors.dtIntable = false;
		//sensors.buckyInPortrait = false;

		List<AxisCtlProtoStuct> ^ updateResponse = gcnew List<AxisCtlProtoStuct>;

		// Controller Status Message
		TBLStatusUpdateReason reason;
		reason.detectorIdCHANGE = true;
		//reason.opModeChange = false;
		//reason.mtSensorChange = true;
		//reason.scbRequest = false;
		//reason.softDtChange = false;
		//reason.swStatusChange = false;
		//reason.xAxisStatusChange = false;

		updateResponse->Add(GetStatusMsg(reason));
		canBus->Send(updateResponse);
	}

	// bucky start signal received event handler
	void TBLControllerCLS::BuckyStartEventHandler()
	{
		if (operateMode == ControlOpMode::LT)
		{
			waitForBuckyStart->Set();
		}
	}

	// grid settings
	void TBLControllerCLS::SetGrid(bool exist)
	{
		if (exist)
		{
			sensors.gridPresent = true;

			List<AxisCtlProtoStuct> ^ updateResponse = gcnew List<AxisCtlProtoStuct>;

			// Controller Status Message
			TBLStatusUpdateReason reason;
			reason.mtSensorChange = true;
			updateResponse->Add(GetStatusMsg(reason));
			canBus->Send(updateResponse);
		}
		else
		{
			sensors.gridPresent = false;

			List<AxisCtlProtoStuct> ^ updateResponse = gcnew List<AxisCtlProtoStuct>;

			// Controller Status Message
			TBLStatusUpdateReason reason;
			reason.mtSensorChange = true;
			updateResponse->Add(GetStatusMsg(reason));
			canBus->Send(updateResponse);
		}
	}

}