//otc xyz definition

#pragma once

#include <Windows.h>
#include "TypeDefination.h"
#include "OTCABControl.h"

using namespace System;
using namespace CanBusChannelNS;
using namespace SCBProtocalWrapper;
using namespace System::Collections::Generic;
using namespace System::Threading;

namespace PositionerSimulationNS {

	// position update delegate
	public delegate void PositonUpdateDelegate();



	public ref class OTCXYZControllerCLS
	{
	private:

	
		/***************************** the control type and software versions *******************************/
		//the hardware type (DMC/TMC) of the OTC controller
		static ControlHardType hardwareType = ControlHardType::TMC;

		// the hardware version of controller
		UINT8 hardwareVersion;

		// the boot tag version
		UINT8 bootTagVersion;

		// the boot revision
		WORD bootRevision;

		// the FPGA tag version
		UINT8 fpgaTagVersion;

		// the FPGA revision
		WORD fpgaRevision;

		// the application tag version
		UINT8 appTagVersion;

		// the application revision
		WORD appRevision;

		// the motor parameter tag version
		UINT8 motoParaTagVersion;

		// the motor parameter revision;
		WORD motoParaRevision;

		// the bootstrap tag version
		UINT8 bootStrapTagVersion;

		// the bootstrap revision
		WORD bootStrapRevision;

		/***************************** the hardwares status and software mode *******************************/
		// the AB control connected with XYZ control
		OTCABControllerCLS^ abControl;

		// the movement control system hardware status
		ControlMSStatus xStatus, yStatus, zStatus;

		// the manual release switches
		MSwStatus releaseSW;

		// soft detents status
		UINT xDtStatus, yDtStatus, zDtStatus;

		// the AUX switch status
		EstopSW auxStatus;

		// if the IR Remote existing
		bool withIR;

		// the switch code of the OTCXYZ controller
		Byte swCode;

		// the control type (OTCXYZ)
		static ControlType type = ControlType::OTCXYZ;

		// the control running software mode
		static ControlHWMode runMode = ControlHWMode::BOOT;
		// current status of the OTCXYZ controller
		static ControlOpMode operateMode = ControlOpMode::NoInit;

		/***************************** OTC XYZ calibration status *******************************/
		bool XminSet, YminSet, ZminSet, XmaxSet, YmaxSet, ZmaxSet, RoomCal, PinCal;

		/***************************** OTC XYZ calibration status *******************************/
		double xPosition, yPosition, zPosition;

		double xPosMinLim, xPosMaxLim, yPosMinLim, yPosMaxLim, zPosMaxLim, zPosMinLim;

		double xmotorPos, xdevicePos, xabsEncoder, xserErr, xpot, xcurrent, xvoltage, xvel, xlliPos;

		double ymotorPos, ydevicePos, yabsEncoder, yserErr, ypot, ycurrent, yvoltage, yvel, ylliPos;

		double zmotorPos, zdevicePos, zabsEncoder, zserErr, zpot, zcurrent, zvoltage, zvel, zlliPos;

		double moveStepLength;
		/***************************** controller network system  *******************************/
		//the can bus which OTCXYZ controller will connect on
		CanBusChannelCls^ canBus;


		/***************************** class members  *******************************/

		//the receive buffer queue 
		List<AxisCtlProtoStuct> ^ otcXYZReceiveQueue;

		//the remote diagnostic cmd buffer
		array<wchar_t> ^ diagCMD;

		//the received cmd count
		int receivedCMD;

		//the cmd length
		int cmdLength;

		//convert remote diagnostic command response to can frames
		List<AxisCtlProtoStuct>^ ConvertDiagResponse(String^ response);

		//remote diagnostic command parser method
		void RemoteDiagHandler();



		// Auto movement thread
		Thread ^ autoMoveThread;

		// Auto movement XYZ destination
		double xDestination, yDestination, zDestination;

		// Auto movement AB destination
		double aDestination, bDestination;

		// AB Control automove participation flag and movement finish flag
		bool ABAutoMoveFlag, ABAutoFinishFlag;

		// Auto movement WSD destination
		double wsdXDestination, wsdYDestination, wsdZDestination, wsdTiltDestination;

		bool WSDAutoMoveFlag, WSDAutoFinishFlag;

		// Auto movement TBL destination
		double tblXDestination, tblYDestination, tblZDestination;

		bool TBLAutoMoveFlag, TBLAutoFinishFlag;
		// Auto movement step length
		static double autoMoveStep = 25;

		// Auto movement continue needed
		static bool autoContinueNeed = false;

		// Auto movement autoreset event
		static AutoResetEvent ^ automoveEvent = gcnew AutoResetEvent(true);

		// Auto movement finish autoreset event
		static AutoResetEvent ^ moveFinishEvent = gcnew AutoResetEvent(false);

		// Auto movement function
		void AutoMoveFunc();

		// Auto movement exit function
		void ExitAutoMove();

		/*************************** LT Function ****************************/
		UInt16 sweepAngle, layerHeight, sweepTime, delayTime;
		TBLMoveDict ltDirection;

		
		//the synchronization object
		static Object^ receiveLockObject = gcnew Object();

		//can data handle threading
		ThreadStart ^ canDataHandlerThreadStart;
		Thread^ canDataHanlerThread;

		//heart beat threading
		ThreadStart ^ heartBeatThreadStart;
		Thread^ heartBeatThread;

		//run boot strap on OTCXYZ controller,will check the SW code and initialize controller
		void RunBootCode();

		//run application code on OTCXYZ controller,will enable interrupt for receiving scb and other controllers' message.Will be in normal work logic
		void RunAppCode();

		//exit application code and reset the controller
		void ExitAppCode();

		//OTCXYZ control logic implement
		void ProcessProtocalData(List<AxisCtlProtoStuct> ^);

		//update control Positon/Cal status to scb
		void HBUpdateToSCB(OTCXYZHBUpdateReason);

		//update control status to scb
		void StatusUpdateToSCB(OTCXYZStaMsgUpdateReason);

	

		/***************************** response generator  *******************************/

		//get controller configuration message
		List<AxisCtlProtoStuct> ^ GetConfigurationRsp();

		//get controller status message
		AxisCtlProtoStuct GetStatusMsg(OTCXYZStaMsgUpdateReason);

		//get heartBeat/PositionUpdate msg
		AxisCtlProtoStuct GetHeartBeatMsg(OTCXYZHBUpdateReason);

	public:
		//ctor
		OTCXYZControllerCLS(CanBusChannelCls^ bus);

		// position update event 
		event PositonUpdateDelegate ^ PositionUpdateEvent;

		//x y z positon
		property double X_POS
		{
			double get();
			void set(double value);
		}

		property double Y_POS
		{
			double get();
			void set(double value);
		}

		property double Z_POS
		{
			double get();
			void set(double value);
		}

		
		property double A_POS
		{
			double get();
			void set(double );
		}

		property double B_POS
		{
			double get();
			void set(double );
		}

		//Operater Mode
		property ControlOpMode OP_MODE
		{
			ControlOpMode get();
		}

		//Operater Mode
		property ControlOpMode AB_OPMODE
		{
			ControlOpMode get();
		}


		//Move X axis forward
		void XStepUp();

		//Move X axis backward
		void XStepDown();

		//Move Y axis forward
		void YStepUp();

		//Move Y axis backward
		void YStepDown();

		//Move Z axis forward
		void ZStepUp();

		//Move Z axis backward
		void ZStepDown();

		//Move A axis forward
		void AStepUp();

		//Move A axis backward
		void AStepDown();

		//Move B axis forward
		void BStepUp();

		//Move B axis backward


		//press X release button
		void XRelBtnPress();

		//Release X release button
		void XRelBtnRelease();

		//press Y release button
		void YRelBtnPress();

		//Release Y release button
		void YRelBtnRelease();

		//Press Z release button
		void ZRelBtnPress();

		//Release Z release button
		void ZRelBtnRelease();

		//Press XYZ release button
		void XYZRelBtnPress();

		//Release XYZ release button
		void XYZRelBtnRelease();

		//Press A release button
		void ARelBtnPress();

		//Release A release button
		void ARelBtnRelease();

		//Press B release button
		void BRelBtnPress();

		//Release B release button
		void BRelBtnRelease();

		//Press Pass detent button
		void DtPassPress();

		//Release Pass detent button
		void DtPassRelease();

		void BStepDown();

		//Power up the OTCXYZ controller
		void PowerUp();

		//Power off the OTCXYZ controller
		void PowerOff();

		//receive event handler
		void OnCanDataReceivedEvent(List<AxisCtlProtoStuct> ^);

		//can data handler thread function
		void CanDataHandlerFunc();

		//OTC XYZ heartbeat thread function
		void HeartBeatFunc();


	};
}
