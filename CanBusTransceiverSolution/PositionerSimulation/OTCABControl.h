//otc ab definition

#pragma once

#include <Windows.h>
#include "TypeDefination.h"

using namespace System;
using namespace CanBusChannelNS;
using namespace SCBProtocalWrapper;
using namespace System::Collections::Generic;
using namespace System::Threading;
using namespace System::Xml;

namespace PositionerSimulationNS {

	public ref class OTCABControllerCLS
	{
	private:

		
		/***************************** the control type and software versions *******************************/
		//the hardware type (DMC/TMC) of the OTC controller
		static ControlHardType hardwareType = ControlHardType::Invalid;

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
		// the movement control system hardware status
		AStatus aStatus;
		BStatus bStatus;

		// the A B release button status,the hardware buttons are on OTC XYZ control,should get new status from OTC XYZ
		bool aRelBtnStatus, bRelBtnStatus;
	
		// the switch code of the OTCAB controller
		Byte swCode;

		// the control type (OTCAB)
		static ControlType type = ControlType::OTCAB;

		// the control running software mode
		static ControlHWMode runMode = ControlHWMode::BOOT;
		// current status of the OTCAB controller
		static ControlOpMode operateMode = ControlOpMode::NoInit;

		/***************************** OTC AB calibration status *******************************/
		bool AminSet,BminSet,AmaxSet,BmaxSet,PinCalSet;

		/***************************** OTC AB position status *******************************/
		double aPosition,bPosition;

		double aMinPos, aMaxPos, bMinPos, bMaxPos;

		double aMotorPos,aDevicePos, aServErr, aCurrent, aVoltage, aVel,aAep,aPot,aLLiPos;

		double bMotorPos,bDevicePos, bServErr, bCurrent, bVoltage, bVel,bAep,bPot,bLLiPos;

		double moveStepLength;
		/***************************** controller network system  *******************************/
		//the can bus which OTCXYZ controller will connect on
		CanBusChannelCls^ canBus;


		/***************************** class members  *******************************/

		//the receive buffer queue 
		List<AxisCtlProtoStuct> ^ otcABReceiveQueue;

		//the remote diagnostic cmd buffer
		array<wchar_t> ^ diagCMD;

		//the received cmd count
		int receivedCMD ;

		//the cmd length
		int cmdLength;

		//convert remote diagnostic command response to can frames
		List<AxisCtlProtoStuct>^ ConvertDiagResponse(String^ response);

		//remote diagnostic command parser method
		void RemoteDiagHandler();


		// Auto movement thread
		Thread ^ autoMoveThread;

		// Auto movement AB destination
		double aDestination,bDestination;

		// Auto movement XYZ destination and flag
		double xDestination, yDestination, zDestination;
		bool xyzAutomoveFlag, xyzAutoFinishFlag;

		// Auto movement WSD destination and flag
		double wsdXDestination, wsdYDestination, wsdZDestination, wsdTiltDestination;
		bool WSDAutoMoveFlag, WSDAutoFinishFlag;

		// Auto movement TBL destination
		double tblXDestination, tblYDestination, tblZDestination;
		bool TBLAutoMoveFlag, TBLAutoFinishFlag;

		// Auto movement step length
		static double autoMoveStep = 2;

		// Auto movement continue needed flag
	    bool autoContinueNeed ;

		// Auto movement autoreset event
		static AutoResetEvent ^ automoveEvent = gcnew AutoResetEvent(true);

		// Auto movement autoreset event 
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
		void HBUpdateToSCB(OTCABHBUpdateReason);

		//update status to scb
		void StatusUpdateToSCB(ABStaUpdateReason);
		
		/***************************** response generator  *******************************/

		//get controller configuration message
		List<AxisCtlProtoStuct> ^ GetConfigurationRsp();

		//get controller status message
		AxisCtlProtoStuct GetStatusMsg(ABStaUpdateReason);

		//get heartBeat/PositionUpdate msg
		AxisCtlProtoStuct GetHeartBeatMsg(OTCABHBUpdateReason);

	public:
		//ctor
		OTCABControllerCLS(CanBusChannelCls^ bus);

	
		//A, B positon
		property double A_POS
		{
			double get();
			void set(double value);
		}

		property double B_POS
		{
			double get();
			void set(double value);
		}

		property double MOVE_STEP
		{
			double get();
			void set(double value);
		}

		// the A Release button status
		property bool A_REL_BTN
		{
			bool get();
			void set(bool value);
		}

		// the B Release button status
		property bool B_REL_BTN
		{
			bool get();
			void set(bool value);
		}

		// the control operator mode
		property ControlOpMode OP_MODE
		{
			ControlOpMode get();
			void set(ControlOpMode value);
		}


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
