// This is the main DLL file.

#include "stdafx.h"

#include "OTCXYZControl.h"


namespace PositionerSimulationNS {

	/*******************************  OTCXYZControllerCLS Implement*******************************/

	/********************private methods******************/
	List<AxisCtlProtoStuct> ^ OTCXYZControllerCLS::GetConfigurationRsp()
	{
		CtlConfigRespPS ^ bootConfigResponse = gcnew CtlConfigRespPS(ControlType::OTCXYZ);
		bootConfigResponse->HARDWARE_VERSION = hardwareVersion;
		bootConfigResponse->BoardType = this->hardwareType;
		bootConfigResponse->MODE = runMode;
		bootConfigResponse->SWITCH_CODE = swCode;
		bootConfigResponse->BOOT_TAG = bootTagVersion;
		bootConfigResponse->BOOT_REV = bootRevision;
		bootConfigResponse->FPGA_TAG = fpgaTagVersion;
		bootConfigResponse->FPGA_REV = fpgaRevision;
		CtlAppVerRspPS^ bootCtlAppResponse = gcnew CtlAppVerRspPS(ControlType::OTCXYZ);
		bootCtlAppResponse->APP_TAG_VER = appTagVersion;
		bootCtlAppResponse->APP_REVISION = appRevision;
		bootCtlAppResponse->MT_TAG_VER = motoParaTagVersion;
		bootCtlAppResponse->MT_PARA_REV = motoParaRevision;
		if (runMode == ControlHWMode::BOOT)
		{
			bootCtlAppResponse->OPTIONS = 0xFF;
		}
		else
		{
			if (withIR == true)
			{
				bootCtlAppResponse->OPTIONS = 0x01;
			}
			else
			{
				bootCtlAppResponse->OPTIONS = 0x00;
			}
		}
		CtlBootStrapVerRspPS^ bootCtlBootStrapResponse = gcnew CtlBootStrapVerRspPS(ControlType::OTCXYZ);
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

	AxisCtlProtoStuct OTCXYZControllerCLS::GetStatusMsg(OTCXYZStaMsgUpdateReason reason)
	{
		OTCXYZStaMsgPS ^ statusUpdateMsg = gcnew OTCXYZStaMsgPS;
		statusUpdateMsg->OP_MODE = operateMode;
		statusUpdateMsg->RES_SW_STA = releaseSW;
		statusUpdateMsg->X_STATUS = xStatus;
		statusUpdateMsg->Y_STATUS = yStatus;
		statusUpdateMsg->Z_STATUS = zStatus;
		statusUpdateMsg->X_DT_STA = xDtStatus;
		statusUpdateMsg->Y_DT_STA = yDtStatus;
		statusUpdateMsg->Z_DT_STA = zDtStatus;
		statusUpdateMsg->ESTOP_SW = auxStatus;

		statusUpdateMsg->UPDATE_REASON = reason;

		return statusUpdateMsg->GetFrame();
	}

	AxisCtlProtoStuct OTCXYZControllerCLS::GetHeartBeatMsg(OTCXYZHBUpdateReason reason)
	{
		OTCXYZHBMsgPS ^ msg = gcnew OTCXYZHBMsgPS;
		msg->PINCAL_DONE = PinCal;
		msg->RMCAL_DONE = RoomCal;
		msg->XMIN_SET = XminSet;
		msg->XMAX_SET = XmaxSet;
		msg->YMIN_SET = YminSet;
		msg->YMAX_SET = YmaxSet;
		msg->ZMIN_SET = ZminSet;
		msg->ZMAX_SET = ZmaxSet;
	//	if (operateMode == ControlOpMode::MMReq || operateMode == ControlOpMode::Idle || operateMode == ControlOpMode::MLock)
		if (operateMode != ControlOpMode::NRmCal)
		{
			msg->X_POSITION = xPosition;
			msg->Y_POSITION = yPosition;
			msg->Z_POSITION = zPosition;
		
		}
		else
		{
			msg->X_POSITION = -8192;
			msg->Y_POSITION = -8192;
			msg->Z_POSITION = -8192;
		}
		msg->UPDATE_REASON = reason;
		return msg->GetFrame();
	}


	//data receive event handler
	void OTCXYZControllerCLS::OnCanDataReceivedEvent(List<AxisCtlProtoStuct> ^ receivedQueue)
	{
		Monitor::Enter(receiveLockObject);
		otcXYZReceiveQueue->AddRange(receivedQueue);
		Monitor::Exit(receiveLockObject);
	}



	//can data handler threading function
	void OTCXYZControllerCLS::CanDataHandlerFunc()
	{
		while (true)
		{
			int frameCount = otcXYZReceiveQueue->Count;
			if (frameCount > 0)
			{

				List<AxisCtlProtoStuct> ^ receivedFrameQueue = otcXYZReceiveQueue->GetRange(0, frameCount);
				Monitor::Enter(receiveLockObject);
				otcXYZReceiveQueue->RemoveRange(0, frameCount);
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
	void OTCXYZControllerCLS::HeartBeatFunc()
	{
		while (true)
		{
			OTCXYZHBUpdateReason reason;
			reason.scbRequest = false;
			reason.hbTimeOut = true;
			reason.axisMove = false;
			AxisCtlProtoStuct msg = GetHeartBeatMsg(reason);
			List<AxisCtlProtoStuct> ^sendBuffer = gcnew List<AxisCtlProtoStuct>;
			sendBuffer->Add(msg);
			canBus->Send(sendBuffer);

			//Console::WriteLine("OTCXYZ Heartbeat");
		//	Console::WriteLine("OTCXYZ Mode:" + operateMode.ToString());
			Thread::Sleep(800);
		}
	}

	// Automove function
	void OTCXYZControllerCLS::AutoMoveFunc()
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
			while (  !xFinish || !yFinish || !zFinish  )
			{

				if (automoveEvent->WaitOne())
				{
					if (!zFinish)
					{
						if (zUp)
						{
							if (zStatus.breakEn)
							{
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								OTCXYZStaMsgUpdateReason reason;
								reason.zStaChange = true;
								reason.scbRequest = true;
								zStatus.mtEn = true;
								zStatus.clutchEn = true;
								zStatus.breakEn = false;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);

							}
							if (zStepCount > 0)
							{
								Z_POS += autoMoveStep;
								zStepCount--;
							}
							if (zStepCount == 0)
							{
								Z_POS += zLastStep;
								zFinish = true;
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								OTCXYZStaMsgUpdateReason reason;
								reason.zStaChange = true;
								reason.scbRequest = true;
								zStatus.mtEn = false;
								zStatus.clutchEn = false;
								zStatus.breakEn = true;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);
							}

						}
						else
						{
							if (zStatus.breakEn)
							{
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								OTCXYZStaMsgUpdateReason reason;
								reason.zStaChange = true;
								reason.scbRequest = true;
								zStatus.mtEn = true;
								zStatus.clutchEn = true;
								zStatus.breakEn = false;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);

							}
							if (zStepCount > 0)
							{
								Z_POS -= autoMoveStep;
								zStepCount--;
							}
							if (zStepCount == 0)
							{
								Z_POS -= zLastStep;
								zFinish = true;
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								OTCXYZStaMsgUpdateReason reason;
								reason.zStaChange = true;
								reason.scbRequest = true;
								zStatus.mtEn = false;
								zStatus.clutchEn = false;
								zStatus.breakEn = true;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);
							}
						}
					}

					if (zFinish && (!yFinish))
					{
						if (yUp)
						{
							if (yStatus.breakEn)
							{
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								OTCXYZStaMsgUpdateReason reason;
								reason.yStaChange = true;
								reason.scbRequest = true;
								yStatus.mtEn = true;
								yStatus.clutchEn = true;
								yStatus.breakEn = false;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);

							}
							if (yStepCount > 0)
							{
								Y_POS += autoMoveStep;
								yStepCount--;
							}
							if (yStepCount == 0)
							{
								Y_POS += yLastStep;
								yFinish = true;
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								OTCXYZStaMsgUpdateReason reason;
								reason.yStaChange = true;
								reason.scbRequest = true;
								yStatus.mtEn = false;
								yStatus.clutchEn = false;
								yStatus.breakEn = true;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);
							}
						}
						else
						{
							if (yStatus.breakEn)
							{
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								OTCXYZStaMsgUpdateReason reason;
								reason.yStaChange = true;
								reason.scbRequest = true;
								yStatus.mtEn = true;
								yStatus.clutchEn = true;
								yStatus.breakEn = false;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);

							}
							if (yStepCount > 0)
							{
								Y_POS -= autoMoveStep;
								yStepCount--;
							}
							if (yStepCount == 0)
							{
								Y_POS -= yLastStep;
								yFinish = true;
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								OTCXYZStaMsgUpdateReason reason;
								reason.yStaChange = true;
								reason.scbRequest = true;
								yStatus.mtEn = false;
								yStatus.clutchEn = false;
								yStatus.breakEn = true;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);
							}
						} // end of yUp
					}// end of zFinish

					if (zFinish && yFinish && (!xFinish))
					{
						if (xUp)
						{
							if (xStatus.breakEn)
							{
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								OTCXYZStaMsgUpdateReason reason;
								reason.xStaChange = true;
								reason.scbRequest = true;
								xStatus.mtEn = true;
								xStatus.clutchEn = true;
								xStatus.breakEn = false;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);

							}
							if (xStepCount > 0)
							{
								X_POS += autoMoveStep;
								xStepCount--;
							}
							if (xStepCount == 0)
							{
								X_POS += xLastStep;
								xFinish = true;
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								OTCXYZStaMsgUpdateReason reason;
								reason.xStaChange = true;
								reason.scbRequest = true;
								xStatus.mtEn = false;
								xStatus.clutchEn = false;
								xStatus.breakEn = true;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);
							}
						}
						else
						{
							if (xStatus.breakEn)
							{
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								OTCXYZStaMsgUpdateReason reason;
								reason.xStaChange = true;
								reason.scbRequest = true;
								xStatus.mtEn = true;
								xStatus.clutchEn = true;
								xStatus.breakEn = false;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);

							}
							if (xStepCount > 0)
							{
								X_POS -= autoMoveStep;
								xStepCount--;
							}
							if (xStepCount == 0)
							{
								X_POS -= xLastStep;
								xFinish = true;
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								OTCXYZStaMsgUpdateReason reason;
								reason.xStaChange = true;
								reason.scbRequest = true;
								xStatus.mtEn = false;
								xStatus.clutchEn = false;
								xStatus.breakEn = true;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);
							}
						} // end of xUp if
					}// end of z/y finish if
				} // end of if automoveEvent
			}// end of while
		}// end of if autoContinueNeed

		// send the finish response to SCB and controllers
		AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::OTCXYZ);
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

			OTCXYZStaMsgUpdateReason reason;
			reason.scbRequest = true;
			reason.OpModeChange = true;
			sendbuffer->Add(GetStatusMsg(reason));
			canBus->Send(sendbuffer);
		}
	//	ExitAutoMove();
		ABAutoMoveFlag = WSDAutoMoveFlag = TBLAutoMoveFlag = false;
		ABAutoFinishFlag = WSDAutoFinishFlag = TBLAutoFinishFlag = false;
	
		return;

	}// end of auto move function

	void OTCXYZControllerCLS::RunBootCode()
	{
		runMode = ControlHWMode::BOOT;
		Console::WriteLine("OTC XYZ booting ....");
		
		List<AxisCtlProtoStuct> ^ configResponse = GetConfigurationRsp();
		canBus->Send(configResponse);
	}

	void OTCXYZControllerCLS::RunAppCode()
	{
		runMode = ControlHWMode::APP;
		Console::WriteLine("OTC XYZ APP initalizing ....");

		//calibrate the OTC XYZ
		xPosMinLim = -500; xPosMaxLim = 2172;
		yPosMinLim = -100; yPosMaxLim = 1649;
		zPosMinLim = -300; zPosMaxLim = 1863;


		XminSet = YminSet = ZminSet = XmaxSet = YmaxSet = ZmaxSet = RoomCal = PinCal = true;

		//set the xyz position
		xPosition = 839;
		yPosition = 416;
		zPosition = 1404;
		xmotorPos = xPosition;
		ymotorPos = yPosition;
		zmotorPos = zPosition;

		xdevicePos = xPosition;
		ydevicePos = yPosition;
		zdevicePos = zPosition;

		xabsEncoder = xPosition;
		yabsEncoder = yPosition;
		zabsEncoder = zPosition;

		xserErr = 0;
		yserErr = 0;
		zserErr = 0;
		xpot = 0;
		ypot = 0;
		zpot = 0;
		xcurrent = 0.25;
		ycurrent = 0;
		zcurrent = 0;
		xvoltage = 1;
		yvoltage = 0;
		zvoltage = 0;
		xvel = 0;
		yvel = 0;
		zvel = 0;

		xlliPos = xPosition;
		ylliPos = yPosition;
		zlliPos = zPosition;

		//brake the axis
		xStatus.breakEn = yStatus.breakEn = zStatus.breakEn = true;
		xStatus.clutchEn = yStatus.clutchEn =zStatus.clutchEn = false;
		xStatus.mtEn = yStatus.mtEn = zStatus.mtEn = false;
		operateMode = ControlOpMode::Idle;
		Console::WriteLine("OTC XYZ is now in IDLE mode");
		List<AxisCtlProtoStuct> ^ initResponse = GetConfigurationRsp();

		OTCXYZStaMsgUpdateReason reason;
		reason.auxSWChange = reason.OpModeChange = reason.relSWChange = reason.xStaChange = reason.yStaChange = reason.zStaChange = true;
		reason.softDTChange =  reason.scbRequest =  false;
	
		initResponse->Add(GetStatusMsg(reason));

		OTCXYZHBUpdateReason heartbeatReason;
		heartbeatReason.axisMove = true;
		heartbeatReason.CalStaChange = true;
		heartbeatReason.scbRequest = true;
		initResponse->Add(GetHeartBeatMsg(heartbeatReason));

		CtlErrStatRespMsgPS ^ errorResponse = gcnew CtlErrStatRespMsgPS(ControlType::OTCXYZ);
		errorResponse->ERROR_CODE = 0;
		errorResponse->UPDATEREASON = CtlErrStatRespUpdateReason::PeriodicUpdate;
		initResponse->Add(errorResponse->GetFrame());

		//App init update
		canBus->Send(initResponse);

		//Start to handle SCB/other controllers' message
		canDataHandlerThreadStart = gcnew ThreadStart(this, &OTCXYZControllerCLS::CanDataHandlerFunc);
		canDataHanlerThread = gcnew Thread(canDataHandlerThreadStart);

		heartBeatThreadStart = gcnew ThreadStart(this, &OTCXYZControllerCLS::HeartBeatFunc);
		heartBeatThread = gcnew Thread(heartBeatThreadStart);
		
		canDataHanlerThread->IsBackground = true;
		heartBeatThread->IsBackground = true;

		canBus->CanDataReceivedEvent += gcnew CanBusChannelNS::CanDataReceivedDelegate(this, &OTCXYZControllerCLS::OnCanDataReceivedEvent);
		canDataHanlerThread->Start();
		heartBeatThread->Start();
	}

	void OTCXYZControllerCLS::ExitAppCode()
	{
		canBus->CanDataReceivedEvent -= gcnew CanBusChannelNS::CanDataReceivedDelegate(this, &OTCXYZControllerCLS::OnCanDataReceivedEvent);

		heartBeatThread->Abort();
		canDataHanlerThread->Abort();

		runMode = ControlHWMode::BOOT;
		operateMode = ControlOpMode::NoInit;

		xStatus.breakEn = xStatus.clutchEn = xStatus.mtEn = false;
		yStatus.breakEn = yStatus.clutchEn = yStatus.mtEn = false;
		zStatus.breakEn = zStatus.clutchEn = zStatus.mtEn = false;

		XminSet = YminSet = ZminSet = XmaxSet = YmaxSet = ZmaxSet = RoomCal = PinCal = true;

		xDtStatus = yDtStatus = zDtStatus = 0;
		releaseSW.aPressed = releaseSW.bPressed = releaseSW.dtSwPressed = releaseSW.xPressed = releaseSW.xyzPressed = releaseSW.yPressed = releaseSW.zPressed = false;
		auxStatus.conAutoPosEn = auxStatus.conAutoPosIdOn = auxStatus.conEstopEn = auxStatus.otcEstopEn = false;

		otcXYZReceiveQueue->Clear();
	}

	void OTCXYZControllerCLS::ExitAutoMove()
	{
		if (moveFinishEvent->WaitOne())
		{
			List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;
			operateMode = ControlOpMode::Idle;
			OTCXYZStaMsgUpdateReason reason;
			reason.OpModeChange = true;
			reason.scbRequest = true;
			sendbuffer->Add(GetStatusMsg(reason));
			canBus->Send(sendbuffer);

			ABAutoMoveFlag = WSDAutoMoveFlag = TBLAutoMoveFlag = false;
			ABAutoFinishFlag = WSDAutoFinishFlag = TBLAutoFinishFlag = false;
		}
	}

	void OTCXYZControllerCLS::ProcessProtocalData(List<AxisCtlProtoStuct> ^ dataQueue)
	{
		//Console::WriteLine("OTC XYZ GET:" + dataQueue->Capacity+" at " + processCount++);
	
		for each(AxisCtlProtoStuct protocalData in dataQueue)
		{
			/*if (protocalData.id != 0x461 && protocalData.id != 0x410 && protocalData.id != 0x210 && protocalData.id != 0x310 && protocalData.id != 0x300 && protocalData.id != 0x310 && protocalData.id != 0x220 && protocalData.id != 0x320)
			{
				Console::WriteLine("{0:x} on OTCXYZ", protocalData.id);
			}*/
			switch (protocalData.id)
			{
			case 0x461:
				//Console::WriteLine("SCB Configuration Request Received");
				{
					SCBConfigReqPS ^ request = gcnew SCBConfigReqPS(protocalData);
					if (request->OTCXYZ_REQ == true)
					{
						List<AxisCtlProtoStuct> ^ otcxyzConfigMsg = GetConfigurationRsp();
						canBus->Send(otcxyzConfigMsg);
					}
				}
				break;
			case 0x410:
				//Console::WriteLine("SCB Controller Status Request Received");
				{
					SCBCtrlStatusReqPS^ request = gcnew SCBCtrlStatusReqPS(protocalData);
					if (request->OTCXYZ_RT == true)
					{
						List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
						if (request->ERRSTA_REQ == true)
						{
							CtlErrStatRespMsgPS ^ response = gcnew CtlErrStatRespMsgPS(ControlType::OTCXYZ);
							response->ERROR_CODE = 0;
							response->UPDATEREASON = CtlErrStatRespUpdateReason::SCBRequest;
							sendBuffer->Add(response->GetFrame());
						}
						if (request->POSUPDATE_REQ == true)
						{
							OTCXYZHBUpdateReason reason;
							reason.scbRequest = true;
							sendBuffer->Add(GetHeartBeatMsg(reason));
						}
						if (request->CONTSTA_REQ == true)
						{
							OTCXYZStaMsgUpdateReason reason;
							reason.scbRequest = true;
							sendBuffer->Add(GetStatusMsg(reason));
						}

						if (request->KEYSTA_REQ == true)
						{
						/*	OTCIRStaMsgPS ^ response = gcnew OTCIRStaMsgPS;
							IRStaUpdateReason updateReason;
							updateReason.scbRequest = true;
							response->UPDATE_REASON = updateReason;
							sendBuffer->Add(response->GetFrame());*/
						}

						if (request->LLIPOS_REQ == true)
						{
							OTCLLIStatusMsgPS ^ response = gcnew OTCLLIStatusMsgPS;
							response->ZTRAV_END = zPosMaxLim;
							response->ZTRAV_START = zPosMinLim;
							response->XYTRAV_START = xPosMinLim;
							response->XYTRAV_END = xPosMaxLim;
							sendBuffer->Add(response->GetFrame());
						}

						canBus->Send(sendBuffer);
					}
				}
				break;

				/*case 0x422:
					Console::WriteLine("SCB Cal Clear Received");
					{
					SCBClrCalPS^ request = gcnew SCBClrCalPS(protocalData);
					if (request->OTC_CLEAR == true)
					{
					List<AxisCtlProtoStuct> ^ sendBUffer = gcnew List<AxisCtlProtoStuct>;
					if (request->PIN_TYPE == true)
					{
					PinCal = false;
					}
					if (request->CAL_TYPE1 == true)
					{
					ZminSet = false;
					}
					if (request->CAL_TYPE2 == true)
					{
					XminSet = false;
					}
					if (request->CAL_TYPE3 == true)
					{
					YminSet = false;
					}
					if (request->CAL_TYPE4 == true)
					{
					ZmaxSet = false;
					}
					if (request->CAL_TYPE5 == true)
					{
					XmaxSet = false;
					}
					if (request->CAL_TYPE6 == true)
					{
					YmaxSet = false;
					}
					if (request->CAL_TYPE7 == true)
					{
					RoomCal = false;
					}

					if (!(XmaxSet&&YmaxSet&&ZmaxSet&&XminSet&&YminSet&&ZminSet&&PinCal&&RoomCal))
					{
					operateMode = ControlOpMode::NRmCal;
					OTCXYZStaMsgUpdateReason reason1;
					reason1.OpModeChange = true;
					StatusUpdateToSCB(reason1);
					OTCXYZHBUpdateReason reason2;
					reason2.CalStaChange = true;
					HBUpdateToSCB(reason2);
					}
					}
					}
					break;

					case */
			case 0x470:
			case 0x474:
				//Console::WriteLine("OK GET");
				break;

			case 0x2E1:
			{
						  AutoMoveRespMsgPS ^ abResponse = gcnew AutoMoveRespMsgPS(protocalData);
						  if (abResponse->AUTOMOVE_STATUS == AMoveStatus::AModeAccepted)
						  {
							  ABAutoMoveFlag = true;
						  }
						  if ((abResponse->AUTOMOVE_STATUS == AMoveStatus::Complete || abResponse->AUTOMOVE_STATUS == AMoveStatus::Abort || abResponse->AUTOMOVE_STATUS == AMoveStatus::Error) &&ABAutoMoveFlag)
						  {
							  ABAutoFinishFlag = true;
							//  Console::WriteLine("AB Finish!");

							  if ((ABAutoFinishFlag == ABAutoMoveFlag) && (WSDAutoMoveFlag == WSDAutoFinishFlag) && (TBLAutoMoveFlag == TBLAutoFinishFlag))
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
						  if ((wsdResponse->AUTOMOVE_STATUS == AMoveStatus::Complete || wsdResponse->AUTOMOVE_STATUS == AMoveStatus::Abort || wsdResponse->AUTOMOVE_STATUS == AMoveStatus::Error)&& WSDAutoMoveFlag)
						  {
							  WSDAutoFinishFlag = true;
							//  Console::WriteLine("WSD Finish!");

							  if ((ABAutoFinishFlag == ABAutoMoveFlag) && (WSDAutoMoveFlag == WSDAutoFinishFlag) && (TBLAutoMoveFlag == TBLAutoFinishFlag))
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
							//  Console::WriteLine("TBL Finish!");

							  if ((ABAutoFinishFlag == ABAutoMoveFlag) && (WSDAutoMoveFlag == WSDAutoFinishFlag) && (TBLAutoMoveFlag == TBLAutoFinishFlag))
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
			//			  AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::OTCXYZ);
			//			  response->AUTOMOVE_STATUS = AMoveStatus::AModeAccepted;
			//			  response->SW_SETTING = swCode;
			//			  sendbuffer->Add(response->GetFrame());


			//			  // change opmode to AMTP
			//			  operateMode = ControlOpMode::AMTP;
			//			  OTCXYZStaMsgUpdateReason reason;
			//			  reason.OpModeChange = true;
			//			  reason.scbRequest = true;
			//			  sendbuffer->Add(GetStatusMsg(reason));

			//			  // send on bus
			//			  canBus->Send(sendbuffer);
			//}
			//	break;

			// the scb to otc xyz destination set
			case 0x400:
				Console::WriteLine("OTC XYZ Auto Move Destination received");
				{
					SCBXYZSetAutoDesMsgPS ^ request = gcnew SCBXYZSetAutoDesMsgPS(protocalData);
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

						// change opmode to AMTP
						operateMode = ControlOpMode::AMTP;
						OTCXYZStaMsgUpdateReason reason;
						reason.OpModeChange = true;
						reason.scbRequest = true;
						xStatus.mtEn = true;
						yStatus.mtEn = true;
						zStatus.mtEn = true;
						
						sendbuffer->Add(GetStatusMsg(reason));
						canBus->Send(sendbuffer);

						// Accept the destination
						AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::OTCXYZ);
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
						  ABAutoMoveFlag = WSDAutoMoveFlag = TBLAutoMoveFlag = false;
						  ABAutoFinishFlag = WSDAutoFinishFlag = TBLAutoFinishFlag = false;

						  List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

						  // send abort response to SCB
						  AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::OTCXYZ);
						  response->AUTOMOVE_STATUS = AMoveStatus::Abort;
						  response->SW_SETTING = swCode;
						  sendbuffer->Add(response->GetFrame());

						  //change status to idle
						  if (operateMode == ControlOpMode::AMTP)
						  {
							  operateMode = ControlOpMode::Idle;
							  OTCXYZStaMsgUpdateReason reason;
							  reason.OpModeChange = true;
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
							  AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::OTCXYZ);
							  response->AUTOMOVE_STATUS = AMoveStatus::EnAccepted;
							  response->SW_SETTING = swCode;
							  sendbuffer->Add(response->GetFrame());
							  canBus->Send(sendbuffer);

							  autoMoveThread = gcnew Thread(gcnew ThreadStart(this, &OTCXYZControllerCLS::AutoMoveFunc));
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
								  AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::OTCXYZ);
								  response->AUTOMOVE_STATUS = AMoveStatus::Complete;
								  response->SW_SETTING = swCode;
								  sendbuffer->Add(response->GetFrame());
								  canBus->Send(sendbuffer);
								
								  // wait for other controllers' finish flag and update status
								  Thread ^ exitAutomoveThread = gcnew Thread(gcnew ThreadStart(this, &OTCXYZControllerCLS::ExitAutoMove));
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
			
			case 0x455:
				if (protocalData.frameData[0] = 0x01)
				{
					cmdLength = protocalData.frameData[1]-3;
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

			/* For Linear Tomo */
			case 0x120:
				{
						  Thread::Sleep(delayTime + sweepTime);
						  List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
						  AxisSyncRespMsgPS ^ response2 = gcnew AxisSyncRespMsgPS(ControlType::OTCXYZ);
						  response2->ERROR_CODE = 0x00;
						  response2->SYNC_STATUS = SyncStatus::SyncComplete;
						  sendBuffer->Clear();
						  sendBuffer->Add(response2->GetFrame());
						  canBus->Send(sendBuffer);
				}
				break;

			case 0x124:
			//	Console::WriteLine("{0}", protocalData.id);
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
						OTCXYZStaMsgUpdateReason reason;
						reason.OpModeChange = true;
						sendBuffer->Add(GetStatusMsg(reason));
						canBus->Send(sendBuffer);

					

						XYZSyncMoveParaPS ^ response = gcnew XYZSyncMoveParaPS;
						if (delayTime > 0)
						{
							response->SYNC_DELAY = delayTime;
						}
						else
						{
							delayTime = 3000;
							response->SYNC_DELAY = delayTime;
						}
						
						response->TBLMOVE_DICT = TBLMoveDict::LTR;
						ltDirection = TBLMoveDict::LTR;
						sendBuffer->Clear();
						sendBuffer->Add(response->GetFrame());
						canBus->Send(sendBuffer);

						AxisSyncRespMsgPS ^ response2 = gcnew AxisSyncRespMsgPS(ControlType::OTCXYZ);
						response2->ERROR_CODE = 0x00;
						response2->SYNC_STATUS = SyncStatus::SyncACK;
						sendBuffer->Clear();
						sendBuffer->Add(response2->GetFrame());
						canBus->Send(sendBuffer);

					}
					else
					{
						operateMode = ControlOpMode::Idle;
						OTCXYZStaMsgUpdateReason reason;
						reason.OpModeChange = true;
						sendBuffer->Add(GetStatusMsg(reason));
						canBus->Send(sendBuffer);
					}
						
				}

				break;

			case 0x46A:
			//	Console::WriteLine("{0}", protocalData.id);
				break;
			default:
				break;
			}
		}
	}


	// convert the diagnostic command response string to can frames list
	List<AxisCtlProtoStuct>^ OTCXYZControllerCLS::ConvertDiagResponse(String^ response)
	{
		int i = 2;
		List<AxisCtlProtoStuct> ^ responseFrameList = gcnew List<AxisCtlProtoStuct>;
		AxisCtlProtoStuct tempFrame;
		tempFrame.id = 0x2C5;
		tempFrame.frameData = gcnew array<Byte>(8);
		tempFrame.frameData[0] = 0x01;
		tempFrame.frameData[1] = response->Length + 3;
		for each (  char c in response)
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
	void OTCXYZControllerCLS::RemoteDiagHandler()
	{
		if (cmdLength > 0)
		{
			String ^ test = gcnew String(diagCMD);
			if (test->Equals("u?v"))
			{
				String ^responseStr = "C?v Sensor5V=1 I2CPwr=1 PotV=1 Cbd1Pwr=1 Cbd2Pwr=1 Cbd3Pwr=1";
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
			    canBus->Send(frameList);
			}
			if (test->Equals("U:0?d"))
			{
				String ^responseStr = "C:0?d p=" + (xPosition * 4).ToString() + " vel=" + xvel.ToString() + " err=" + (xserErr * 4).ToString() + " mp=" + (xmotorPos * 4).ToString() + " dp=" + (xdevicePos * 4).ToString() + " aep=" + (xabsEncoder * 4).ToString() + " pot=" + (xpot * 4).ToString() + " v=" + (xvoltage * 4).ToString() + " i=" + (xcurrent * 256).ToString(); // +" Lp="+(xlliPos*1000).ToString();
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
				canBus->Send(frameList);
			}
			if (test->Equals("U:1?d"))
			{
				String ^responseStr = "C:0?d p=" + (yPosition * 4).ToString() + " vel=" + yvel.ToString() + " err=" + (yserErr * 4).ToString() + " mp=" + (ymotorPos * 4).ToString() + " dp=" + (ydevicePos * 4).ToString() + " aep=" + (yabsEncoder * 4).ToString() + " pot=" + (ypot * 4).ToString() + " v=" + (yvoltage * 4).ToString() + " i=" + (ycurrent * 256).ToString();// +" Lp=" + (ylliPos * 1000).ToString();
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
				canBus->Send(frameList);
			}
			if (test->Equals("U:2?d"))
			{
				String ^responseStr = "C:0?d p=" + (zPosition * 4).ToString() + " vel=" + zvel.ToString() + " err=" + (zserErr * 4).ToString() + " mp=" + (zmotorPos * 4).ToString() + " dp=" + (zdevicePos * 4).ToString() + " aep=" + (zabsEncoder * 4).ToString() + " pot=" + (zpot * 4).ToString() + " v=" + (zvoltage * 4).ToString() + " i=" + (zcurrent * 256).ToString();// +" Lp=" + (zlliPos * 1000).ToString();
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
				canBus->Send(frameList);
			}
			if (test->Equals("U:0?L"))
			{
				String ^responseStr = "C:0?l st="+(xPosMinLim*4).ToString()+" et="+(xPosMaxLim*4).ToString();
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
				canBus->Send(frameList);
			}
			if (test->Equals("U:1?L"))
			{
				String ^responseStr = "C:0?l st=" + (yPosMinLim * 4).ToString() + " et=" + (yPosMaxLim * 4).ToString();
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
				canBus->Send(frameList);
			}
			if (test->Equals("U:2?L"))
			{
				String ^responseStr = "C:0?l st=" + (zPosMinLim * 4).ToString() + " et=" + (zPosMaxLim * 4).ToString();
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
				canBus->Send(frameList);
			}
			
		}
		receivedCMD = 0;
		cmdLength = 0;
		diagCMD = nullptr;
	}

	//send position update to scb
	void OTCXYZControllerCLS::HBUpdateToSCB(OTCXYZHBUpdateReason reason)
	{
		List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
		AxisCtlProtoStuct updateMsg = GetHeartBeatMsg(reason);
		sendBuffer->Add(updateMsg);
		canBus->Send(sendBuffer);
	}

	//send staus update to scb
	void OTCXYZControllerCLS::StatusUpdateToSCB(OTCXYZStaMsgUpdateReason reason)
	{
		List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
		AxisCtlProtoStuct updateMsg = GetStatusMsg(reason);
		sendBuffer->Add(updateMsg);
		canBus->Send(sendBuffer);
	}










	/********************public methods******************/

	//ctor
	OTCXYZControllerCLS::OTCXYZControllerCLS(CanBusChannelCls ^ bus)
	{
		hardwareVersion = 0x01;
		bootTagVersion = 0x34;
		bootRevision = 0x18F8;
		fpgaTagVersion = 0x01;
		fpgaRevision = 0x0005;
		appTagVersion = 0x34;
		appRevision = 0x18F8;
		motoParaTagVersion = 0x34;
		motoParaRevision = 0x18F8;
		bootStrapTagVersion = 0x34;
		bootStrapRevision = 0x18F8;

		swCode = OTCTYPE3;
		withIR = false;

		xStatus.breakEn = xStatus.clutchEn = xStatus.mtEn = false;
		yStatus.breakEn = yStatus.clutchEn = yStatus.mtEn = false;
		zStatus.breakEn = zStatus.clutchEn = zStatus.mtEn = false;

		xDtStatus = yDtStatus = zDtStatus = 0;

		moveStepLength = 10;

		releaseSW.aPressed = releaseSW.bPressed = releaseSW.dtSwPressed = releaseSW.xPressed = releaseSW.xyzPressed = releaseSW.yPressed = releaseSW.zPressed = false;
		auxStatus.conAutoPosEn = auxStatus.conAutoPosIdOn = auxStatus.conEstopEn = auxStatus.otcEstopEn =false;

		canBus = bus;
		otcXYZReceiveQueue = gcnew List<AxisCtlProtoStuct>;

		abControl = gcnew OTCABControllerCLS(bus);

	}

	// x y z position property 
	double OTCXYZControllerCLS::X_POS::get()
	{
		return xPosition;
	}

	void OTCXYZControllerCLS::X_POS::set(double value)
	{
		xPosition = value;
		OTCXYZHBUpdateReason reason;
		reason.axisMove = true;
		HBUpdateToSCB(reason);
		PositionUpdateEvent();
	}

	double OTCXYZControllerCLS::Y_POS::get()
	{
		return yPosition;

	}

	void OTCXYZControllerCLS::Y_POS::set(double value)
	{
		yPosition = value;
		OTCXYZHBUpdateReason reason;
		reason.axisMove = true;
		HBUpdateToSCB(reason);
		PositionUpdateEvent();
	}

	double OTCXYZControllerCLS::Z_POS::get()
	{
		return zPosition;
	}

	void OTCXYZControllerCLS::Z_POS::set(double value)
	{
		zPosition = value;
		OTCXYZHBUpdateReason reason;
		reason.axisMove = true;
		HBUpdateToSCB(reason);
		PositionUpdateEvent();
	}

	double OTCXYZControllerCLS::A_POS::get()
	{
		return abControl->A_POS;
	}

	void OTCXYZControllerCLS::A_POS::set(double value)
	{
		abControl->A_POS = value;
	}

	double OTCXYZControllerCLS::B_POS::get()
	{
		return abControl->B_POS;
	}

	void OTCXYZControllerCLS::B_POS::set(double value)
	{
		abControl->B_POS = value;
	}

	ControlOpMode OTCXYZControllerCLS::OP_MODE::get()
	{
		return operateMode;
	}

	ControlOpMode OTCXYZControllerCLS::AB_OPMODE::get()
	{
		return abControl->OP_MODE;
	}


	void OTCXYZControllerCLS::XStepUp()
	{
		if (releaseSW.xPressed == true || releaseSW.xyzPressed == true)
		{
			X_POS += moveStepLength;
		}
		
	}

	void OTCXYZControllerCLS::XStepDown()
	{
		if (releaseSW.xPressed == true || releaseSW.xyzPressed == true)
		{
			X_POS -= moveStepLength;
		}
	
	}

	void OTCXYZControllerCLS::YStepUp()
	{
		if (releaseSW.yPressed == true || releaseSW.xyzPressed == true)
		{
			Y_POS += moveStepLength;
		}	
	}

	void OTCXYZControllerCLS::YStepDown()
	{
		if (releaseSW.yPressed == true || releaseSW.xyzPressed == true)
		{
			Y_POS -= moveStepLength;
		}
	}

	void OTCXYZControllerCLS::ZStepUp()
	{
		if (releaseSW.zPressed == true || releaseSW.xyzPressed == true)
		{
			Z_POS += moveStepLength;
		}
	}

	void OTCXYZControllerCLS::ZStepDown()
	{
		if (releaseSW.zPressed == true || releaseSW.xyzPressed == true)
		{
			Z_POS -= moveStepLength;
		}
	}

	void OTCXYZControllerCLS::AStepUp()
	{
		if (releaseSW.aPressed == true)
		{
			abControl->A_POS = (abControl->MOVE_STEP*10+abControl->A_POS*10)/10;
		}
	}

	void OTCXYZControllerCLS::AStepDown()
	{
		if (releaseSW.aPressed == true)
		{
			abControl->A_POS = (abControl->A_POS * 10 - abControl->MOVE_STEP * 10) / 10;
			
		}
	}

	void OTCXYZControllerCLS::BStepUp()
	{
		if (releaseSW.bPressed == true)
		{
			abControl->B_POS = (abControl->B_POS*10 + abControl->MOVE_STEP*10)/10;
		}
	}

	void OTCXYZControllerCLS::BStepDown()
	{
		if (releaseSW.bPressed == true)
		{
			abControl->B_POS =( abControl->B_POS*10 - abControl->MOVE_STEP*10)/10;
		}
	}

	void OTCXYZControllerCLS::XRelBtnPress()
	{
		// hardware button should be not pressed if pressed
		if (releaseSW.xPressed)
		{
			return;
		}
		releaseSW.xPressed = true;
		xStatus.breakEn = false;
		OTCXYZStaMsgUpdateReason reason;
		reason.relSWChange = true;
		reason.xStaChange = true;
		if (operateMode == ControlOpMode::Idle)
		{
			operateMode = ControlOpMode::MMReq;
			reason.OpModeChange = true;
		}
		StatusUpdateToSCB(reason);
	}

	void OTCXYZControllerCLS::XRelBtnRelease()
	{
		// hardware button should be not released if released already
		if (!releaseSW.xPressed)
		{
			return;
		}
		releaseSW.xPressed = false;
		xStatus.breakEn = true;
		OTCXYZStaMsgUpdateReason reason;
		reason.relSWChange = true;
		reason.xStaChange = true;
		if ((operateMode == ControlOpMode::MMReq) && (!releaseSW.xyzPressed) && (!releaseSW.yPressed )&& (!releaseSW.zPressed))
		{
			operateMode = ControlOpMode::Idle;
			reason.OpModeChange = true;
		}
		
		StatusUpdateToSCB(reason);
	}

	void OTCXYZControllerCLS::YRelBtnPress()
	{
		//hardware button 
		if (releaseSW.yPressed)
		{
			return;
		}
		releaseSW.yPressed = true;
		OTCXYZStaMsgUpdateReason reason;
		reason.relSWChange = true;
		reason.yStaChange = true;
		if (operateMode == ControlOpMode::Idle)
		{
			operateMode = ControlOpMode::MMReq;
			reason.OpModeChange = true;
		}
		StatusUpdateToSCB(reason);
	}

	void OTCXYZControllerCLS::YRelBtnRelease()
	{
		if (!releaseSW.yPressed)
		{
			return;
		}
		releaseSW.yPressed = false;
		OTCXYZStaMsgUpdateReason reason;
		reason.relSWChange = true;
		reason.yStaChange = true;
		if ((operateMode == ControlOpMode::MMReq) && (!releaseSW.xPressed) && (!releaseSW.zPressed) &&(!releaseSW.xyzPressed))
		{
			operateMode = ControlOpMode::Idle;
			reason.OpModeChange = true;
		}
		StatusUpdateToSCB(reason);
	}

	void OTCXYZControllerCLS::ZRelBtnPress()
	{
		// hardware button should be not pressed if pressed
		if (releaseSW.zPressed)
		{
			return;
		}
		releaseSW.zPressed = true;
		zStatus.breakEn = false;
		OTCXYZStaMsgUpdateReason reason;
		reason.relSWChange = true;
		reason.zStaChange = true;
		if (operateMode == ControlOpMode::Idle)
		{
			operateMode = ControlOpMode::MMReq;
			reason.OpModeChange = true;
		}
		StatusUpdateToSCB(reason);
	}

	void OTCXYZControllerCLS::ZRelBtnRelease()
	{
		// hardware button should be not released if released already
		if (!releaseSW.zPressed)
		{
			return;
		}
		releaseSW.zPressed = false;
		zStatus.breakEn = true;
		OTCXYZStaMsgUpdateReason reason;
		reason.relSWChange = true;
		reason.zStaChange = true;
		if ((operateMode == ControlOpMode::MMReq) && (!releaseSW.xyzPressed) && (!releaseSW.yPressed) && (!releaseSW.xPressed))
		{
			operateMode = ControlOpMode::Idle;
			reason.OpModeChange = true;
		}

		StatusUpdateToSCB(reason);
	}

	void OTCXYZControllerCLS::XYZRelBtnPress()
	{
		// hardware button should be not pressed if pressed
		if (releaseSW.xyzPressed)
		{
			return;
		}
		releaseSW.xyzPressed = true;
		xStatus.breakEn = yStatus.breakEn = zStatus.breakEn = false;
		OTCXYZStaMsgUpdateReason reason;
		reason.relSWChange = true;
		reason.xStaChange = reason.yStaChange = reason.zStaChange = true;
		if (operateMode == ControlOpMode::Idle)
		{
			operateMode = ControlOpMode::MMReq;
			reason.OpModeChange = true;
		}
		StatusUpdateToSCB(reason);
	}

	void OTCXYZControllerCLS::XYZRelBtnRelease()
	{
		// hardware button should be not released if released already
		if (!releaseSW.xyzPressed)
		{
			return;
		}
		releaseSW.xyzPressed = false;
		xStatus.breakEn = yStatus.breakEn = zStatus.breakEn =true;
		OTCXYZStaMsgUpdateReason reason;
		reason.relSWChange = true;
		reason.xStaChange = reason.yStaChange = reason.zStaChange = true;
		if ((operateMode == ControlOpMode::MMReq) && (!releaseSW.xPressed) && (!releaseSW.yPressed) && (!releaseSW.xPressed))
		{
			operateMode = ControlOpMode::Idle;
			reason.OpModeChange = true;
		}
		StatusUpdateToSCB(reason);
	}

	//need TBD?
	void OTCXYZControllerCLS::DtPassPress()
	{
		releaseSW.dtSwPressed = true;
		OTCXYZStaMsgUpdateReason reason;
		reason.relSWChange = true;
		StatusUpdateToSCB(reason);
	}

	void OTCXYZControllerCLS::DtPassRelease()
	{
		releaseSW.dtSwPressed = false;
		OTCXYZStaMsgUpdateReason reason;
		reason.relSWChange = true;
		StatusUpdateToSCB(reason);
	}

	void OTCXYZControllerCLS::ARelBtnPress()
	{
		if (releaseSW.aPressed)
		{
			return;
		}

		releaseSW.aPressed = true;
		abControl->A_REL_BTN = true;
		OTCXYZStaMsgUpdateReason reason;
		reason.relSWChange = true;
		StatusUpdateToSCB(reason);
	}

	void OTCXYZControllerCLS::ARelBtnRelease()
	{
		if (!releaseSW.aPressed)
		{
			return;
		}
		releaseSW.aPressed = false;
		abControl->A_REL_BTN = false;
		OTCXYZStaMsgUpdateReason reason;
		reason.relSWChange = true;
		StatusUpdateToSCB(reason);
	}

	void OTCXYZControllerCLS::BRelBtnPress()
	{
		if (releaseSW.bPressed)
		{
			return;
		}
		releaseSW.bPressed = true;
		abControl->B_REL_BTN = true;
		OTCXYZStaMsgUpdateReason reason;
		reason.relSWChange = true;
		StatusUpdateToSCB(reason);
	}

	void OTCXYZControllerCLS::BRelBtnRelease()
	{
		if (!releaseSW.bPressed)
		{
			return;
		}
		releaseSW.bPressed = false;
		abControl->B_REL_BTN = false;
		OTCXYZStaMsgUpdateReason reason;
		reason.relSWChange = true;
		StatusUpdateToSCB(reason);
	}


	//Power Up
	void OTCXYZControllerCLS::PowerUp()
	{
		this->RunBootCode();
		this->RunAppCode();
		abControl->PowerUp();

	}

	//Power Off
	void OTCXYZControllerCLS::PowerOff()
	{
		this->ExitAppCode();
		abControl->PowerOff();
	}


}	