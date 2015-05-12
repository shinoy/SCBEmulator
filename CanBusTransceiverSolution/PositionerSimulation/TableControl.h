//Table definition

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

	public enum class DetectorType :int{ Trixell, Varian, DRX1, Undefined };

	[Serializable]
	public ref class TBLControllerCLS
	{
	private:

		/***************************** the control type and software versions *******************************/
		//the hardware type (DMC/TMC) of the TBL controller
		static ControlHardType hardwareType = ControlHardType::DMC;

		// the hardware version of controller
		UINT8 hardwareVersion;

		// the boot tag version
		UINT8 bootTagVersion;

		// the boot revision
		WORD bootRevision;

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
		TBLMtControls motionControl;

		// if handheld keypad installed
		bool keyPadInstalled;

		// the detector type in TBL control
		DetectorType detectorType;

		// the detector id
		UINT8 detectorID;

		// the switch status of TBL control
		TBLSwStatus swStatus;

		// Bucky Sensors;
		TBLMtSensors sensors;

		// Handheld Keypad Switch Status of TBL
		TBLKeyPadStatus keypadStatus;

		// Indicator Status of TBL
		TBLIndicatorStatus indicatorStatus;

		// the TBL Layout
		 TBLRoomLayout layout;

		// the switch code of the TBL controller
		Byte swCode;

		// the control type (TBL)
		static ControlType type = ControlType::TBL;

		// the control running software mode
		static ControlHWMode runMode = ControlHWMode::BOOT;
		// current status of the OTCAB controller
		static ControlOpMode operateMode = ControlOpMode::NoInit;

		/***************************** TBL detent status *******************************/
		UINT8 buckyDetent, zDetent;

		/***************************** TBL calibration status *******************************/
		bool zMinSet, zMaxSet, longMinSet, longMaxSet, tblocEst, rmCalDone, pinCalDone;
		/***************************** TBL position status *******************************/
		double xPosition, yPosition, zPosition;

		double xPosMinLim, xPosMaxLim, zPosMaxLim, zPosMinLim;

		double xmotorPos, xdevicePos, xabsEncoder, xserErr, xpot, xcurrent, xvoltage, xvel, xlliPos;

		double zmotorPos, zdevicePos, zabsEncoder, zserErr, zpot, zcurrent, zvoltage, zvel, zlliPos;

		double axisMoveStepLength;

		double motoMoveStepLength;
		/***************************** controller network system  *******************************/
		//the can bus which WSD controller will connect on
		CanBusChannelCls^ canBus;

		/***************************** control file upload and download information  *******************************/
		//the upload file size
		UInt32 uploadSize;

		//the number of blocks need to be updated
		UInt16 lastUpSeqNum;

		//current block size
		UInt16 currentUpBlockSize;

		//current block sequence number
		UInt16 currentUpSeqNum;


		//the upload file sequence number

		/***************************** class members  *******************************/
		//the receive buffer queue 
		List<AxisCtlProtoStuct> ^ tblReceiveQueue;

		//the synchronization object
		static Object ^ receiveLockObject = gcnew Object();


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



		// Auto  movement thread
		Thread ^ autoMoveThread;

		// Auto movement TBL destination
		double xDestination, yDestination,zDestination;

		// Auto movement XYZ destination and flag
		double xyzXDestination, xyzYDestination, xyzZDestination;
		bool XYZAutoMoveFlag, XYZAutoFinishFlag;

		// Auto movement AB destination and flag
		double aDestination, bDestination;
		bool ABAutoMoveFlag, ABAutoFinishFlag;

		// Auto movement WSD destination and flag
		double wsdXDestination, wsdYDestination, wsdZDestination;
		bool WSDAutoMoveFlag, WSDAutoFinishFlag;
		
		// Auto movement step length
		static double autoMoveStep = 20;

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
		static AutoResetEvent ^ waitForBuckyStart = gcnew AutoResetEvent(false);

		//LT movement threading
		ThreadStart ^ ltMoveThreadStart;
		Thread^ ltMoveThread;

		//LT movement function
		void LTMove();


		//can data handle threading
		ThreadStart ^ canDataHandlerThreadStart;
		Thread^ canDataHanlerThread;

		//heart beat threading
		ThreadStart ^ heartBeatThreadStart;
		Thread^ heartBeatThread;

		//Z motorized movement threading
		ThreadStart ^ zMotoMoveThreadStart;
		Thread^ zMotoMoveThread;

		

		//run boot strap on WSD controller,will check the SW code and initialize controller
		void RunBootCode();

		//run application code on WSD controller,will enable interrupt for receiving scb and other controllers' message.Will be in normal work logic
		void RunAppCode();

		//exit application code and reset the controller
		void ExitAppCode();

		//WSD control logic implement
		void ProcessProtocalData(List<AxisCtlProtoStuct> ^);

		//update control Positon/Cal status to scb
		void HBUpdateToSCB(TBLHBUpdateReason);

		//update status to scb
		void StatusUpdateToSCB(TBLStatusUpdateReason);

		//motorized Z movement function
		void MotoZMove();

		


		/***************************** response generator  *******************************/

		//get controller configuration message
		List<AxisCtlProtoStuct> ^ GetConfigurationRsp();

		//get controller status message
		AxisCtlProtoStuct GetStatusMsg(TBLStatusUpdateReason);

		//get heartBeat/PositionUpdate msg
		AxisCtlProtoStuct GetHeartBeatMsg(TBLHBUpdateReason);

	public:
		//ctor
		TBLControllerCLS(CanBusChannelCls^ bus);

		// detector id
		property int DET_ID
		{
			int get();
		}

		//TBL positon
		//xPosition, yPosition, zPosition
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

	
		property double AXIS_MOVE_STEP
		{
			double get();
			void set(double value);
		}

		// the TBL buttons

		//Auto Center Switch
		property bool AUTOCENTER_SW
		{
			bool get();
			void set(bool value);
		}

		// Auto Track Switch
		property bool AUTOTK_SW
		{
			bool get();
			void set(bool value);
		}

		// TBL Estop button
		property bool ESTOP_SW
		{
			bool get();
			void set(bool value);
		}

		// TBL Z up button
		property bool ZUP_SW
		{
			bool get();
			void set(bool value);
		}

		// TBL Z Down button
		property bool ZDOWN_SW
		{
			bool get();
			void set(bool value);
		}

		// TBL Float button
		property bool FLOAT_SW
		{
			bool get();
			void set(bool value);
		}

		//Press Z up button
		void ZUpBtnDown();

		//Relase Z up button
		void ZUpBtnUp();

		//Press Z down button
		void ZDownBtnDown();

		//Relase Z down button
		void ZDownBtnUp();

		//Press Auto Center Button
		void AutoCenterBtnDown();

		//Release Auto Center Button
		void AutoCenterBtnUp();

		//Press Auto Track Button
		void AutoTKBtnDown();

		//Release Auto Track Button
		void AutoTKBtnUp();

		//Press Estop Button
		void EstopBtnDown();

		//Release Estop Button
		void EstopBtnUp();

		//Press Float Button
		void FloatBtnDown();

		//Release Flat Button
		void FloatBtnUp();

		//TBL X forward
		void XStepUp();

		//TBL X backward
		void XStepDown();

		//TBL Y forward
		void YStepUp();

		//TBL Y backward
		void YStepDown();



		// the control operator mode
		property ControlOpMode OP_MODE
		{
			ControlOpMode get();
		}


		//Power up the WSD controller
		void PowerUp();

		//Power off the WSD controller
		void PowerOff();

		//receive event handler
		void OnCanDataReceivedEvent(List<AxisCtlProtoStuct> ^);

		//can data handler thread function
		void CanDataHandlerFunc();

		//TBL heartbeat thread function
		void HeartBeatFunc();

		//TBL Insert detector
		void InsertDetector(Byte detectorId);

		//TBL Eject detector
		void EjectDetector();

		//TBL Grid setting
		void SetGrid(bool);

		//TBL BuckyStart Event Handler
		void BuckyStartEventHandler();

	};
}
