//otc xyz definition

#pragma once

#include <Windows.h>
#include "TypeDefination.h"

using namespace System;
using namespace CanBusChannelNS;
using namespace SCBProtocalWrapper;
using namespace System::Collections::Generic;
using namespace System::Threading;


namespace PositionerSimulationNS {

	// collimator blades position update delegate
	public delegate void BladePositionUpdateDelegate();

	public ref class CollimatorCLS
	{
	private:

	
		/***************************** the control mode and software versions *******************************/
		static String ^ asr1Model = "LH10F100";
		static String ^ asr1SN = "S/N00001";
		static String ^ asr2Model = "CH10F304";
		static String ^ asr2SN = "S/N00000";

		int  vSid, lSid;
		int  crossBladeSize, longBladeSize;
		static Byte filterType;
		
		bool lampOn;
		static bool isPowOn = false;
		double lampTime, rampTime;

		CollimatorMode opMode;

		static bool manSwitchStatus;
	
		/***************************** controller network system  *******************************/
		//the can bus which OTCXYZ controller will connect on
		CanBusChannelCls^ canBus;


		/***************************** class members  *******************************/
		//the receive buffer queue 
		List<AxisCtlProtoStuct> ^ collimatorReceiveQueue;

		//the synchronization object
		static Object^ receiveLockObject = gcnew Object();

		//can data handle threading
		ThreadStart ^ canDataHandlerThreadStart;
		Thread^ canDataHanlerThread;

		//heart beat threading
		ThreadStart ^ heartBeatThreadStart;
		Thread^ heartBeatThread;

		// initialize Collimator
		void RunBootCode();

		//run application code on Collimator,will enable interrupt for receiving scb and other controllers' message
		void RunAppCode();

		//exit application code and reset the controller
		void ExitAppCode();

		//Collimator logic implement
		void ProcessProtocalData(List<AxisCtlProtoStuct> ^);

		////update Collimator status to scb
		//void HBUpdateToSCB( );

		//update control status to scb
		void StatusUpdateToSCB( );


		//Collimator heartbeat thread function
		void HeartBeatFunc();

		//Collimator lampcycle thread function
		void LampCycleFunc();

		/***************************** response generator  *******************************/

		////get controller configuration message
		//List<AxisCtlProtoStuct> ^ GetConfigurationRsp();

		////get controller status message
		//AxisCtlProtoStuct GetStatusMsg();

		////get heartBeat/PositionUpdate msg
		//AxisCtlProtoStuct GetHeartBeatMsg();

	public:
		//ctor
		CollimatorCLS(CanBusChannelCls^ bus);

		//
		event BladePositionUpdateDelegate ^ BladePositionUpdateEvent;

		//Power up the Collimator
		void PowerUp();

		//Power off the Collimator
		void PowerOff();

		//receive event handler
		void OnCanDataReceivedEvent(List<AxisCtlProtoStuct> ^);

		//can data handler thread function
		void CanDataHandlerFunc();

		//collimator power status
		property bool POW_ON
		{
			bool get();
		}

		//collimator manual switch
		property bool MAN_SWITCH_ON
		{
			void set(bool);
			bool get();
		}

		//Long blades position
		property int LONG_BLADE
		{
			void set(int);
			int get();
		}

		//Cross blades position
		property int CROSS_BLADE
		{
			void set(int);
			int get();
		}

		//Filter position selection
		property byte FILTER_POSITION
		{
			void set(byte);
			byte get();
		}
	};
}
