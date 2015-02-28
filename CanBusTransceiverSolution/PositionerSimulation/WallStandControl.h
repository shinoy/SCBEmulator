//otc ab definition

#pragma once

#include <Windows.h>
#include "TypeDefination.h"

using namespace System;
using namespace CanBusChannelNS;
using namespace SCBProtocalWrapper;
using namespace System::Collections::Generic;
using namespace System::Threading;

namespace PositionerSimulationNS {

	public enum class DetectorType:int{Trixell,Varian,DRX1,Undefined};

	public ref class WSDKeypadCLS
	{
	private:
		//the can bus which WSD controller will connect on
		CanBusChannelCls^ canBus;
		// Simple Motion Swtich
		bool hsUpSw, lsUpSw, hsDownSW, lsDownSw, hsLfSw, lsLfSw, hsRgSw, lsRgSw;
		// Tilt Switch
		bool tiltUpSw, tiltDownSw;
		// Complex Motion Switch
		bool autoCenterSW, autoTrackSW, autoPosSW, autoCenter2SW;
		//  Keypad Indicators 
		bool autoTrackInd, autoPosInd, autoCenterInd;
		// Collimator Preset Switch
		bool PreSet1SW, PreSet2SW, PreSet3SW;
		// Collimator Blade Position Swtich
		bool openHoriBladeSW, closeHoriBladeSW, openVertBladeSW, closeVertBladeSW, openHoriBladeTwSW, closeHorBladeTwSW, openVertBladeTwSW, closeVertBladeTwSW;

		// Collimator Lamp Swtich
		bool lampSW;


	public:
		// constructor
		WSDKeypadCLS(CanBusChannelCls ^ bus);

		// Simple Motion Swtich
		property bool AUTO_POS_SW
		{
			bool get();
			void set(bool value);
		}

		property bool AUTO_POS_IND
		{
			bool get();
			void set(bool);
		}

		property bool AUTO_CENTER_SW
		{
			bool get();
			void set(bool);
		}

		property bool AUTO_CENTER_IND
		{
			bool get();
			void set(bool);
		}

	};

	public ref class WSDControllerCLS
	{
	private:

		// debug count
		long count;

		/***************************** the control type and software versions *******************************/
		//the hardware type (DMC/TMC) of the WSD controller
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
		// the movement control system hardware status
		WSDMotionControls montionControl;
		
		// if handheld keypad installed
		bool keyPadInstalled;

		// the handheld keypad number
		UINT8 keyPadNum;

		// the detector type in WSD control
		DetectorType detectorType;

		// the detector id
		UINT8 detectorID;

		// the switch status of WSD control
		WSDSWStatus swStatus;

		// Grid Type
		GridType gridType;

		// Detector Sensor
		DetGridSensor detectorSensor;
		
		// Bucky Sensors;
		Sensors sensors;

		// the WSD Layout
		WSDRoomLayout layout;

		// the Z X release and ESTOP button status,the hardware buttons are on Wall Stand control,should get new status from wall stand
		bool zRelBtnStatus, xRelBtnStatus, eStopBtnStatus;

		// the switch code of the OTCAB controller
		Byte swCode;

		// the control type (OTCAB)
		static ControlType type = ControlType::WSD;

		// the control running software mode
		static ControlHWMode runMode = ControlHWMode::BOOT;
		// current status of the OTCAB controller
		static ControlOpMode operateMode = ControlOpMode::NoInit;

		/***************************** WSD detent status *******************************/
		UINT8 xDetent, zDetent, tiltDetent;

		/***************************** WSD calibration status *******************************/
		bool zMinSet, zMaxSet, xMinSet, xMaxSet, horRMCalDone, vertRMCalDone,pinCalDone;
		bool tiltMinSet, tiltVertSet, tiltHorSet;

		/***************************** WSD position status *******************************/
		double xPosition, yPosition,zPosition,tiltPosition,angPosition;

		double xPosMinLim, xPosMaxLim, zPosMinLim, zPosMaxLim, tiltPosMaxLim, tiltPosMinLim;

		double xmotorPos, xdevicePos, xabsEncoder, xserErr, xpot, xcurrent, xvoltage, xvel,xlliPos;

		double zmotorPos, zdevicePos, zabsEncoder, zserErr, zpot, zcurrent, zvoltage, zvel, zlliPos;

		double tiltmotorPos, tiltdevicePos, tiltabsEncoder, tiltserErr, tiltpot, tiltcurrent, tiltvoltage, tiltvel;

		double axisMoveStepLength;
		double angleMoveStepLength;
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
		List<AxisCtlProtoStuct> ^ wsdReceiveQueue;

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


		
		// Auto movement thread
		Thread ^ autoMoveThread;
		// Auto movement WSD destination
		double xDestination, yDestination, zDestination,tiltDestination;
		// Auto movement XYZ destination and flag
		double xyzXDestination, xyzYDestination, xyzZDestination;
		bool XYZAutoMoveFlag, XYZAutoFinishFlag;
		// Auto movement AB destination and Flag
		double aDestination, bDestination;
		bool ABAutoMoveFlag, ABAutoFinishFlag;
		// Auto movement TBL destination
		double tblXDestination, tblYDestination, tblZDestination;
		bool TBLAutoMoveFlag, TBLAutoFinishFlag;

		// Auto movement step length
		static double autoMoveAxisStep = 20;
		static double autoMoveAngStep = 5;
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



		//can data handle threading
		ThreadStart ^ canDataHandlerThreadStart;
		Thread^ canDataHanlerThread;

		//heart beat threading
		ThreadStart ^ heartBeatThreadStart;
		Thread^ heartBeatThread;

		//run boot strap on WSD controller,will check the SW code and initialize controller
		void RunBootCode();

		//run application code on WSD controller,will enable interrupt for receiving scb and other controllers' message.Will be in normal work logic
		void RunAppCode();

		//exit application code and reset the controller
		void ExitAppCode();

		//WSD control logic implement
		void ProcessProtocalData(List<AxisCtlProtoStuct> ^);

		//update control Positon/Cal status to scb
		void HBUpdateToSCB(WSDHBUpdateReason);

		//update status to scb
		void StatusUpdateToSCB(WSDStatusMsgUpdateReason);

		//update tilt and angulation to scb
		void TiltAngPosUpdateToSCB(WSDTiltPosUpdateReason);

		/***************************** response generator  *******************************/

		//get controller configuration message
		List<AxisCtlProtoStuct> ^ GetConfigurationRsp();

		//get controller status message
		AxisCtlProtoStuct GetStatusMsg(WSDStatusMsgUpdateReason);

		//get heartBeat/PositionUpdate msg
		AxisCtlProtoStuct GetHeartBeatMsg(WSDHBUpdateReason);

		//get WSD Tilt and Angulation Position msg
		AxisCtlProtoStuct GetWSDTiltAngMsg(WSDTiltPosUpdateReason reason);

	public:
		//ctor
		WSDControllerCLS(CanBusChannelCls^ bus);

		// Keypad 
		WSDKeypadCLS ^ keypad;

		// detector id
		property int DET_ID
		{
			int get();
		}

		//WSD positon
		//xPosition, yPosition, zPosition, tiltPosition, AngPosition;
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

		property double TILT_POS
		{
			double get();
			void set(double value);
		}

		property double ANG_POS
		{
			double get();
			void set(double value);
		}

		property double AXIS_MOVE_STEP
		{
			double get();
			void set(double value);
		}

		property double ANG_MOVE_STEP
		{
			double get();
			void set(double value);
		}

		//zRelBtnStatus, xRelBtnStatus, eStopBtnStatus;
		// the Z Release button status
		property bool Z_REL_BTN
		{
			bool get();
			void set(bool value);
		}

		// the X Release button status
		property bool X_REL_BTN
		{
			bool get();
			void set(bool value);
		}


		//Press Z release button
		void ZRelBtnPress();

		//Release Z release button
		void ZRelBtnRelease();

		//Press X release button
		void XRelBtnPress();

		//Release X release button
		void XRelBtnRelease();


		//Move Z axis forward
		void ZStepUp();

		//Move Z axis backward
		void ZStepDown();

		//Move X axis forward
		void XStepUp();

		//Move X axis backward
		void XStepDown();

		//Tilt Up
		void TiltUp();

		//Tilt Down
		void TiltDown();

		// the control operator mode
		property ControlOpMode OP_MODE
		{
			ControlOpMode get();
			void set(ControlOpMode value);
		}


		//Power up the WSD controller
		void PowerUp();

		//Power off the WSD controller
		void PowerOff();

		//receive event handler
		void OnCanDataReceivedEvent(List<AxisCtlProtoStuct> ^);

		//can data handler thread function
		void CanDataHandlerFunc();

		//WSD heartbeat thread function
		void HeartBeatFunc();

		//WSD Detector Insert
		void InsertDetector(Byte detectorId);

		//WSD Detector Eject
		void EjectDetector();
		
		//change the grid type
		void ChangeGridType(GridType);


	};
}
