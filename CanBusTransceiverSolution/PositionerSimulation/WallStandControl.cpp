// OTC WSD Control implement

#include "stdafx.h"

#include "WallStandControl.h"

namespace PositionerSimulationNS {

	/*******************************  WSD KeyPad Implement*******************************/

	WSDKeypadCLS::WSDKeypadCLS(CanBusChannelCls ^ bus)
	{
		canBus = bus;
	}

	bool WSDKeypadCLS::AUTO_POS_SW::get()
	{
		return autoPosSW;
	}

	void WSDKeypadCLS::AUTO_POS_SW::set(bool value)
	{
		autoPosSW = value;
		WSDKEYStatusMsgUpdateReason updateReason;
		updateReason.comMtSWChange = true;

		WSDKeyStatusMsgPS ^ request = gcnew WSDKeyStatusMsgPS;
		ComplexMotionSW complexMt;
		complexMt.autoPosSW = value;
		KeyIndicator indicators;
		indicators.autoPosInd = value;

		request->COM_MT_SW = complexMt;
		request->KEY_INDICATOR = indicators;
		request->UPDATE_REASON = updateReason;

		List<AxisCtlProtoStuct> ^ responseFrameList = gcnew List<AxisCtlProtoStuct>;
		responseFrameList->Add(request->GetFrame());
		
		canBus->Send(responseFrameList);
	}


	bool WSDKeypadCLS::AUTO_POS_IND::get()
	{
		return autoPosInd;
	}

	void WSDKeypadCLS::AUTO_POS_IND::set(bool value)
	{
		WSDKEYStatusMsgUpdateReason updateReason;
		updateReason.comMtSWChange = true;

		WSDKeyStatusMsgPS ^ request = gcnew WSDKeyStatusMsgPS;
		ComplexMotionSW complexMt;
		KeyIndicator indicators;
		indicators.autoPosInd = true;

		request->COM_MT_SW = complexMt;
		request->KEY_INDICATOR = indicators;
		request->UPDATE_REASON = updateReason;

		List<AxisCtlProtoStuct> ^ responseFrameList = gcnew List<AxisCtlProtoStuct>;
		responseFrameList->Add(request->GetFrame());

		canBus->Send(responseFrameList);
	}

	bool WSDKeypadCLS::AUTO_CENTER_SW::get()
	{
		return autoCenterSW;
	}

	void WSDKeypadCLS::AUTO_CENTER_SW::set(bool value)
	{
		autoCenterSW = value;
		WSDKEYStatusMsgUpdateReason updateReason;
		updateReason.comMtSWChange = true;

		WSDKeyStatusMsgPS ^ request = gcnew WSDKeyStatusMsgPS;
		ComplexMotionSW complexMt;
		complexMt.autoCenterSW = value;
		KeyIndicator indicators;
		indicators.autoCenterInd = value;

		request->COM_MT_SW = complexMt;
		request->KEY_INDICATOR = indicators;
		request->UPDATE_REASON = updateReason;

		List<AxisCtlProtoStuct> ^ responseFrameList = gcnew List<AxisCtlProtoStuct>;
		responseFrameList->Add(request->GetFrame());

		canBus->Send(responseFrameList);
	}

	bool WSDKeypadCLS::AUTO_CENTER_IND::get()
	{
		return autoCenterInd;
	}

	void WSDKeypadCLS::AUTO_CENTER_IND::set(bool value)
	{
		//do not need to change indicator from outside now
	}

	/*******************************  WSD ControllerCLS Implement*******************************/

	/********************private methods******************/
	List<AxisCtlProtoStuct> ^ WSDControllerCLS::GetConfigurationRsp()
	{
		CtlConfigRespPS ^ bootConfigResponse = gcnew CtlConfigRespPS(ControlType::WSD);
		bootConfigResponse->HARDWARE_VERSION = hardwareVersion;
		bootConfigResponse->BoardType = this->hardwareType;
		bootConfigResponse->MODE = runMode;
		bootConfigResponse->SWITCH_CODE = swCode;
		bootConfigResponse->BOOT_TAG = bootTagVersion;
		bootConfigResponse->BOOT_REV = bootRevision;
		bootConfigResponse->FPGA_TAG = fpgaTagVersion;
		bootConfigResponse->FPGA_REV = fpgaRevision;
		CtlAppVerRspPS^ bootCtlAppResponse = gcnew CtlAppVerRspPS(ControlType::WSD);
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
		switch (keyPadNum)
		{
		case 0:
			bootCtlAppResponse->OPTIONS &= 0xFC;
			break;
		case 1:
			bootCtlAppResponse->OPTIONS &= 0xFC;
			bootCtlAppResponse->OPTIONS += 0x01;
			break;
		case 2:
			bootCtlAppResponse->OPTIONS &= 0xFC;
			bootCtlAppResponse->OPTIONS += 0x02;
			break;
		default:
			bootCtlAppResponse->OPTIONS &= 0xFC;
			bootCtlAppResponse->OPTIONS += 0x03;
			break;
		}
		
		CtlBootStrapVerRspPS^ bootCtlBootStrapResponse = gcnew CtlBootStrapVerRspPS(ControlType::WSD);
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

	AxisCtlProtoStuct WSDControllerCLS::GetStatusMsg(WSDStatusMsgUpdateReason reason)
	{
		WSDStatusMsgPS ^ statusUpdateMsg = gcnew WSDStatusMsgPS;
		statusUpdateMsg->OP_MODE = operateMode;
		statusUpdateMsg->DETECTOR_ID = detectorID;
		statusUpdateMsg->GRID_TYPE = gridType;
		statusUpdateMsg->DT_SENSOR = detectorSensor;
		statusUpdateMsg->MT_CONTROL = montionControl;
		statusUpdateMsg->SENSORS = sensors;
		statusUpdateMsg->SW_STATUS = swStatus;
		statusUpdateMsg->TILT_DT = tiltDetent;
		statusUpdateMsg->X_DT = xDetent;
		statusUpdateMsg->Z_DT = zDetent;

		statusUpdateMsg->UPDATE_REASON = reason;

		return statusUpdateMsg->GetFrame();
	}

	AxisCtlProtoStuct WSDControllerCLS::GetHeartBeatMsg(WSDHBUpdateReason reason)
	{
		WSDHBMsgPS ^ msg = gcnew WSDHBMsgPS;
		//if (operateMode == ControlOpMode::MMReq || operateMode == ControlOpMode::Idle || operateMode == ControlOpMode::MLock)
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

		WSDCalStatus calStatus;
		calStatus.HorRMCalDone = horRMCalDone;
		calStatus.PinCalDone = pinCalDone;
		calStatus.VertRMCalDone = vertRMCalDone;
		calStatus.XMaxSet = xMaxSet;
		calStatus.XMinSet = xMinSet;
		calStatus.ZMaxSet = zMaxSet;
		calStatus.ZMinSet = zMinSet;

		msg->CAL_STATUS = calStatus;
		msg->UPDATE_REASON = reason;

		return msg->GetFrame();
	}

	AxisCtlProtoStuct WSDControllerCLS::GetWSDTiltAngMsg(WSDTiltPosUpdateReason reason)
	{
		WSDTiltPosMsgPS ^ msg = gcnew WSDTiltPosMsgPS;
		msg->ANG_POS = angPosition;
		msg->TILT_POS = tiltPosition;

		WSDTiltCalStatus calStatus;
		calStatus.TiltHorSet = tiltHorSet;
		calStatus.TiltMinSet = tiltMinSet;
		calStatus.TiltVertSet = tiltVertSet;

		msg->CAL_STATUS = calStatus;
		msg->UPDATE_REASON = reason;

		return msg->GetFrame();
	}

	void WSDControllerCLS::RunBootCode()
	{
		runMode = ControlHWMode::BOOT;
		Console::WriteLine("WSD booting ....");

		keyPadNum = 1;

		List<AxisCtlProtoStuct> ^ configResponse = GetConfigurationRsp();
		canBus->Send(configResponse);

		//boot code will take about 3s to run into application code.
	}

	void WSDControllerCLS::RunAppCode()
	{
		runMode = ControlHWMode::APP;
		Console::WriteLine("WSD APP initalizing ....");

		//calibrate the WSD
		xMinSet = xMaxSet = zMinSet = zMaxSet = horRMCalDone = vertRMCalDone = pinCalDone = true;
		tiltHorSet = tiltMinSet = tiltVertSet = true;

		//set the wsd position
		xPosition = -252;
		yPosition = 425;
		zPosition = 1405;
		tiltPosition = 0;
		angPosition = 0;

		xPosMinLim = -700;
		xPosMaxLim = 700;

		zPosMinLim = 290;
		zPosMaxLim = 2300;

		tiltPosMinLim = -20;
		tiltPosMaxLim = 90;

		xmotorPos = xdevicePos = xlliPos = xabsEncoder = xPosition;
		zmotorPos = zdevicePos = zlliPos = zabsEncoder = zPosition;
		tiltmotorPos = tiltdevicePos = tiltabsEncoder = tiltPosition;

		xserErr = zserErr = tiltserErr = 0;
		xvoltage = zvoltage = tiltvoltage = 0;
		xcurrent = zcurrent = tiltcurrent = 0;
		xvel = zvel = tiltvel = 0;


		//set the detector type
		detectorType = DetectorType::DRX1;
	//	detectorType = DetectorType::Undefined;

		//set the detector id
	    detectorID = 0x01;
		tiltDetent = 0x00;

		//set the grid type
		gridType = GridType::NotPresent;

		//set the bucky rotation
		detectorSensor.buckyInPort = true;
		//detectorSensor.buckyInLand = true;
		//set the detector rotation
       // detectorSensor.detInLand = true;
		detectorSensor.detInPort = true;

		//set the tray status
		sensors.isTrayIn = true;
		sensors.buckyExtend = false;
		sensors.buckyRetract = true;

		//set the room layout
		layout = WSDRoomLayout::Layout1;



		operateMode = ControlOpMode::Idle;
		Console::WriteLine("WSD is now in IDLE mode");
		List<AxisCtlProtoStuct> ^ initResponse = GetConfigurationRsp();

		// enable Z brake
		montionControl.zBrakeEn = true;
		montionControl.tiltMotorRun = true;
		montionControl.xMotorRun = true;
		montionControl.zMotorRun = false;

		WSDStatusMsgUpdateReason reason;
		reason.opModeChange = true;
		reason.detIdChange = true;
		reason.motionCtlChange = true;
		reason.buckyGridChange = true;
		reason.detentChange = true;
		reason.sensorChange = true;
		reason.swIndStateChange = true;
		reason.scbRequest = false;

		initResponse->Add(GetStatusMsg(reason));

		//Init Heartbeat Message
		WSDHBUpdateReason heartbeatReason;
		heartbeatReason.calStatusChange = true;
		heartbeatReason.movementOccur = true;
		heartbeatReason.scbRequest = true;
		initResponse->Add(GetHeartBeatMsg(heartbeatReason));


		CtlErrStatRespMsgPS ^ errorResponse = gcnew CtlErrStatRespMsgPS(ControlType::WSD);
		errorResponse->ERROR_CODE = 0;
		errorResponse->UPDATEREASON = CtlErrStatRespUpdateReason::PeriodicUpdate;
		initResponse->Add(errorResponse->GetFrame());

		//App init update
		canBus->Send(initResponse);

		//Start to handle SCB/other controllers' message
		canDataHandlerThreadStart = gcnew ThreadStart(this, &WSDControllerCLS::CanDataHandlerFunc);
		canDataHanlerThread = gcnew Thread(canDataHandlerThreadStart);

		heartBeatThreadStart = gcnew ThreadStart(this, &WSDControllerCLS::HeartBeatFunc);
		heartBeatThread = gcnew Thread(heartBeatThreadStart);

		canDataHanlerThread->IsBackground = true;
		heartBeatThread->IsBackground = true;

		canBus->CanDataReceivedEvent += gcnew CanBusChannelNS::CanDataReceivedDelegate(this, &WSDControllerCLS::OnCanDataReceivedEvent);
		canDataHanlerThread->Start();
		heartBeatThread->Start();
	}

	void WSDControllerCLS::ExitAppCode()
	{
		canBus->CanDataReceivedEvent -= gcnew CanBusChannelNS::CanDataReceivedDelegate(this, &WSDControllerCLS::OnCanDataReceivedEvent);

		heartBeatThread->Abort();
		canDataHanlerThread->Abort();

		runMode = ControlHWMode::BOOT;
		operateMode = ControlOpMode::NoInit;

		montionControl.tiltMotorRun = montionControl.xMotorRun = montionControl.zBrakeEn = montionControl.zMotorRun = false;
		
		xMinSet = xMaxSet = zMinSet = zMaxSet = pinCalDone = horRMCalDone = vertRMCalDone = false;

		wsdReceiveQueue->Clear();
	}


	// convert the diagnostic command response string to can frames list
	List<AxisCtlProtoStuct>^ WSDControllerCLS::ConvertDiagResponse(String^ response)
	{
		int i = 2;
		List<AxisCtlProtoStuct> ^ responseFrameList = gcnew List<AxisCtlProtoStuct>;
		AxisCtlProtoStuct tempFrame;
		tempFrame.id = 0x2C7;
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
	void WSDControllerCLS::RemoteDiagHandler()
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
				String ^responseStr = "C:0?d p=" + (zPosition * 4).ToString() + " vel=" + zvel.ToString() + " err=" + (zserErr * 4).ToString() + " mp=" + (zmotorPos * 4).ToString() + " dp=" + (zdevicePos * 4).ToString() + " aep=" + (zabsEncoder * 4).ToString() + " pot=" + (zpot * 4).ToString() + " v=" + (zvoltage * 4).ToString() + " i=" + (zcurrent * 256).ToString() + " Lp=" + (zlliPos * 1000).ToString();
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
				canBus->Send(frameList);
			}
			if (test->Equals("U:1?d"))
			{
				String ^responseStr = "C:0?d p=" + (tiltPosition * 10).ToString() + " vel=" + tiltvel.ToString() + " err=" + (tiltserErr * 10).ToString() + " mp=" + (tiltmotorPos * 10).ToString() + " dp=" + (tiltdevicePos * 10).ToString() + " aep=" + (tiltabsEncoder * 10).ToString() + " pot=" + (tiltpot * 10).ToString() + " v=" + (tiltvoltage * 4).ToString() + " i=" + (tiltcurrent * 256).ToString() + " Lp=0";
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
				canBus->Send(frameList);
			}
			if (test->Equals("U:2?d"))
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
				String ^responseStr = "C:0?l st=" + (tiltPosMinLim * 10).ToString() + " et=" + (tiltPosMaxLim * 10).ToString();
				List<AxisCtlProtoStuct>^ frameList = ConvertDiagResponse(responseStr);
				canBus->Send(frameList);
			}
			if (test->Equals("U:2?L"))
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
	void WSDControllerCLS::AutoMoveFunc()
	{
		int xStepCount = 0, yStepCount = 0, zStepCount = 0,tiltStepCount = 0;
		double xLastStep = 0, yLastStep = 0, zLastStep = 0,tiltLastStep = 0;
		bool xFinish = false, yFinish = false, zFinish = false,tiltFinsh =false;

		bool xUp, yUp, zUp,tiltUp;

		if (xDestination > xPosition)
		{
			xUp = true;
			xStepCount = (int)((xDestination - xPosition) / autoMoveAxisStep);
			xLastStep = (xDestination - xPosition) - xStepCount*autoMoveAxisStep;
		}
		else
		{
			xUp = false;
			xStepCount = (int)((xPosition - xDestination) / autoMoveAxisStep);
			xLastStep = (xPosition - xDestination) - xStepCount*autoMoveAxisStep;
		}

		if (yDestination > yPosition)
		{
			yUp = true;
			yStepCount = (int)((yDestination - yPosition) / autoMoveAxisStep);
			yLastStep = (yDestination - yPosition) - yStepCount*autoMoveAxisStep;
		}
		else
		{
			yUp = false;
			yStepCount = (int)((yPosition - yDestination) / autoMoveAxisStep);
			yLastStep = (yPosition - yDestination) - yStepCount*autoMoveAxisStep;
		}

		if (zDestination > zPosition)
		{
			zUp = true;
			zStepCount = (int)((zDestination - zPosition) / autoMoveAxisStep);
			zLastStep = (zDestination - zPosition) - zStepCount * autoMoveAxisStep;
		}
		else
		{
			zUp = false;
			zStepCount = (int)((zPosition - zDestination) / autoMoveAxisStep);
			zLastStep = (zPosition - zDestination) - zStepCount * autoMoveAxisStep;
		}

		if (tiltDestination > tiltPosition)
		{
			tiltUp = true;
			tiltStepCount = (int)(tiltDestination * 10 - tiltPosition * 10) / (int)(autoMoveAngStep * 10);
			tiltLastStep = (tiltDestination - tiltPosition) - autoMoveAngStep*tiltStepCount;
		}
		else
		{
			tiltUp = false;
			tiltStepCount = (int)(tiltPosition * 10 - tiltDestination * 10) / (int)(autoMoveAngStep * 10);
			tiltLastStep = (tiltPosition - tiltDestination) - autoMoveAngStep*tiltStepCount;
		}
		Thread::Sleep(1000);
		if (autoContinueNeed)
		{
			while (!xFinish || !yFinish || !zFinish || !tiltFinsh)
			{
				if (automoveEvent->WaitOne())
				{
					if (!zFinish)
					{
						if (zUp)
						{
							if (montionControl.zBrakeEn)
							{
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								WSDStatusMsgUpdateReason reason;
								reason.motionCtlChange = true;
								montionControl.zMotorRun = true;
								montionControl.zBrakeEn = false;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);

							}
							if (zStepCount > 0)
							{
								Z_POS += autoMoveAxisStep;
								zStepCount--;
							}
							if (zStepCount == 0)
							{
								Z_POS += zLastStep;
								zFinish = true;
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								WSDStatusMsgUpdateReason reason;
								reason.motionCtlChange = true;
								montionControl.zMotorRun = false;
								montionControl.zBrakeEn = true;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);
							}

						}
						else
						{
							if (montionControl.zBrakeEn)
							{
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								WSDStatusMsgUpdateReason reason;
								reason.motionCtlChange = true;
								montionControl.zMotorRun = true;
								montionControl.zBrakeEn = false;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);

							}
							if (zStepCount > 0)
							{
								Z_POS -= autoMoveAxisStep;
								zStepCount--;
							}
							if (zStepCount == 0)
							{
								Z_POS -= zLastStep;
								zFinish = true;
								List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

								// change motor status
								WSDStatusMsgUpdateReason reason;
								reason.motionCtlChange = true;
								montionControl.zMotorRun = false;
								montionControl.zBrakeEn = true;
								sendbuffer->Add(GetStatusMsg(reason));
								canBus->Send(sendbuffer);
							}
						}
					}
					

					if (zFinish && (!yFinish))
					{
						if (yUp)
						{
							if (yStepCount > 0)
							{
								Y_POS += autoMoveAxisStep;
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
								Y_POS -= autoMoveAxisStep;
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
								X_POS += autoMoveAxisStep;
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
								X_POS -= autoMoveAxisStep;
								xStepCount--;
							}
							if (xStepCount == 0)
							{
								X_POS -= xLastStep;
								xFinish = true;
							}
						} // end of xUp if
					}// end of z/y finish if

					if (zFinish && xFinish && yFinish && (!tiltFinsh))
					{
						if (tiltUp)
						{
							if (tiltStepCount > 0)
							{
								TILT_POS += autoMoveAngStep;
								tiltStepCount--;
							}
							if (tiltStepCount == 0)
							{
								TILT_POS += tiltLastStep;
								tiltFinsh = true;
							}
						}
						else
						{
							if (tiltStepCount > 0)
							{
								TILT_POS -= autoMoveAngStep;
								tiltStepCount--;
							}
							if (tiltStepCount == 0)
							{
								TILT_POS -= tiltLastStep;
								tiltFinsh = true;
							}
						}//end of if tiltUp
					}// end of if zFinish && xFinish && yFinish

				} // end of if automoveEvent
			}// end of while
		}// end of if autoContinueNeed

		// send the finish response to SCB and controllers
		AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::WSD);
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

			WSDStatusMsgUpdateReason reason;
			reason.scbRequest = true;
			reason.opModeChange = true;
			sendbuffer->Add(GetStatusMsg(reason));
			canBus->Send(sendbuffer);
		}
	//	ExitAutoMove();

		ABAutoMoveFlag = XYZAutoMoveFlag = TBLAutoMoveFlag = false;
		ABAutoFinishFlag = XYZAutoFinishFlag = TBLAutoFinishFlag = false;
		return;

	}// end of auto move function

	// exit automovement function
	void WSDControllerCLS::ExitAutoMove()
	{
		if (moveFinishEvent->WaitOne())
		{
			List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;
			operateMode = ControlOpMode::Idle;
			WSDStatusMsgUpdateReason reason;
			reason.opModeChange = true;
			reason.scbRequest = true;
			sendbuffer->Add(GetStatusMsg(reason));
			canBus->Send(sendbuffer);
			ABAutoMoveFlag = XYZAutoMoveFlag = TBLAutoMoveFlag = false;
			ABAutoFinishFlag = XYZAutoFinishFlag = TBLAutoFinishFlag = false;
		}
	}

	void WSDControllerCLS::ProcessProtocalData(List<AxisCtlProtoStuct> ^ dataQueue)
	{
		for each(AxisCtlProtoStuct protocalData in dataQueue)
		{
			//Console::WriteLine("{0:x} on WSD {1}", protocalData.id,count++);
			switch (protocalData.id)
			{
			case 0x461:
					//Console::WriteLine("SCB Configuration Request Received on WSD control");
					{
						SCBConfigReqPS ^ request = gcnew SCBConfigReqPS(protocalData);
						if (request->WSD_REQ == true)
						{
							List<AxisCtlProtoStuct> ^ wsdConfigMsg = GetConfigurationRsp();
							canBus->Send(wsdConfigMsg);
						}
					}
				break;
			case 0x410:
				//Console::WriteLine("SCB Controller Status Request Received on WSD control");
				{
					SCBCtrlStatusReqPS^ request = gcnew SCBCtrlStatusReqPS(protocalData);
					if (request->WSD_RT == true)
					{
						List<AxisCtlProtoStuct> ^ sendBUffer = gcnew List<AxisCtlProtoStuct>;
						if (request->POSUPDATE_REQ == true)
						{
							WSDHBUpdateReason reason1;
							reason1.scbRequest = true;
							sendBUffer->Add(GetHeartBeatMsg(reason1));
							// add tilt and angulation message
							WSDTiltPosUpdateReason reason2;
							reason2.scbRequest = true;
							sendBUffer->Add(GetWSDTiltAngMsg(reason2));
						}
						if (request->CONTSTA_REQ == true)
						{
							WSDStatusMsgUpdateReason reason;
							reason.scbRequest = true;
							sendBUffer->Add(GetStatusMsg(reason));
						}
						if (request->ERRSTA_REQ == true)
						{
							CtlErrStatRespMsgPS ^ response = gcnew CtlErrStatRespMsgPS(ControlType::WSD);
							response->ERROR_CODE = 0;
							response->UPDATEREASON = CtlErrStatRespUpdateReason::SCBRequest;
							sendBUffer->Add(response->GetFrame());
						}
						if (request->LLIPOS_REQ == true)
						{
							WSDLLIStatusMsgPS ^ response = gcnew WSDLLIStatusMsgPS;
							response->Z_LLIPOS = (Int32)(zPosition * 1000);
							response->ZTRAV_END = zPosMaxLim;
							response->ZTRAV_START = zPosMinLim;
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
							 /* if (!keypad->AUTO_POS_IND)
							  {
								  keypad->AUTO_POS_IND = true;
							  }*/
						  }
						  if ((xyzResponse->AUTOMOVE_STATUS == AMoveStatus::Complete || xyzResponse->AUTOMOVE_STATUS == AMoveStatus::Abort || xyzResponse->AUTOMOVE_STATUS == AMoveStatus::Error)&&XYZAutoMoveFlag)
						  {
							  XYZAutoFinishFlag = true;

							  if ((ABAutoFinishFlag == ABAutoMoveFlag) && (XYZAutoMoveFlag == XYZAutoFinishFlag) && (TBLAutoMoveFlag == TBLAutoFinishFlag))
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
							 /* if (!keypad->AUTO_POS_IND)
							  {
								  keypad->AUTO_POS_IND = true;
							  }*/
						  }
						  if ((abResponse->AUTOMOVE_STATUS == AMoveStatus::Complete || abResponse->AUTOMOVE_STATUS == AMoveStatus::Abort || abResponse->AUTOMOVE_STATUS == AMoveStatus::Error) && ABAutoMoveFlag)
						  {
							  ABAutoFinishFlag = true;

							  if ((ABAutoFinishFlag == ABAutoMoveFlag) && (XYZAutoMoveFlag == XYZAutoFinishFlag) && (TBLAutoMoveFlag == TBLAutoFinishFlag))
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
							/*  if (!keypad->AUTO_POS_IND)
							  {
								  keypad->AUTO_POS_IND = true;
							  }*/
						  }
						  if ((tblResponse->AUTOMOVE_STATUS == AMoveStatus::Complete || tblResponse->AUTOMOVE_STATUS == AMoveStatus::Abort || tblResponse->AUTOMOVE_STATUS == AMoveStatus::Error) && TBLAutoMoveFlag)
						  {
							  TBLAutoFinishFlag = true;

							  if ((ABAutoFinishFlag == ABAutoMoveFlag) && (XYZAutoMoveFlag == XYZAutoFinishFlag) && (TBLAutoMoveFlag == TBLAutoFinishFlag))
							  {
								  moveFinishEvent->Set();
							  }
						  }
			}
				break;

			//case 0x401:
			//{
			//	
			//			  List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

			//			  // Accept the destination
			//			  AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::WSD);
			//			  response->AUTOMOVE_STATUS = AMoveStatus::AModeAccepted;
			//			  response->SW_SETTING = swCode;
			//			  sendbuffer->Add(response->GetFrame());

			//			  //change opmode to AMTP
			//			  operateMode = ControlOpMode::AMTP;
			//			  WSDStatusMsgUpdateReason reason;
			//			  reason.opModeChange = true;
			//			  reason.scbRequest = true;
			//			  sendbuffer->Add(GetStatusMsg(reason));

			//			  //send on bus
			//			  canBus->Send(sendbuffer);
			//}
			//	break;

			case 0x402:
				Console::WriteLine("WSD Auto Move Destination received {0}",count++);
				{
					SCBWSDSetAutoDesMsgPS ^ request = gcnew SCBWSDSetAutoDesMsgPS(protocalData);
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
						if (request->TILT_POS != -32768)
						{
							tiltDestination = request->TILT_POS;

						}
						else
						{
							tiltDestination = tiltPosition;
						}
						
						List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

						//change opmode to AMTP
						operateMode = ControlOpMode::AMTP;
						WSDStatusMsgUpdateReason reason;
						reason.opModeChange = true;
						reason.scbRequest = true;
						reason.motionCtlChange = true;
						sendbuffer->Add(GetStatusMsg(reason));
						canBus->Send(sendbuffer);

						// Accept the destination
						AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::WSD);
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

						  ABAutoMoveFlag = XYZAutoMoveFlag = TBLAutoMoveFlag = false;
						  ABAutoFinishFlag = XYZAutoFinishFlag = TBLAutoFinishFlag = false;

						  List<AxisCtlProtoStuct> ^ sendbuffer = gcnew List<AxisCtlProtoStuct>;

						  // send abort response to SCB
						  AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::WSD);
						  response->AUTOMOVE_STATUS = AMoveStatus::Abort;
						  response->SW_SETTING = swCode;
						  sendbuffer->Add(response->GetFrame());

						  //change status to idle
						  if (operateMode == ControlOpMode::AMTP)
						  {
							  operateMode = ControlOpMode::Idle;
							  WSDStatusMsgUpdateReason reason;
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
							  AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::WSD);
							  response->AUTOMOVE_STATUS = AMoveStatus::EnAccepted;
							  response->SW_SETTING = swCode;
							  sendbuffer->Add(response->GetFrame());
							  canBus->Send(sendbuffer);

							  autoMoveThread = gcnew Thread(gcnew ThreadStart(this, &WSDControllerCLS::AutoMoveFunc));
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

							  AutoMoveRespMsgPS ^ response = gcnew AutoMoveRespMsgPS(ControlType::WSD);
							  response->AUTOMOVE_STATUS = AMoveStatus::Complete;
							  response->SW_SETTING = swCode;
							  sendbuffer->Add(response->GetFrame());
							  canBus->Send(sendbuffer);
							 
							  // wait for other controllers' finish flag and update status
							  Thread ^ exitAutomoveThread = gcnew Thread(gcnew ThreadStart(this, &WSDControllerCLS::ExitAutoMove));
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
		
			case 0x422:
				//Console::WriteLine("SCB Clear Calculation Received on WSD control");
				{
					SCBClrCalPS^ request = gcnew SCBClrCalPS(protocalData);
					if (request->WSD_CLEAR == true)
					{
						List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
						if (request->CAL_TYPE1 == true)
						{
							zMinSet = false;
						}
						if (request->CAL_TYPE3 == true)
						{
							zMaxSet = false;
						}
						if (request->CAL_TYPE4 == true)
						{
							xMinSet = false;
						}
						if (request->CAL_TYPE5 == true)
						{
							xMaxSet = false;
						}
						if (request->CAL_TYPE6 == true)
						{
							horRMCalDone = false;
						}
						if (request->CAL_TYPE7 == true)
						{
							vertRMCalDone = false;
						}
						if (request->PIN_TYPE == true)
						{
							pinCalDone = false;
						}

						WSDHBUpdateReason reason1;
						reason1.calStatusChange = true;
						reason1.scbRequest = true;
						sendBuffer->Add(GetHeartBeatMsg(reason1));

						if (!(zMinSet && zMaxSet && xMinSet && xMaxSet && horRMCalDone && vertRMCalDone && pinCalDone))
						{
							operateMode = ControlOpMode::NRmCal;
							WSDStatusMsgUpdateReason reason2;
							reason2.opModeChange = true;
							reason2.scbRequest = true;
							sendBuffer->Add(GetStatusMsg(reason2));
						}

						canBus->Send(sendBuffer);
					}
				}
				break;

			case 0x600:
				//Console::WriteLine("SCB WSD Set Room Cal Received on WSD control");
				{
					WSDSetRMCalMsgPS ^ request = gcnew WSDSetRMCalMsgPS(protocalData);
					if ((request->UPDATETYPE == WSDRMCalUpdateType::Horizontal) || (request->UPDATETYPE == WSDRMCalUpdateType::Vertical))
					{
						List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
						X_POS = request->X_POS;
						Y_POS = request->Y_POS;
						Z_POS = request->Z_POS;


						if (request->UPDATETYPE == WSDRMCalUpdateType::Horizontal)
						{
							/*detectorSensor.buckyInLand = true;
							detectorSensor.detInLand = true;
							detectorSensor.buckyInPort = false;
							detectorSensor.detInPort = false;*/
							horRMCalDone = true;
							vertRMCalDone = true;
							// tbd
						}
						else
						{
						/*	detectorSensor.buckyInLand = false;
							detectorSensor.detInLand = false;
							detectorSensor.buckyInPort = true;
							detectorSensor.detInPort = true;*/
							horRMCalDone = true;
							vertRMCalDone = true;
						}
						switch (request->ORIENTATION)
						{
						case WSDRMOrientation::LeftWall:
							layout = WSDRoomLayout::Layout1;
							break;
						case WSDRMOrientation::BackWall:
							layout = WSDRoomLayout::Layout2;
							break;
						case WSDRMOrientation::RightWall:
							layout = WSDRoomLayout::Layout3;
							break;
						case WSDRMOrientation::FrontWall:
							layout = WSDRoomLayout::Layout4;
							break;
						default:
							break;
						}

						WSDHBUpdateReason reason1;
						reason1.calStatusChange = true;
						reason1.scbRequest = true;
						sendBuffer->Add(GetHeartBeatMsg(reason1));

						WSDTiltPosUpdateReason reason3;
						reason3.calStaChange = true;
						reason3.scbRequest = true;
						sendBuffer->Add(GetWSDTiltAngMsg(reason3));

						if (zMinSet && zMaxSet && xMinSet && xMaxSet && horRMCalDone && vertRMCalDone && pinCalDone)
						{
							operateMode = ControlOpMode::MLock;
							WSDStatusMsgUpdateReason reason2;
							reason2.opModeChange = true;
							reason2.scbRequest = true;
							sendBuffer->Add(GetStatusMsg(reason2));
							operateMode = ControlOpMode::Idle;
							WSDStatusMsgUpdateReason reason4;
							reason4.opModeChange = true;
							reason4.scbRequest = true;
						}

						canBus->Send(sendBuffer);
					}
				}
				break;

			case 0x601:
				//Console::WriteLine("SCB WSD Set Axis Cal Received on WSD control");
				{
					WSDSetAxisCalMsgPS ^ request = gcnew WSDSetAxisCalMsgPS(protocalData);
					if (request->ZMIN_SET == true)
					{
						zMinSet = true;
					}
					if (request->ZMAX_SET == true)
					{
						zMaxSet = true;
					}
					if (request->XRAILMIN_SET == true)
					{
						xMinSet = true;
					}
					if (request->XRAILMAX_SET == true)
					{
						xMaxSet = true;
					}
					if (request->PIN_SET == true)
					{
						pinCalDone = true;
					}

					List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
					WSDHBUpdateReason reason;
					reason.calStatusChange = true;
					reason.scbRequest = true;
					sendBuffer->Add(GetHeartBeatMsg(reason));

					canBus->Send(sendBuffer);
				}
				break;

			case 0x430:
			{

						  AxisCtlProtoStuct frame = protocalData;
						  

						  WSDKEYStatusMsgUpdateReason updateReason;
						  updateReason.scbRequest = true;

						  WSDKeyStatusMsgPS ^ request = gcnew WSDKeyStatusMsgPS;
						  ComplexMotionSW complexMt;
						  KeyIndicator indicators;
						  indicators.autoTrackInd = false;

						  request->COM_MT_SW = complexMt;
						  request->KEY_INDICATOR = indicators;
						  request->UPDATE_REASON = updateReason;

						  List<AxisCtlProtoStuct> ^ responseFrameList = gcnew List<AxisCtlProtoStuct>;
						  responseFrameList->Add(request->GetFrame());

						  canBus->Send(responseFrameList);
			}
				break;

			case 0x620:
			{
						  
						  AxisCtlProtoStuct frame = protocalData;
						  bool autoPosValue = false;
						  if (frame.frameData[1] == 0x02)
						  {
							  autoPosValue = true;
						  }
						  else
						  {
							  autoPosValue = false;
						  }
		
						  WSDKEYStatusMsgUpdateReason updateReason;
						  updateReason.scbRequest = true;

						  WSDKeyStatusMsgPS ^ request = gcnew WSDKeyStatusMsgPS;
						  ComplexMotionSW complexMt;
						  KeyIndicator indicators;
						  indicators.autoPosInd = autoPosValue;

						  request->COM_MT_SW = complexMt;
						  request->KEY_INDICATOR = indicators;
						  request->UPDATE_REASON = updateReason;

						  List<AxisCtlProtoStuct> ^ responseFrameList = gcnew List<AxisCtlProtoStuct>;
						  responseFrameList->Add(request->GetFrame());

						  canBus->Send(responseFrameList);
			}
				
				break;

			case 0x470:
			//	Console::WriteLine("SCB File Transfer Request Received on WSD control");
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
				}
				break;
			case 0x490:
				//Console::WriteLine("SCB File Transfer Response Received on WSD control");
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
				}
				break;
			case 0x491:
			//	Console::WriteLine("SCB Block Upload Response Received on WSD control");
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
				}
				break;
			case 0x474:
			//	Console::WriteLine("clear ");
				break;

			case 0x457:
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
			default:
				
				break;
			}
		//	Console::WriteLine(" can id data: {0:x} received : ", protocalData.id);
			/*for (int i = 0; i < protocalData.size; i++)
			{
				Console::Write(" {0:x} ", protocalData.frameData[i]);
			}
			Console::WriteLine("");*/
			
		}
	}

	//send position update to scb
	void WSDControllerCLS::HBUpdateToSCB(WSDHBUpdateReason reason)
	{
		List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
		AxisCtlProtoStuct updateMsg = GetHeartBeatMsg(reason);
		sendBuffer->Add(updateMsg);
		canBus->Send(sendBuffer);
	}

	//update status to scb
	void WSDControllerCLS::StatusUpdateToSCB(WSDStatusMsgUpdateReason reason)
	{
		List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
		AxisCtlProtoStuct updateMsg = GetStatusMsg(reason);
		sendBuffer->Add(updateMsg);
		canBus->Send(sendBuffer);
	}

	//update tilt and angulation position to scb
	void WSDControllerCLS::TiltAngPosUpdateToSCB(WSDTiltPosUpdateReason reason)
	{
		List<AxisCtlProtoStuct> ^ sendBuffer = gcnew List<AxisCtlProtoStuct>;
		AxisCtlProtoStuct updateMsg = GetWSDTiltAngMsg(reason);
		sendBuffer->Add(updateMsg);
		canBus->Send(sendBuffer);	
	}



	/********************public methods******************/

	//ctor
	WSDControllerCLS::WSDControllerCLS(CanBusChannelCls ^ bus)
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
				if (nodeName == "fpgaTagVersion")
				{
					if (reader->Read())
					{
						fpgaTagVersion = UINT8::Parse(reader->Value);
					}
				}
				if (nodeName == "fpgaRevision")
				{
					if (reader->Read())
					{
						fpgaRevision = UINT16::Parse(reader->Value);
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

		/*hardwareVersion = 0x01;
		bootTagVersion = 0x34;
		bootRevision = 0x18F8;
		fpgaTagVersion = 0x01;
		fpgaRevision = 0x0005;
		appTagVersion = 0x34;
		appRevision = 0x18F8;
		motoParaTagVersion = 0x34;
		motoParaRevision = 0x18F8;
		bootStrapTagVersion = 0x34;
		bootStrapRevision = 0x18F8;*/

		swCode = PREWSDTYPE1;

		
		axisMoveStepLength = 10;
		angleMoveStepLength = 5;
		canBus = bus;
		wsdReceiveQueue = gcnew List<AxisCtlProtoStuct>;
		keypad = gcnew WSDKeypadCLS(canBus);

	}

	//detector id
	int WSDControllerCLS::DET_ID::get()
	{
		return detectorID;
	}

	// position property 
	double WSDControllerCLS::X_POS::get()
	{
		return xPosition;
	}

	void WSDControllerCLS::X_POS::set(double value)
	{
		xPosition = value;
		WSDHBUpdateReason reason;
		reason.movementOccur = true;
		HBUpdateToSCB(reason);
	}

	double WSDControllerCLS::Y_POS::get()
	{
		return yPosition;
	}

	void WSDControllerCLS::Y_POS::set(double value)
	{
		yPosition = value;
		WSDHBUpdateReason reason;
		reason.movementOccur = true;
		HBUpdateToSCB(reason);
	}

	double WSDControllerCLS::Z_POS::get()
	{
		return zPosition;
	}

	void WSDControllerCLS::Z_POS::set(double value)
	{
		zPosition = value;
		WSDHBUpdateReason reason;
		reason.movementOccur = true;
		HBUpdateToSCB(reason);
	}

	double WSDControllerCLS::TILT_POS::get()
	{
		return tiltPosition;
	}

	void WSDControllerCLS::TILT_POS::set(double value)
	{
		tiltPosition = value;
		WSDHBUpdateReason reason;
		reason.movementOccur = true;
		HBUpdateToSCB(reason);
	}


	double WSDControllerCLS::ANG_POS::get()
	{
		return angPosition;
	}

	void WSDControllerCLS::ANG_POS::set(double value)
	{
		angPosition = value;
		WSDHBUpdateReason reason;
		reason.movementOccur = true;
		HBUpdateToSCB(reason);
	}

	double WSDControllerCLS::AXIS_MOVE_STEP::get()
	{
		return axisMoveStepLength;
	}

	void WSDControllerCLS::AXIS_MOVE_STEP::set(double value)
	{
		axisMoveStepLength = value;
	}

	double WSDControllerCLS::ANG_MOVE_STEP::get()
	{
		return angleMoveStepLength;
	}

	void WSDControllerCLS::ANG_MOVE_STEP::set(double value)
	{
		angleMoveStepLength = value;
	}

	bool WSDControllerCLS::Z_REL_BTN::get()
	{
		return zRelBtnStatus;
	}

	void WSDControllerCLS::Z_REL_BTN::set(bool value)
	{
		if (value == true)
		{
			zRelBtnStatus = true;
			WSDStatusMsgUpdateReason reason;
			reason.swIndStateChange = true;
			if (operateMode == ControlOpMode::Idle)
			{
				operateMode = ControlOpMode::MMReq;
				reason.opModeChange = true;
			}
			StatusUpdateToSCB(reason);

		}
		else
		{
			zRelBtnStatus = false;
			WSDStatusMsgUpdateReason reason;
			reason.swIndStateChange = true;
			if ((operateMode == ControlOpMode::MMReq) && (!xRelBtnStatus))
			{
				operateMode = ControlOpMode::Idle;
				reason.opModeChange = true;
			}
			StatusUpdateToSCB(reason);
		}
	}

	bool WSDControllerCLS::X_REL_BTN::get()
	{
		return xRelBtnStatus;
	}

	void WSDControllerCLS::X_REL_BTN::set(bool value)
	{
		if (value == true)
		{
			xRelBtnStatus = true;
			WSDStatusMsgUpdateReason reason;
			reason.swIndStateChange = true;
			if (operateMode == ControlOpMode::Idle)
			{
				operateMode = ControlOpMode::MMReq;
				reason.opModeChange = true;
			}
			StatusUpdateToSCB(reason);

		}
		else
		{
			xRelBtnStatus = false;
			WSDStatusMsgUpdateReason reason;
			reason.swIndStateChange = true;
			if ((operateMode == ControlOpMode::MMReq) && (!zRelBtnStatus))
			{
				operateMode = ControlOpMode::Idle;
				reason.opModeChange = true;
			}
			StatusUpdateToSCB(reason);
		}
	}


	// the Z release button pressed

	void WSDControllerCLS::ZRelBtnPress()
	{
		if (zRelBtnStatus)
		{
			return;
		}

		zRelBtnStatus = true;
		montionControl.zBrakeEn = false;

		WSDStatusMsgUpdateReason reason;
		reason.motionCtlChange = true;
		reason.swIndStateChange = true;
		if (operateMode == ControlOpMode::Idle)
		{
			operateMode = ControlOpMode::MMReq;
			reason.opModeChange = true;
		}
		StatusUpdateToSCB(reason);
	}


	// the Z release button released
	void WSDControllerCLS::ZRelBtnRelease()
	{
		if (!zRelBtnStatus)
		{
			return;
		}

		zRelBtnStatus = false;
		montionControl.zBrakeEn = false;

		WSDStatusMsgUpdateReason reason;
		reason.motionCtlChange = true;
		reason.swIndStateChange = true;
		if ((operateMode == ControlOpMode::MMReq) && (!xRelBtnStatus))
		{
			operateMode = ControlOpMode::Idle;
			reason.opModeChange = true;
		}

		StatusUpdateToSCB(reason);
	}


	// the X release button pressed
	void WSDControllerCLS::XRelBtnPress()
	{
		if (xRelBtnStatus)
		{
			return;
		}

		xRelBtnStatus = true;

		WSDStatusMsgUpdateReason reason;
		reason.swIndStateChange = true;
		if (operateMode == ControlOpMode::Idle)
		{
			operateMode = ControlOpMode::MMReq;
			reason.opModeChange = true;
		}
		StatusUpdateToSCB(reason);
	}

	// the X release button released
	void WSDControllerCLS::XRelBtnRelease()
	{
		if (!xRelBtnStatus)
		{
			return;
		}

		xRelBtnStatus = false;

		WSDStatusMsgUpdateReason reason;
		reason.swIndStateChange = true;
		if ((operateMode == ControlOpMode::MMReq) && (!zRelBtnStatus))
		{
			operateMode = ControlOpMode::Idle;
			reason.opModeChange = true;
		}

		StatusUpdateToSCB(reason);
	}


	// the Z step up function
	void WSDControllerCLS::ZStepUp()
	{
		if (zRelBtnStatus)
		{
			Z_POS += AXIS_MOVE_STEP;
		}
	}

	// the Z step down function
	void WSDControllerCLS::ZStepDown()
	{
		if (zRelBtnStatus)
		{
			Z_POS -= AXIS_MOVE_STEP;
		}
	}

	// the X step up function
	void WSDControllerCLS::XStepUp()
	{
		if (xRelBtnStatus)
		{
			X_POS += AXIS_MOVE_STEP;
		}
	}

	// the X step down function
	void WSDControllerCLS::XStepDown()
	{
		if (xRelBtnStatus)
		{
			X_POS -= AXIS_MOVE_STEP;
		}
	}

	// the Tilt step up function
	void WSDControllerCLS::TiltUp()
	{
		TILT_POS = (TILT_POS * 10 + angleMoveStepLength * 10) / 10;
	}

	// the Tilt step down function
	void WSDControllerCLS::TiltDown()
	{
		TILT_POS = (TILT_POS * 10 - angleMoveStepLength * 10) / 10;
	}

	ControlOpMode WSDControllerCLS::OP_MODE::get()
	{
		return operateMode;
	}

	void WSDControllerCLS::OP_MODE::set(ControlOpMode value)
	{
		operateMode = value;
	}

	//data receive event handler
	void WSDControllerCLS::OnCanDataReceivedEvent(List<AxisCtlProtoStuct> ^ receivedQueue)
	{
		Monitor::Enter(receiveLockObject);
		wsdReceiveQueue->AddRange(receivedQueue);
		Monitor::Exit(receiveLockObject);
	}



	//can data handler threading function
	void WSDControllerCLS::CanDataHandlerFunc()
	{
		while (true)
		{
			int frameCount = wsdReceiveQueue->Count;
			if (frameCount > 0)
			{

				List<AxisCtlProtoStuct> ^ receivedFrameQueue = wsdReceiveQueue->GetRange(0, frameCount);
			    Monitor::Enter(receiveLockObject);
				wsdReceiveQueue->RemoveRange(0, frameCount);
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
	void WSDControllerCLS::HeartBeatFunc()
	{
		while (true)
		{
				WSDHBUpdateReason reason;
			   reason.scbRequest = false;
				reason.hbTimeOut = true;
				reason.movementOccur = false;
				AxisCtlProtoStuct msg = GetHeartBeatMsg(reason);

				WSDTiltPosUpdateReason reason1;
				//reason1.scbRequest = true;
				reason1.hbTimeOut = true;
				reason.movementOccur = false;
				AxisCtlProtoStuct msg1 = GetWSDTiltAngMsg(reason1);


				List<AxisCtlProtoStuct> ^sendBuffer = gcnew List<AxisCtlProtoStuct>;
				sendBuffer->Add(msg);
				sendBuffer->Add(msg1);
				canBus->Send(sendBuffer);
	
			//Console::WriteLine("WSD Heartbeat");
			//Console::WriteLine("WSD Mode:" + operateMode.ToString());
			Thread::Sleep(500);
		}
	}

	//Insert detector 
	void WSDControllerCLS::InsertDetector(Byte id)
	{
		detectorID = id;
		detectorSensor.detInPort = true;
		detectorSensor.detInLand = false;

		List<AxisCtlProtoStuct> ^ updateResponse = gcnew List<AxisCtlProtoStuct>;
		WSDStatusMsgUpdateReason reason;
		reason.opModeChange = false;
		reason.detIdChange = true;
		reason.motionCtlChange = false;
		reason.buckyGridChange = false;
		reason.detentChange = false;
		reason.sensorChange = true;
		reason.swIndStateChange = false;
		reason.scbRequest = false;

		updateResponse->Add(GetStatusMsg(reason));
		canBus->Send(updateResponse);
	}

	//Eject detector
	void WSDControllerCLS::EjectDetector()
	{
		detectorID = 0x00;
		detectorSensor.detInPort = false;
		detectorSensor.detInLand = false;

		List<AxisCtlProtoStuct> ^ updateResponse = gcnew List<AxisCtlProtoStuct>;
		WSDStatusMsgUpdateReason reason;
		reason.opModeChange = false;
		reason.detIdChange = true;
		reason.motionCtlChange = false;
		reason.buckyGridChange = false;
		reason.detentChange = false;
		reason.sensorChange = true;
		reason.swIndStateChange = false;
		reason.scbRequest = false;

		updateResponse->Add(GetStatusMsg(reason));
		canBus->Send(updateResponse);
	}

	//Power Up
	void WSDControllerCLS::PowerUp()
	{
		this->RunBootCode();
		this->RunAppCode();
	}

	//Power Off
	void WSDControllerCLS::PowerOff()
	{
		this->ExitAppCode();
	}

	//Change the grid type
	void WSDControllerCLS::ChangeGridType(GridType type)
	{
		gridType = type;
		List<AxisCtlProtoStuct> ^ updateResponse = gcnew List<AxisCtlProtoStuct>;
		WSDStatusMsgUpdateReason reason;
		reason.buckyGridChange = true;
		updateResponse->Add(GetStatusMsg(reason));
		canBus->Send(updateResponse);
	}

	//Change the bucky orientaion
	void WSDControllerCLS::ChangeBuckyOrientation(bool value)
	{
		if (value)
		{
			detectorSensor.buckyInPort = true;
			detectorSensor.buckyInLand = false;
		}
		else
		{
			detectorSensor.buckyInPort = false;
			detectorSensor.buckyInLand = true;
		}
		List<AxisCtlProtoStuct> ^ updateResponse = gcnew List<AxisCtlProtoStuct>;
		WSDStatusMsgUpdateReason reason;
		reason.buckyGridChange = true;
		updateResponse->Add(GetStatusMsg(reason));
		canBus->Send(updateResponse);

	}
		//change the detector orientation
	void WSDControllerCLS::ChangeDetOrientation(bool value1, bool value2)
	{
		if (value1 == true)
		{
			detectorSensor.detInPort = true;
			detectorSensor.detInLand = false;
		}
		else if (value2 == true)
		{
			detectorSensor.detInPort = false;
			detectorSensor.detInLand = true;
		}
		else if(value1 == false && value2 == false)
		{
			detectorSensor.detInPort = false;
			detectorSensor.detInLand = false;
		}
		List<AxisCtlProtoStuct> ^ updateResponse = gcnew List<AxisCtlProtoStuct>;
		WSDStatusMsgUpdateReason reason;
		reason.buckyGridChange = true;
		updateResponse->Add(GetStatusMsg(reason));
		canBus->Send(updateResponse);
	}

	//extract wall bucky
	void WSDControllerCLS::ExtractWallBucky(double value)
	{
		if (0 == value)
		{
			sensors.isTrayIn = true;
		}
		else
		{
			sensors.isTrayIn = false;
		}
		List<AxisCtlProtoStuct> ^ updateResponse = gcnew List<AxisCtlProtoStuct>;
		WSDStatusMsgUpdateReason reason;
		reason.buckyGridChange = true;
		updateResponse->Add(GetStatusMsg(reason));
		canBus->Send(updateResponse);
	}
}