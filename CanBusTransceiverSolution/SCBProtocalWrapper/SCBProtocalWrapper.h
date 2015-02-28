// SCBProtocalWrapper.h

#pragma once
#include "Stdafx.h"
#include "TypeDefination.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace CanBusChannelNS;


/// the controller hardware type enum
public enum class ControlHardType :int{ DMC, TMC ,Invalid};
/// the controller hardware type enum
public enum class ControlType :int{ OTCXYZ, OTCAB, WSD, TBL };

/// the controller operator mode enum
public enum class ControlOpMode :int{NoInit,Init,NMtPara,MLock,ESTOP,NRmCal,SoftDTMod,MMReq,AMTP,TkWSD,TkTBL,TKOTC,MtNoInit,LT,AsyCollim,Idle,AsyTkWSD};

namespace SCBProtocalWrapper {

	/*************************  SCB to All Controllers Messages  *************************/

	/******************************************/
	//     1. SCB Controller Reset(0x100)      /
	/******************************************/

	/// controllers reset message (0X100)
	[StructLayout(LayoutKind::Explicit, Size = 24)]
	public value struct SCBCtlReset
	{
		[FieldOffset(0)] Byte Byte0;
		[FieldOffset(1)] WORD id;
	};

	/// the service class for controllers reset messages
	public ref class SCBCtlResetPS
	{

	private:
		bool OTCXYZReset ;
		bool OTCABReset ;
		bool WSDReset ;
		bool TBLReset ;

	public:
		SCBCtlReset data;

		// the property if the OTCXYZ controller will be reset
		property bool RESET_OTCXYZ
		{
			bool get();
			void set(bool value);
		}

		// the property if the OTCAB controller will be reset
		property bool RESET_OTCAB
		{
			bool get();
			void set(bool value);
		}

		// the property if the WSD controller will be reset
		property bool RESET_WSD
		{
			bool get();
			void set(bool value);
		}

		// the property if the TBL controller will be reset
		property bool RESET_OTCTBL
		{
			bool get();
			void set(bool value);
		}

		//ctor
		SCBCtlResetPS();
		SCBCtlResetPS(Byte segment);


	};


	/******************************************/
	//     2. SCB Auto Move Abort(0x101)      /
	/******************************************/
	///  SCB Auto Move Abort(0x101) message
	[StructLayout(LayoutKind::Explicit, Size = 16)]
	public value struct SCBAutoAbt
	{
		[FieldOffset(0)] WORD id;
	};

	/// the service class for scb auto movement abort message
	public ref class SCBAutoAbtPS
	{
	public:
		SCBAutoAbt data;

		SCBAutoAbtPS();

	};

	/******************************************/
	//     3. SCB Auto Move Enable(0x102)      /
	/******************************************/

	/// SCB Auto Move Enable(0x102) message
	[StructLayout(LayoutKind::Explicit, Size = 24)]
	public value struct SCBAutoEn
	{
		[FieldOffset(0)] Byte Byte0;
		[FieldOffset(1)] WORD id;
	};

	///the service class for scb auto movement enable
	public ref class SCBAutoEnPS
	{
	private:
		bool ContinueNeed ;

	public:
		SCBAutoEn data;

		property bool CONTINUE_NEED
		{
			bool get();
			void set(bool value);
		}

		//ctor 
		SCBAutoEnPS(AxisCtlProtoStuct frame);

	};


	/******************************************/
	//      4. SCB Verify Auto Move(0x103)     /
	/******************************************/
	/// SCB Verify Auto Move(0x103) message
	[StructLayout(LayoutKind::Explicit, Size = 16)]
	public value struct SCBAutoVfy
	{
		[FieldOffset(0)] WORD id;
	};

	///the service class for scb auto movement verify
	public ref class SCBAutoVfyPS
	{
	public:
		SCBAutoVfy data;

		SCBAutoVfyPS();

	};


	/******************************************/
	//     5. SCB Auto Move Continue(0x104)    /
	/******************************************/
	/// SCB Auto Move Continue(0x104) message
	[StructLayout(LayoutKind::Explicit, Size = 16)]
	public value struct SCBAutoCont
	{
		[FieldOffset(0)] WORD id;
	};

	///the service class for scb auto movement continue
	public ref class SCBAutoContPS
	{
	public:
		SCBAutoCont data;

		SCBAutoContPS();

	};



	/**********************************************************/
	//      6. SCB Set Tomographic Parameters (0X124)          /
	/**********************************************************/
	/// SCB Set Tomographic Parameters (0X124) message (TBD)
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct SCBTomoPara
	{
		// Change Request
		[FieldOffset(0)] Byte RequestType;
		// Tomo Sweep Angle
		[FieldOffset(1)] Byte SweepAngle;

		// Object-Table Distance or "Layer Height"(mm) (LSB)
		[FieldOffset(2)] Byte LayerHeightL;
		// Object-Table Distance or "Layer Height"(mm) (MSB)
		[FieldOffset(3)] Byte LayerHeightM;
		// Object-Table Distance or "Layer Height"(mm)
		[FieldOffset(2)] WORD LayerHeight;

		//Scan Sweep Time(ms) (LSB)
		[FieldOffset(4)] Byte SweepTimeL;
		//Scan Sweep Time(ms) (MSB)
		[FieldOffset(5)] Byte SweepTimeM;
		//Scan Sweep Time(ms) 
		[FieldOffset(4)] WORD SweepTime;

		//Scan "Delay" Time(ms) (LSB)
		[FieldOffset(6)] Byte DelayTimeL;
		//Scan "Delay" Time(ms) (MSB)
		[FieldOffset(7)] Byte DelayTimeM;
		//Scan "Delay" Time(ms)
		[FieldOffset(6)] WORD DelayTime;

		[FieldOffset(8)] WORD id;
	};

	public ref class SCBTomoParaPS
	{

	private:

		bool tomoTrue;
		UInt16 sweepAngle, layerHeight, sweepTime, delayTime;

	public:

		SCBTomoPara data;

		property bool TOMO_TRUE
		{
			bool get();
		}

		property UInt16 SWEEP_ANGLE
		{
			UInt16 get();
		}

		property UInt16 LAYER_HEIGHT
		{
			UInt16 get();
		}

		property UInt16 SWEEP_TIME
		{
			UInt16 get();
		}

		property UInt16 DELAY_TIME
		{
			UInt16 get();
		}

		SCBTomoParaPS(AxisCtlProtoStuct);
	};

	/************************************************/
	//    7. SCB Controller Status Request(0x410)    /
	/************************************************/
	/// SCB Controller Status Request(0x410) message
	[StructLayout(LayoutKind::Explicit, Size = 32)]
	public value struct SCBCtrlStatusReq
	{
		[FieldOffset(0)] Byte CommandTarget;
		[FieldOffset(1)] Byte RequestType;
		[FieldOffset(0)] WORD Frame;
		[FieldOffset(2)] WORD id;
	};

	/// the service class for scb controller status request
	public ref class SCBCtrlStatusReqPS
	{
	private:
		bool OTCXYZrt, OTCABrt, WSDrt, TBLrt;
		bool PosUpdateReq, ContStaReq, KeyStaReq, ErrStaReq, LLIPosReq;

	public:
		SCBCtrlStatusReq data;

		property bool OTCXYZ_RT
		{
			bool get();
			void set(bool value);
		}

		property bool OTCAB_RT
		{
			bool get();
			void set(bool value);
		}

		property bool WSD_RT
		{
			bool get();
			void set(bool value);
		}

		property bool TBL_RT
		{
			bool get();
			void set(bool value);
		}

		property bool POSUPDATE_REQ
		{
			bool get();
			void set(bool value);
		}


		property bool CONTSTA_REQ
		{
			bool get();
			void set(bool value);
		}

		property bool KEYSTA_REQ
		{
			bool get();
			void set(bool value);
		}

		property bool ERRSTA_REQ
		{
			bool get();
			void set(bool value);
		}

		property bool LLIPOS_REQ
		{
			bool get();
			void set(bool value);
		}

		//ctor
		SCBCtrlStatusReqPS();
		SCBCtrlStatusReqPS(AxisCtlProtoStuct );

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
		
	};


	/*************************************************************/
	//    8. SCB Controller Soft Detent Status Request (0x411)    /
	/*************************************************************/
	/// SCB Controller Soft Detent Status Request (0x411) message
	[StructLayout(LayoutKind::Explicit, Size = 32)]
	public value struct SCBSoftDtReq
	{
		[FieldOffset(0)] Byte CommandTarget;
		[FieldOffset(1)] Byte MotionPlane;
		[FieldOffset(0)] WORD Frame;
		[FieldOffset(2)] WORD id;
	};

	/// the service class for scb controller status request
	public ref class SCBSoftDtReqPS
	{
	private:
		bool OTCXYZdt, WSDdt, TBLdt;
		bool ZPlane, XPlane, YPlane;

	public:
		SCBSoftDtReq data;

		// if the OTC XYZ controller should return its requested detent status
		property bool OTCXYZ_DT
		{
			bool get();
			void set(bool);
		}

		// if the wall stand controller should return its requested detent status
		property bool WSD_DT
		{
			bool get();
			void set(bool);
		}
		// if the table controller should return its requested detent status
		property bool TBL_DT
		{
			bool get();
			void set(bool);
		}
		// Z plane detent
		property bool ZPLANE
		{
			bool get();
			void set(bool);
		}
		// X plane detent
		property bool XPLANE
		{
			bool get();
			void set(bool);
		}
		// Y plane detent
		property bool YPLANE
		{
			bool get();
			void set(bool);
		}
		// ctor
		SCBSoftDtReqPS();
		SCBSoftDtReqPS(WORD segment);

	};


	/*************************************************/
	//    9. SCB Controller Set Soft Detent (0x420)   /
	/*************************************************/
	/// SCB Controller Set Soft Detent (0x420) message
	[StructLayout(LayoutKind::Explicit, Size = 40)]
	public value struct SCBSoftDtSet
	{
		[FieldOffset(0)] Byte CommandTarget;
		[FieldOffset(1)] Byte MotionPlane;
		[FieldOffset(2)] Byte DetentIndex;
		[FieldOffset(3)] WORD id;
	};
	/// the service class for scb controller set soft detent 
	public ref class SCBSoftDtSetPS
	{
	private:
		bool OTCXYZdt, WSDdt, TBLdt;
		bool ZPlane, XPlane, YPlane;

	public:
		SCBSoftDtSet data;

		// need to set OTCXYZ detent
		property bool OTCXYZ_DT
		{
			bool get();
			void set(bool);
		}
		// need to set WSD detent
		property bool WSD_DT
		{
			bool get();
			void set(bool);
		}
		// need to set TBL detent
		property bool TBL_DT
		{
			bool get();
			void set(bool);
		}
		// Z plane detent
		property bool ZPLANE
		{
			bool get();
			void set(bool);
		}
		// X plane detent
		property bool XPLANE
		{
			bool get();
			void set(bool);
		}
		// Y plane detent
		property bool YPLANE
		{
			bool get();
			void set(bool);
		}
		// Soft Detent Index
		property UINT8 SOFT_INDEX
		{
			UINT8 get();
			void set(UINT8 index);
		}

		//ctor
		SCBSoftDtSetPS();
		SCBSoftDtSetPS(array<Byte> ^ segment);
	};

	/*************************************************/
	//   10. SCB Controller Clear Soft Detent (0x421) /
	/*************************************************/
	/// SCB Controller Clear Soft Detent (0x421) message
	[StructLayout(LayoutKind::Explicit, Size = 40)]
	public value struct SCBSoftDtClear
	{
		[FieldOffset(0)] Byte CommandTarget;
		[FieldOffset(1)] Byte MotionPlane;
		[FieldOffset(2)] Byte DetentIndex;
		[FieldOffset(3)] WORD id;
	};
	/// the service class for scb controller set soft detent
	public ref class SCBSoftDtClearPS
	{
	private:
		bool OTCXYZdt, WSDdt, TBLdt;
		bool ZPlane, XPlane, YPlane;

	public:
		SCBSoftDtClear data;

		// need to clear OTCXYZ detent
		property bool OTCXYZ_DT
		{
			bool get();
			void set(bool);
		}
		// need to clear WSD detent
		property bool WSD_DT
		{
			bool get();
			void set(bool);
		}
		// need to clear TBL detent
		property bool TBL_DT
		{
			bool get();
			void set(bool);
		}
		// Z plane detent
		property bool ZPLANE
		{
			bool get();
			void set(bool);
		}
		// X plane detent
		property bool XPLANE
		{
			bool get();
			void set(bool);
		}
		// Y plane detent
		property bool YPLANE
		{
			bool get();
			void set(bool);
		}
		// Soft Detent Index
		property UINT8 SOFT_INDEX
		{
			UINT8 get();
			void set(UINT8 index);
		}

		//ctor
		SCBSoftDtClearPS();
		SCBSoftDtClearPS(array<Byte> ^ segment);

	};

	/*******************************/
	//   11. SCB Clear Cal (0x422 ) /
	/*******************************/
	/// SCB Clear Calculation (0x422) message
	[StructLayout(LayoutKind::Explicit, Size = 32)]
	public value struct SCBClrCal
	{
		[FieldOffset(0)] Byte CommandTarget;
		[FieldOffset(1)] Byte CalibrationType;
		[FieldOffset(2)] WORD id;
	};

	/// the service class for scb clear calculation
	public ref class SCBClrCalPS
	{
	private:
		bool OTCClr, WSDClr, TBLClr;
		bool CalType1, CalType2, CalType3, CalType4, CalType5, CalType6, CalType7, PinCal;

	public:
		SCBClrCal data;

		// if the OTC calibration should be cleared
		property bool OTC_CLEAR
		{
			bool get();
			void set(bool value);
		}
		// if the WSD calibration should be cleared
		property bool WSD_CLEAR
		{
			bool get();
			void set(bool value);
		}
		// if the TBL calibration should be cleared
		property bool TBL_Clear
		{
			bool get();
			void set(bool value);
		}

		// the calibration type 1, mapping XYZ-Z min , AB-Reserved , TBL-Z min , WSD-Z min
		property bool CAL_TYPE1
		{
			bool get();
			void set(bool value);
		}

		// the calibration type 2, mapping XYZ-X min , AB-Reserved , TBL-Long min ,WSD-reserved
		property bool CAL_TYPE2
		{
			bool get();
			void set(bool value);
		}

		// the calibration type 3, mapping XYZ-Y min, AB-Reserved, TBL-Z max, WSD-Z max
		property bool CAL_TYPE3
		{
			bool get();
			void set(bool value);
		}

		// the calibration type 4, mapping XYZ-Z max, AB-Reserved, TBL-Long max,WSD-X min
		property bool CAL_TYPE4
		{
			bool get();
			void set(bool value);
		}

		// the calibration type 5, mapping XYZ-X max, AB-Reserved, TBL-Reserved, WSD-X max
		property bool CAL_TYPE5
		{
			bool get();
			void set(bool value);
		}

		// the calibration type 6, mapping XYZ-Y max, AB-Reserved, TBL-Reserved, WSD- Hor.Room Cal
		property bool CAL_TYPE6
		{
			bool get();
			void set(bool value);
		}

		// the calibration type 7, mapping XYZ-Room Cal, AB-Reserved, TBL-Room Cal, WSD- Vert.Room Cal
		property bool CAL_TYPE7
		{
			bool get();
			void set(bool value);
		}

		// the calibration type 8, mapping XYZ-Pin Cal, AB-Pin Cal, TBL-Pin Cal, WSD-Pin Cal
		property bool PIN_TYPE
		{
			bool get();
			void set(bool value);
		}

		//ctor
		SCBClrCalPS();

		SCBClrCalPS(AxisCtlProtoStuct segment);

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};


	/*******************************/
	//   12. SCB Auto Track Mode (0x430 ) /
	/*******************************/
	/// SCB Auto Track Mode (0x430) message
	[StructLayout(LayoutKind::Explicit, Size = 24)]
	public value struct SCBAutoTrack
	{
		[FieldOffset(0)] Byte TrackTarget;
		[FieldOffset(1)] WORD id;
	};


	/// the auto track mode enum
	public enum class TrackMode :int{ ABORT_AT, OTC_ATWSD, OTC_ATTBL, WSD_ATOTC, TBL_ATOTC };

	/// the service class for auto track mode 
	public ref class SCBAutoTrackPS
	{
	private:
		TrackMode mode;
	public:
		SCBAutoTrack data;

		// the track mode property
		property TrackMode Mode
		{
			TrackMode get();
			void set(TrackMode);
		}

		//ctor
		SCBAutoTrackPS(TrackMode);
	};


	/*******************************************/
	//   16. SCB Configuration Request (0x461 ) /
	/*******************************************/
	/// SCB Configuration Request (0x461) message
	[StructLayout(LayoutKind::Explicit, Size = 24)]
	public value struct SCBConfigReq
	{
		[FieldOffset(0)] Byte ResponseReq;
		[FieldOffset(1)] WORD id;
	};

	/// the service class for scb configuration request
	public ref class SCBConfigReqPS
	{
	private:
		bool otcxyzReq, otcabReq, wsdReq, tblReq;

	public:
		SCBConfigReq data;

		property bool OTCXYZ_REQ
		{
			bool get();
			void set(bool value);
		}

		property bool OTCAB_REQ
		{
			bool get();
			void set(bool value);
		}

		property bool WSD_REQ
		{
			bool get();
			void set(bool value);
		}

		property bool TBL_REQ
		{
			bool get();
			void set(bool value);
		}

		SCBConfigReqPS(AxisCtlProtoStuct);

	};


	/*******************************************/
	//   17. SCB File Transfer Request (0x470)  /
	/*******************************************/
	/// SCB File Transfer Request (0x470) message
	[StructLayout(LayoutKind::Explicit, Size = 72)]
	public value struct SCBFileTranReqMsg
	{
		[FieldOffset(0)] Byte DownloadTarget;
		[FieldOffset(1)] Byte TransferType;
		[FieldOffset(2)] Byte TransSizeL;
		[FieldOffset(3)] Byte TransSizeC;
		[FieldOffset(4)] Byte TransSizeM;
		[FieldOffset(5)] Byte PlaceHolder;

		[FieldOffset(2)] DWORD TransSize;
		[FieldOffset(6)] Byte SFCode;
		[FieldOffset(7)] WORD id;
	};

	public enum class SCBDownloadTarget :int{OTCXYZ,OTCAB,WSD,TBL};

	public enum class SCBTransferType :int{BootStrap,BootCode,AppCode,FPGACode,MotorPara,CalFileDown,CalFileUp};

	public enum class SCBSFCode :int{TransferStart,TransferFinish};

	// the service class for SCB File Transfer Request (0x470)
	public ref class SCBFileTranReqMsgPS
	{
	private:
		SCBDownloadTarget downloadTarget;
		SCBTransferType transferType;
		UInt32  transferSize;
		SCBSFCode sfCode;

	public:
		SCBFileTranReqMsg data;

		property SCBDownloadTarget DOWNLOAD_TARGET
		{
			SCBDownloadTarget get();
			void set(SCBDownloadTarget value);
		}

		property SCBTransferType TRANSFER_TYPE
		{
			SCBTransferType get();
			void set(SCBTransferType value);
		}

		property UINT32 TRANSFER_SIZE
		{
			UINT32 get();
			void set(UInt32 value);
		}

		property SCBSFCode SFCODE
		{
			SCBSFCode get();
			void set(SCBSFCode value);
		}

		SCBFileTranReqMsgPS(AxisCtlProtoStuct frame);


	};


	/*******************************************/
	//   22. SCB File Transfer Response (0x490) /
	/*******************************************/
	// 22. SCB File Transfer Response (0x490) message
	[StructLayout(LayoutKind::Explicit, Size = 40)]
	public value struct SCBFileTranRespMsg
	{
		[FieldOffset(0)] Byte TransferSource;
		[FieldOffset(1)] Byte TransferType;
		[FieldOffset(2)] Byte Status;
		[FieldOffset(3)] WORD id;
	};

	public ref class SCBFileTranRespMsgPS
	{
	private:
		SCBDownloadTarget transferSource;
		SCBTransferType transferType;
		Byte status;

	public:
		SCBFileTranRespMsg data;

		property SCBDownloadTarget TRANS_SOURCE
		{
			SCBDownloadTarget get();
			void set(SCBDownloadTarget value);
		}

		property SCBTransferType TRANS_TYPE
		{
			SCBTransferType get();
			void set(SCBTransferType value);
		}

		property Byte STATUS
		{
			Byte get();
			void set(Byte value);
		}

		SCBFileTranRespMsgPS(AxisCtlProtoStuct);
	};

	/*******************************************/
	//   23. SCB Block Transfer Response (0x491) /
	/*******************************************/
	///  23. SCB Block Transfer Response (0x491) message
	[StructLayout(LayoutKind::Explicit, Size = 56)]
	public value struct SCBBLKTranRespMsg
	{
		[FieldOffset(0)] Byte TransferSource;
		// the actual size if byte size plus 1
		[FieldOffset(1)] Byte TransferSize;
		
		[FieldOffset(2)] Byte BLKSeqNumL;
		[FieldOffset(3)] Byte BLKSeqNumM;
		[FieldOffset(2)] WORD BLKSeqNum;

		[FieldOffset(4)] Byte Status;
		[FieldOffset(5)] WORD id;
	};

	/// the service class for  SCB Block Transfer Response (0x491)
	public ref class SCBBLKTranRespMsgPS
	{
	private:
		SCBDownloadTarget transferSource;
		UInt16 transferSize;
		UInt16 blockSeqNum;
		Byte Status;

	public:
		SCBBLKTranRespMsg data;

		property SCBDownloadTarget TRAN_SOURCE
		{
			SCBDownloadTarget get();
			void set(SCBDownloadTarget value);
		}

		property UInt16 TRAN_SIZE
		{
			UInt16 get();
			void set(UInt16 value);
		}

		property UInt16 BLK_SEQNUM
		{
			UInt16 get();
			void set(UInt16 value);
		}

		property Byte STATUS
		{
			Byte get();
			void set(Byte value);
		}

		SCBBLKTranRespMsgPS(AxisCtlProtoStuct);
	};

	/*************************  All Controllers to SCB Messages  *************************/


	/*****************************************************************/
	//   1. Axis Controller Sync Move Responses (0x125 through 0x127) /
	/*****************************************************************/
	///  1. Axis Controller Sync Move Responses (0x125 through 0x127) 
	[StructLayout(LayoutKind::Explicit, Size = 32)]
	public value struct AxisSyncRespMsg
	{
		[FieldOffset(0)] Byte SyncMoveStatus;
		[FieldOffset(1)] Byte ErrorCode;
		[FieldOffset(2)] WORD id;
	};

	public enum class SyncStatus: int{SyncACK =0,SyncDeny,SyncComplete,AbortReq,AbortErr,AbortMM};

	public ref class AxisSyncRespMsgPS
	{
	private:

		SyncStatus syncStatus;
		Byte errorCode;

	public:
		AxisSyncRespMsg data;

		property SyncStatus SYNC_STATUS
		{
			void set(SyncStatus value);
		}

		property Byte ERROR_CODE
		{
			void set(Byte value);
		}

		AxisSyncRespMsgPS(ControlType);

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};

	/*************************************************************/
	//   3. Axis Controller File Transfer Response (0x280)        /
	/*************************************************************/
	///  3. Axis Controller File Transfer Response (0x280) message
	[StructLayout(LayoutKind::Explicit, Size = 40)]
	public value struct AxisFileTranRespMsg
	{
		[FieldOffset(0)] Byte DownloadTarget;
		[FieldOffset(1)] Byte DownloadType;
		[FieldOffset(2)] Byte Status;
		[FieldOffset(3)] WORD id;
	};

	/// the service class for Axis Controller File Transfer Response
	public ref class AxisFileTranRespMsgPS
	{
	private:
		SCBDownloadTarget downloadTarget;
		SCBTransferType downloadType;
		Byte status;

	public:
		AxisFileTranRespMsg data;

		property SCBDownloadTarget DOWNLOAD_TARGET
		{
			SCBDownloadTarget get();
			void set(SCBDownloadTarget value);
		}

		property SCBTransferType DOWNLOAD_TYPE
		{
			SCBTransferType get();
			void set(SCBTransferType value);
		}

		property Byte STATUSCODE
		{
			Byte get();
			void set(Byte value);
		}

		AxisFileTranRespMsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};


	/*************************************************************/
	//   4. Axis Controller File Transfer Requests (0x295-0x298)  /
	/*************************************************************/
	/// Axis Controller File Transfer Requests (0x295-0x298) message
	[StructLayout(LayoutKind::Explicit, Size = 64)]
	public value struct AxisFileTranReqMsg
	{
		[FieldOffset(0)] Byte TransferType;
		[FieldOffset(1)] Byte TransferSizeL;
		[FieldOffset(2)] Byte TransferSizeC;
		[FieldOffset(3)] Byte TransferSizeM;
		[FieldOffset(4)] Byte PlaceHolder;
		[FieldOffset(1)] DWORD TransferSize;
		[FieldOffset(5)] Byte SFCode;
		[FieldOffset(6)] WORD id;
	};

	public ref class AxisFileTranReqMsgPS
	{
	private:
		UInt32 transferSize;
		SCBSFCode sfCode;

	public:
		AxisFileTranReqMsg data;

		property UInt32 TRANSFER_SIZE
		{
			UInt32 get();
			void set(UInt32 value);
		}

		property SCBSFCode SFCODE
		{
			SCBSFCode get();
			void set(SCBSFCode value);
		}

		AxisFileTranReqMsgPS(ControlType);

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};


	/*********************************************************************/
	//   5. Axis Controller File Transfer Axis Configuration (0x291)      /
	/*********************************************************************/
	//   5. Axis Controller File Transfer Axis Configuration (0x291) message
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct AxisFileTranCfgMsg
	{
		[FieldOffset(0)] Byte SWCodeB7;
		[FieldOffset(1)] Byte SWCodeB6;
		[FieldOffset(2)] Byte SWCodeB5;
		[FieldOffset(3)] Byte SWCodeB4;
		[FieldOffset(4)] Byte SWCodeB3;
		[FieldOffset(5)] Byte SWCodeB2;
		[FieldOffset(6)] Byte SWCodeB1;
		[FieldOffset(7)] Byte SWCodeB0;
		[FieldOffset(8)] WORD id;
	};

	// the service class for Axis Controller File Transfer Axis Configuration
	public ref class AxisFileTranCfgMsgPS
	{
	private:
		Byte swCode;

	public:

		AxisFileTranCfgMsg data;

		property Byte SWCODE
		{
			Byte get();
			void set(Byte value);
		}

		AxisFileTranCfgMsgPS(Byte);

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};



	/******************************************************/
	//   6. Axis Controller Block Upload Request (0x292)   /
	/******************************************************/
	/// 6. Axis Controller Block Upload Request (0x292) message
	[StructLayout(LayoutKind::Explicit, Size = 64)]
	public value struct CtlBLKUPReqMsg
	{
		[FieldOffset(0)] Byte TransferSource;
		// real transfer size is byte size plus 1
		[FieldOffset(1)] Byte TransferSize;

		[FieldOffset(2)] Byte BKLSeqNumL;
		[FieldOffset(3)] Byte BKLSeqNumM;
		[FieldOffset(2)] WORD BKLSeqNum;

		[FieldOffset(4)] Byte CheckSumL;
		[FieldOffset(5)] Byte CheckSumM;
		[FieldOffset(4)] WORD CheckSum;

		[FieldOffset(6)] WORD id;
	};

	/// the service class of Axis Controller Block Upload Request
	public ref class CtlBLKUPReqMsgPS
	{
	private:
		SCBDownloadTarget transferSource;
		UInt16 transferSize;
		UINT16 blockSeqNum;
		UInt16 checkSum;

	public:

		CtlBLKUPReqMsg data;

		property SCBDownloadTarget TRANS_SOURCE
		{
			SCBDownloadTarget get();
			void set(SCBDownloadTarget value);
		}

		property UInt16 TRANS_SIZE
		{
			UInt16 get();
			void set(UInt16 value);
		}

		property UInt16 BLOCK_SEQNUM
		{
			UInt16 get();
			void set(UInt16 value);
		}

		property UInt16 CHECKSUM
		{
			UInt16 get();
			void set(UInt16 value);
		}

		CtlBLKUPReqMsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};

	/**********************************************/
	//   7. Axis Controller Data Upload (0x293)    /
	/**********************************************/
	/// 7. Axis Controller Data Upload (0x293) message
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct AxisDataUpMsg
	{
		[FieldOffset(0)] Byte RecordByte0;
		[FieldOffset(1)] Byte RecordByte1;
		[FieldOffset(2)] Byte RecordByte2;
		[FieldOffset(3)] Byte RecordByte3;
		[FieldOffset(4)] Byte RecordByte4;
		[FieldOffset(5)] Byte RecordByte5;
		[FieldOffset(6)] Byte RecordByte6;
		[FieldOffset(7)] Byte RecordByte7;
		[FieldOffset(8)] WORD id;
	};

	public ref class AxisDataUpMsgPS
	{
	private:
		Byte recordByte0, recordByte1, recordByte2, recordByte3, recordByte4, recordByte5, recordByte6, recordByte7;

	public:
		AxisDataUpMsg data;

		UINT dataCount;

		property Byte RD_BYTE0
		{
			Byte get();
			void set(Byte value);
		}

		property Byte RD_BYTE1
		{
			Byte get();
			void set(Byte value);
		}
		property Byte RD_BYTE2
		{
			Byte get();
			void set(Byte value);
		}
		property Byte RD_BYTE3
		{
			Byte get();
			void set(Byte value);
		}
		property Byte RD_BYTE4
		{
			Byte get();
			void set(Byte value);
		}
		property Byte RD_BYTE5
		{
			Byte get();
			void set(Byte value);
		}
		property Byte RD_BYTE6
		{
			Byte get();
			void set(Byte value);
		}
		property Byte RD_BYTE7
		{
			Byte get();
			void set(Byte value);
		}
		
		AxisDataUpMsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};

	/*************************************************************/
	//   9. Axis Controller Configuration Responses (0x2A0-0x2A3) /
	/*************************************************************/

	/// the controller hardware mode
	public enum class ControlHWMode :int{ APP,BOOT };

	///  Axis Controller Configuration Responses (0x2A0-0x2A3) message
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct CtlConfigResp
	{
		[FieldOffset(0)] Byte BoardInfo;
		[FieldOffset(1)] Byte SWCode;
		[FieldOffset(2)] Byte BootTagVer;

		[FieldOffset(3)] Byte BootRevL;
		[FieldOffset(4)] Byte BootRevM;
		[FieldOffset(3)] WORD BootRev;

		[FieldOffset(5)] Byte FPGATagVer;

		[FieldOffset(6)] Byte FPGARevL;
		[FieldOffset(7)] Byte FPGARevM;
		[FieldOffset(6)] WORD FPGARev;

		[FieldOffset(8)] WORD id;
	};


	/// the service class for axis controller configuration response
	public ref class CtlConfigRespPS
	{
	private:
		ControlHWMode mode;
		ControlHardType type;
		UINT8 hardwareVer;
		Byte swCode;
		Byte bootTagVer;
		WORD bootRevision;
		Byte fpgaTagVer;
		WORD fpgaRevision;

	public:
		CtlConfigResp data;
		
		property ControlHWMode MODE
		{
			ControlHWMode get();
			void set(ControlHWMode value);
		}

		property ControlHardType BoardType
		{
			ControlHardType get();
			void set(ControlHardType value);
		}

		property UINT8 HARDWARE_VERSION
		{
			UINT8 get();
			void set(UINT8 value);
		}

		property Byte SWITCH_CODE
		{
			Byte get();
			void set(Byte value);
		}

		property Byte BOOT_TAG
		{
			Byte get();
			void set(Byte value);
		}

		property WORD BOOT_REV
		{
			WORD get();
			void set(WORD value);
		}

		property Byte FPGA_TAG
		{
			Byte get();
			void set(Byte value);
		}

		property WORD FPGA_REV
		{
			WORD get();
			void set(WORD value);
		}

		CtlConfigRespPS(ControlType);
		
		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();

	};



	/********************************************************************/
	//   10. Axis Controller Application Version Responses (0x2B0-0x2B3) /
	/********************************************************************/

	/// Axis Controller Application Version Responses (0x2B0-0x2B3) messages
	[StructLayout(LayoutKind::Explicit, Size = 72)]
	public value struct CtlAppVerRsp
	{
		[FieldOffset(0)] Byte AppTagVer;

		[FieldOffset(1)] Byte AppRevL;
		[FieldOffset(2)] Byte AppRevM;
		[FieldOffset(1)] WORD AppRev;

		[FieldOffset(3)] Byte MtParaTagVer;

		[FieldOffset(4)] Byte MtParaRevL;
		[FieldOffset(5)] Byte MtParaRevM;
		[FieldOffset(4)] WORD MtParaRev;

		[FieldOffset(6)] Byte Options;
		[FieldOffset(7)] WORD id;
	};

	/// the service class for Axis Controller Application Version Responses
	public ref class CtlAppVerRspPS
	{
	private:
		Byte appTagVer, motorParaTagVer,options;
		WORD appRev, motorParaRev;

	public:
		CtlAppVerRsp data;

		property Byte APP_TAG_VER
		{
			Byte get();
			void set(Byte value);
		}

		property WORD APP_REVISION
		{
			WORD get();
			void set(WORD value);
		}

		property Byte MT_TAG_VER
		{
			Byte get();
			void set(Byte value);
		}

		property WORD MT_PARA_REV
		{
			WORD get();
			void set(WORD value);
		}

		property Byte OPTIONS
		{
			Byte get();
			void set(Byte value);
		}

		CtlAppVerRspPS(ControlType);

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	
	};


	/********************************************************************/
	//   11. Axis Controller Boot Strap Version Responses (0x2B5-0x2B8) /
	/********************************************************************/
	/// Axis Controller Boot Strap Version Responses (0x2B5-0x2B8) messages
	[StructLayout(LayoutKind::Explicit, Size = 40)]
	public value struct CtlBootStrapVerRsp
	{
		[FieldOffset(0)] Byte BootStrapTag;

		[FieldOffset(1)] Byte BootStrapRevL;
		[FieldOffset(2)] Byte BootStrapRevM;
		[FieldOffset(1)] WORD BootStrapRev;

		[FieldOffset(3)] WORD id;
	};


	// the service class for Axis Controller Boot Strap Version Responses
	public ref class CtlBootStrapVerRspPS
	{
	private:
		Byte bootStrapTagVer;
		WORD bootStrapRev;

	public:
		CtlBootStrapVerRsp data;

		property Byte BOOTSTRAP_TAG_VER
		{
			Byte get();
			void set(Byte value);
		}

		property WORD BOOTSTRAP_REV
		{
			WORD get();
			void set(WORD value);
		}

		CtlBootStrapVerRspPS(ControlType);

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};


	/***********************************************/
	//   14. WSD Tilt Center Vector Message (0x2D2) /
	/***********************************************/
	// 14. WSD Tilt Center Vector Message(0x2D2) message
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct WSDTiltCenterVectMsg
	{
		[FieldOffset(0)] Byte RadTiltCenterByte0;
		[FieldOffset(1)] Byte RadTiltCenterByte1;
		[FieldOffset(2)] Byte RadTiltCenterByte2;
		[FieldOffset(3)] Byte RadTiltCenterByte3;
		[FieldOffset(0)] DWORD RadTiltCenter;

		[FieldOffset(4)] Byte AngTiltCenterByte0;
		[FieldOffset(5)] Byte AngTiltCenterByte1;
		[FieldOffset(6)] Byte AngTiltCenterByte2;
		[FieldOffset(7)] Byte AngTiltCenterByte3;
		[FieldOffset(4)] DWORD AngTiltCenter;

		[FieldOffset(8)] WORD id;
	};

	public ref class WSDTiltCenterVectMsgPS
	{
	private:

		double radTiltCenter, angTiltCenter;

	public:

		WSDTiltCenterVectMsg data;

		property double RAD_TILTCENTER
		{
			double get();
			void set(double value);
		}

		property double ANG_TILTCENTER
		{
			double get();
			void set(double value);
		}

		//ctor
		WSDTiltCenterVectMsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};


	/********************************************************************/
	//   15. Axis Controller Auto Movement Responses (0x2E0 thru 0x2E3)   /
	/********************************************************************/
	//  15. Axis Controller Auto Movement Responses (0x2E0 thru 0x2E3) message
	[StructLayout(LayoutKind::Explicit, Size = 32)]
	public value struct AutoMoveRespMsg
	{
		[FieldOffset(0)] Byte AutoMoveStatus;
		[FieldOffset(1)] Byte SWSetting;
		[FieldOffset(2)] WORD id;
	};

	public enum class AMoveStatus {Complete,Abort,Error,EnAccepted,AModeAccepted};

	public ref class AutoMoveRespMsgPS
	{
	private:
		AMoveStatus autoMoveStatus;
		Byte swSetting;

	public:
		AutoMoveRespMsg data;

		property AMoveStatus AUTOMOVE_STATUS
		{
			AMoveStatus get();
			void set(AMoveStatus value);
		}

		property Byte SW_SETTING
		{
			Byte get();
			void set(Byte value);
		}

		//ctor
		AutoMoveRespMsgPS(ControlType);

		AutoMoveRespMsgPS(AxisCtlProtoStuct);

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};



	/********************************************************************/
	//   16. Axis Controller Error Status Responses (0x2F0 thru 0x2F3)   /
	/********************************************************************/
	/// 16. Axis Controller Error Status Responses (0x2F0 thru 0x2F3) message
	[StructLayout(LayoutKind::Explicit, Size = 40)]
	public value struct CtlErrStatRespMsg
	{
		[FieldOffset(0)] Byte ErrorCodeL;
		[FieldOffset(1)] Byte ErrorCodeM;
		[FieldOffset(0)] WORD ErrorCode;

		[FieldOffset(2)] Byte UpdateReason;
		[FieldOffset(3)] WORD id;
	};

	// the Axis Controller Error Status Response update reason
	public enum class CtlErrStatRespUpdateReason :int{ SCBRequest, PeriodicUpdate };

	// the service class for Axis Controller Error Status Response (0x2F0 thru 0x2F3)
	public ref class CtlErrStatRespMsgPS
	{
	private:
		WORD errorCode;
		CtlErrStatRespUpdateReason updateReason;

	public:

		CtlErrStatRespMsg data;

		property WORD ERROR_CODE
		{
			WORD get();
			void set(WORD value);
		}

		property CtlErrStatRespUpdateReason UPDATEREASON
		{
			CtlErrStatRespUpdateReason get();
			void set(CtlErrStatRespUpdateReason value);
		}

		CtlErrStatRespMsgPS(ControlType);

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};


	/*************************  SCB to OTC Controllers Messages  *************************/
	/*****************************************************/
	//   1. SCB OTC XYZ Set Auto Move Destination (0x400) /
	/*****************************************************/
	//  1. SCB OTC XYZ Set Auto Move Destination (0x400) 
	[StructLayout(LayoutKind::Explicit, Size = 64)]
	public value struct SCBXYZSetAutoDesMsg
	{
		[FieldOffset(0)] Byte XPosL;
		[FieldOffset(1)] Byte XPosM;
		[FieldOffset(0)] WORD XPos;

		[FieldOffset(2)] Byte YPosL;
		[FieldOffset(3)] Byte YPosM;
		[FieldOffset(2)] WORD YPos;

		[FieldOffset(4)] Byte ZPosL;
		[FieldOffset(5)] Byte ZPosM;
		[FieldOffset(4)] WORD ZPos;

		[FieldOffset(6)] WORD id;
	};

	public ref class SCBXYZSetAutoDesMsgPS
	{
	private:
		double xPos, yPos, zPos;

	public:

		SCBXYZSetAutoDesMsg data;

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

		//ctor 
		SCBXYZSetAutoDesMsgPS(AxisCtlProtoStuct);
	};

	/*****************************************************/
	//   2.  SCB OTC AB Set Auto Move Destination (0x401) /
	/*****************************************************/
	//  2.  SCB OTC AB Set Auto Move Destination (0x401)
	[StructLayout(LayoutKind::Explicit, Size = 48)]
	public value struct SCBABSetAutoDesMsg
	{
		[FieldOffset(0)] Byte APosL;
		[FieldOffset(1)] Byte APosM;
		[FieldOffset(0)] WORD APos;

		[FieldOffset(2)] Byte BPosL;
		[FieldOffset(3)] Byte BPosM;
		[FieldOffset(2)] WORD BPos;

		[FieldOffset(4)] WORD id;
	};

	public ref class SCBABSetAutoDesMsgPS
	{
	private:
		double aPos, bPos;

	public:
		SCBABSetAutoDesMsg data;

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

		//ctor
		SCBABSetAutoDesMsgPS(AxisCtlProtoStuct);
	};


	/****************************************/
	//   3. SCB OTC XYZ Set Room Cal (0x500) /
	/****************************************/
	//  3. SCB OTC XYZ Set Room Cal (0x500) message
	[StructLayout(LayoutKind::Explicit, Size = 64)]
	public value struct OTCXYZSetRMCalMsg
	{
		[FieldOffset(0)] Byte XPosL;
		[FieldOffset(1)] Byte XPosM;
		[FieldOffset(0)] WORD XPos;

		[FieldOffset(2)] Byte YPosL;
		[FieldOffset(3)] Byte YPosM;
		[FieldOffset(2)] WORD YPos;

		[FieldOffset(4)] Byte ZPosL;
		[FieldOffset(5)] Byte ZPosM;
		[FieldOffset(4)] WORD ZPos;

		[FieldOffset(6)] WORD id;
	};

	// the service class for SCB OTC XYZ Set Room Cal (0x500) message
	public ref class OTCXYZSetRMCalMsgPS
	{
	private:

		double xPos, yPos, zPos;

	public:
		OTCXYZSetRMCalMsg data;

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

		OTCXYZSetRMCalMsgPS(AxisCtlProtoStuct);

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};


	/****************************************/
	//   4. SCB OTC Set Axis Cal (0x501)     /
	/****************************************/

	//  4.SCB OTC Set Axis Cal (0x501) message
	[StructLayout(LayoutKind::Explicit, Size = 32)]
	public value struct OTCXYZSetAxisCalMsg
	{
		[FieldOffset(0)] Byte PinAxisCal;
		[FieldOffset(1)] Byte ABCal;
		[FieldOffset(2)] WORD id;
	};

	// the service class for SCB OTC Set Axis Cal (0x501) message
	public ref class OTCXYZSetAxisCalMsgPS
	{
	private:
		bool xMinSet, yMinSet, zMinSet, xMaxSet, yMaxSet, zMaxSet, pinSet, aMinSet, bMinSet, aMaxSet, bMaxSet;

	public:

		OTCXYZSetAxisCalMsg data;

		property bool XMIN_SET
		{
			bool get();
			void set(bool value);
		}

		property bool XMAX_SET
		{
			bool get();
			void set(bool value);
		}

		property bool YMIN_SET
		{
			bool get();
			void set(bool value);
		}

		property bool YMAX_SET
		{
			bool get();
			void set(bool value);
		}

		property bool ZMIN_SET
		{
			bool get();
			void set(bool value);
		}

		property bool ZMAX_SET
		{
			bool get();
			void set(bool value);
		}

		property bool AMIN_SET
		{
			bool get();
			void set(bool value);
		}

		property bool AMAX_SET
		{
			bool get();
			void set(bool value);
		}

		property bool BMIN_SET
		{
			bool get();
			void set(bool value);
		}

		property bool BMAX_SET
		{
			bool get();
			void set(bool value);
		}

		property bool PIN_SET
		{
			bool get();
			void set(bool value);
		}

		OTCXYZSetAxisCalMsgPS(AxisCtlProtoStuct);

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};

	/*************************  SCB to TBL Controllers Messages  *************************/
	/****************************************************/
	//   1. SCB TBL Set Auto Move Destination (0x403)    /
	/****************************************************/
	//  1. SCB TBL Set Auto Move Destination (0x403)
	[StructLayout(LayoutKind::Explicit, Size = 64)]
	public value struct SCBTBLSetAutoDesMsg
	{
		[FieldOffset(0)] Byte XPosL;
		[FieldOffset(1)] Byte XPosM;
		[FieldOffset(0)] WORD XPos;

		[FieldOffset(2)] Byte YPosL;
		[FieldOffset(3)] Byte YPosM;
		[FieldOffset(2)] WORD YPos;

		[FieldOffset(4)] Byte ZPosL;
		[FieldOffset(5)] Byte ZPosM;
		[FieldOffset(4)] WORD ZPos;

		[FieldOffset(6)] WORD id;
	};

	public ref class SCBTBLSetAutoDesMsgPS
	{
	private:
		double xPos, yPos, zPos;

	public:
		
		SCBTBLSetAutoDesMsg data;

		property double X_POS
		{
			double get();
		}

		property double Y_POS
		{
			double get();
		}

		property double Z_POS
		{
			double get();
		}

		SCBTBLSetAutoDesMsgPS(AxisCtlProtoStuct);
	};
	/****************************************/
	//   2. SCB TBL Set Room Cal (0x700)     /
	/****************************************/
	// 2.SCB TBL Set Room Cal (0x700)  message
	[StructLayout(LayoutKind::Explicit, Size = 72)]
	public value struct TBLSetRMCalMsg
	{
		[FieldOffset(0)] Byte XPosL;
		[FieldOffset(1)] Byte XPosM;
		[FieldOffset(0)] WORD XPos;

		[FieldOffset(2)] Byte YPosL;
		[FieldOffset(3)] Byte YPosM;
		[FieldOffset(2)] WORD YPos;

		[FieldOffset(4)] Byte ZPosL;
		[FieldOffset(5)] Byte ZPosM;
		[FieldOffset(4)] WORD ZPos;

		[FieldOffset(6)] Byte Orientation;
		[FieldOffset(7)] WORD id;
	};

	public enum class TBLRMOrientation :int { LeftWall, BackWall, RightWall, FrontWall };

	public ref class TBLSetRMCalMsgPS
	{
	private:
		double xPos, yPos, zPos;
		TBLRMOrientation orientation;

	public:
		TBLSetRMCalMsg data;
		
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

		property TBLRMOrientation ORIENTATION
		{
			TBLRMOrientation get();
			void set(TBLRMOrientation value);
		}

		TBLSetRMCalMsgPS(AxisCtlProtoStuct);
	};


	/****************************************/
	//   3. SCB TBL Set Axis Cal (0x701)     /
	/****************************************/
	//  3. SCB TBL Set Axis Cal (0x701) message
	[StructLayout(LayoutKind::Explicit, Size = 24)]
	public value struct TBLSetAxisCalMsg
	{
		[FieldOffset(0)] Byte AxisCal;
		[FieldOffset(1)] WORD id;
	};

	public ref class TBLSetAxisCalMsgPS
	{
	private:
		bool ZMinSet, LongMinSet, ZMaxSet, LongMaxSet, PinCal;

	public:
		TBLSetAxisCalMsg data;

		property bool ZMIN_SET
		{
			bool get();
			void set(bool value);
		}

		property bool LONGMIN_SET
		{
			bool get();
			void set(bool value);
		}

		property bool ZMAX_SET
		{
			bool get();
			void set(bool value);
		}

		property bool LONGMAX_SET
		{
			bool get();
			void set(bool value);
		}

		property bool PIN_CAL
		{
			bool get(); 
			void set(bool value);
		}

		TBLSetAxisCalMsgPS(AxisCtlProtoStuct);
	};

	/*************************  SCB to WSD Controllers Messages  *************************/
	/**************************************************/
	//   1.  SCB WSD Set Auto Move Destination (0x402) /
	/**************************************************/
	// 1.  SCB WSD Set Auto Move Destination (0x402)
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct SCBWSDSetAutoDesMsg
	{
		[FieldOffset(0)] Byte ZPosL;
		[FieldOffset(1)] Byte ZPosM;
		[FieldOffset(0)] WORD ZPos;

		[FieldOffset(2)] Byte TiltPosL;
		[FieldOffset(3)] Byte TiltPosM;
		[FieldOffset(2)] WORD TiltPos;

		[FieldOffset(4)] Byte XPosL;
		[FieldOffset(5)] Byte XPosM;
		[FieldOffset(4)] WORD XPos;

		[FieldOffset(6)] Byte YPosL;
		[FieldOffset(7)] Byte YPosM;
		[FieldOffset(6)] WORD YPos;

		[FieldOffset(8)] WORD id;
	};

	public ref class SCBWSDSetAutoDesMsgPS
	{
	private:

		double zPos, tiltPos, xPos, yPos;

	public:

		SCBWSDSetAutoDesMsg data;

		property double Z_POS
		{
			double get();
		}

		property double TILT_POS
		{
			double get();
		}

		property double X_POS
		{
			double get();
		}

		property double Y_POS
		{
			double get();
		}

		SCBWSDSetAutoDesMsgPS(AxisCtlProtoStuct);
	};

	/****************************************/
	//   2. SCB WSD Set Room Cal (0x600)     /
	/****************************************/

	// 2. SCB WSD Set Room Cal (0x600) message
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct WSDSetRMCalMsg
	{
		[FieldOffset(0)] Byte XPosL;
		[FieldOffset(1)] Byte XPosM;
		[FieldOffset(0)] WORD XPos;

		[FieldOffset(2)] Byte YPosL;
		[FieldOffset(3)] Byte YPosM;
		[FieldOffset(2)] WORD YPos;

		[FieldOffset(4)] Byte ZPosL;
		[FieldOffset(5)] Byte ZPosM;
		[FieldOffset(4)] WORD ZPos;

		[FieldOffset(6)] Byte Orientation;
		[FieldOffset(7)] Byte UpdateType;

		[FieldOffset(8)] WORD id;
	};

	public enum class WSDRMOrientation:int {LeftWall,BackWall,RightWall,FrontWall};

	public enum class WSDRMCalUpdateType :int{ Vertical, Horizontal,VertToHoriz,HorizToVert};

	// the service class for SCB WSD Set Room Cal (0x600)
	public ref class WSDSetRMCalMsgPS
	{
	private:

		double xPos, yPos, zPos;
		WSDRMOrientation orientation;
		WSDRMCalUpdateType updateType;

	public:

		WSDSetRMCalMsg data;

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

		property WSDRMOrientation ORIENTATION
		{
			WSDRMOrientation get();
			void set(WSDRMOrientation value);
		}

		property WSDRMCalUpdateType UPDATETYPE
		{
			WSDRMCalUpdateType get();
			void set(WSDRMCalUpdateType value);
		}

		WSDSetRMCalMsgPS(AxisCtlProtoStuct);

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};


	/****************************************/
	//   3. SCB WSD Set Axis Cal (0x601)     /
	/****************************************/
	//  3. SCB WSD Set Axis Cal (0x601) message
	[StructLayout(LayoutKind::Explicit, Size = 24)]
	public value struct WSDSetAxisCalMsg
	{
		[FieldOffset(0)] Byte AxisCal;
		[FieldOffset(1)] WORD id;
	};

	// the service class of SCB WSD Set Axis Cal message
	public ref class WSDSetAxisCalMsgPS
	{
	private:
		bool zMinSet, zMaxSet, xRailMinSet, xRailMaxSet, pinSet;

	public:
		WSDSetAxisCalMsg data;

		property bool ZMIN_SET
		{
			bool get();
			void set(bool value);
		}

		property bool ZMAX_SET
		{
			bool get();
			void set(bool value);
		}

		property bool XRAILMIN_SET
		{
			bool get();
			void set(bool value);
		}

		property bool XRAILMAX_SET
		{
			bool get();
			void set(bool value);
		}

		property bool PIN_SET
		{
			bool get();
			void set(bool value);
		}

		WSDSetAxisCalMsgPS(AxisCtlProtoStuct);

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};

	/*************************  OTC XYZ Messages  *************************/
	/***********************************************/
	//  1. XYZ Set Sync Move Parameters (0x123)       /
	/***********************************************/
	/// 1. XYZ Set Sync Move Parameters (0x123)  
	[StructLayout(LayoutKind::Explicit, Size = 40)]
	public value struct XYZSyncMovePara
	{
		[FieldOffset(0)] Byte SyncMoveDelayL;
		[FieldOffset(1)] Byte SyncMoveDelayM;
		[FieldOffset(0)] WORD SyncMoveDelay;
		[FieldOffset(2)] Byte TBLMoveDirection;
		[FieldOffset(3)] WORD id;
	};

	public enum class TBLMoveDict :int{ LTR = 0, RTL };

	public ref class XYZSyncMoveParaPS
	{
	private:

		UINT16 syncMoveDelay;
		TBLMoveDict tblMoveDirection;

	public:
		
		XYZSyncMovePara data;

		property UINT16 SYNC_DELAY
		{
			UINT16 get();
			void set(UINT16);
		}

		property TBLMoveDict TBLMOVE_DICT
		{
			TBLMoveDict get();
			void set(TBLMoveDict);
		}

		//ctor
		XYZSyncMoveParaPS();

		//ctor
		XYZSyncMoveParaPS(AxisCtlProtoStuct);

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();

	};

	/********************************************************************/
	//   2. OTC XYZ Status Message (0x200) /
	/********************************************************************/
	/// OTC XYZ Status Message (0x200) message
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct OTCXYZStaMsg
	{
		[FieldOffset(0)] Byte MResSWStatus;
		[FieldOffset(1)] Byte XAxisStatus;
		[FieldOffset(2)] Byte YAxisStatus;
		[FieldOffset(3)] Byte ZAxisStatus;
		[FieldOffset(4)] Byte OpMode;
		[FieldOffset(5)] Byte XYSoftDtStatus;
		[FieldOffset(6)] Byte ZSoftDtAuxSWStatus;
		[FieldOffset(7)] Byte UpdateReason;
		[FieldOffset(8)] WORD id;
	};

	// the helper class represent controller switch status
	public value struct MSwStatus
	{
		bool xPressed, yPressed, zPressed, xyzPressed, aPressed, bPressed, dtSwPressed;
	};

	// the helper class represent controller movement system hardware status
	public value struct ControlMSStatus
	{
		bool mtEn, clutchEn, breakEn;

	};

	// the helper class represent controller Estop switch activated
	public value struct EstopSW
	{
		bool conEstopEn, conAutoPosEn, otcEstopEn, conAutoPosIdOn;
	};

	// the helper class represent controller status updating reason
	public value struct OTCXYZStaMsgUpdateReason
	{
		bool relSWChange, xStaChange, yStaChange, zStaChange, OpModeChange, auxSWChange, softDTChange, scbRequest;
	};

	/// the service class for OTC XYZ Status Message (0x200)
	public ref class OTCXYZStaMsgPS
	{
	private :
		MSwStatus resSWStatus;
		ControlMSStatus xMSStatus, yMSStatus, zMSStatus;
		ControlOpMode opMode;
		UINT xDtStatus, yDtStatus, zDtStatus;
		EstopSW estopSWStatus;
		OTCXYZStaMsgUpdateReason updataReason;

	public:

		OTCXYZStaMsg data;

		// Manual Release Switch Status
		property MSwStatus RES_SW_STA
		{
			MSwStatus get();
			void set(MSwStatus value);
		}

		// the X Axis movement hardware status
		property ControlMSStatus X_STATUS
		{
			ControlMSStatus get();
			void set(ControlMSStatus value);
		}

		// the Y Axis movement hardware status
		property ControlMSStatus Y_STATUS
		{
			ControlMSStatus get();
			void set(ControlMSStatus value);
		}

		// the Z Axis movement hardware status
		property ControlMSStatus Z_STATUS
		{
			ControlMSStatus get();
			void set(ControlMSStatus value);
		}

		// the otc xyz operator mode
		property ControlOpMode OP_MODE
		{
			ControlOpMode get();
			void set(ControlOpMode value);
		}

		// the X Axis Soft Detent status
		property UINT X_DT_STA
		{
			UINT get();
			void set(UINT value);
		}

		// the Y Axis Soft Detent status
		property UINT Y_DT_STA
		{
			UINT get();
			void set(UINT value);
		}

		// the Z Axis Soft Detent status
		property UINT Z_DT_STA
		{
			UINT get();
			void set(UINT value);
		}

		// the ESTOP switch status
		property EstopSW ESTOP_SW
		{
			EstopSW get();
			void set(EstopSW value);
		}

		// status update reason
		property OTCXYZStaMsgUpdateReason UPDATE_REASON
		{
			OTCXYZStaMsgUpdateReason get();
			void set(OTCXYZStaMsgUpdateReason value);
		}

		OTCXYZStaMsgPS();
		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};	


	/********************************************************************/
	//   3. IR Remote Status Message (0x260) /
	/********************************************************************/
    
	/// IR Remote Status Message (0x260) message
	[StructLayout(LayoutKind::Explicit, Size = 72)]
	public value struct OTCIRStaMsg
	{
		[FieldOffset(0)] Byte SimpleMSStatus;
		[FieldOffset(1)] Byte TiltSWStatus;
		[FieldOffset(2)] Byte ComplexMSStatus;
		[FieldOffset(3)] Byte ColPSSWStatus;
		[FieldOffset(4)] Byte ColBPSWStatus;
		[FieldOffset(5)] Byte ColLPSWStatus;
		[FieldOffset(6)] Byte UpdataReason;
		[FieldOffset(7)] WORD id;
	};

	public value struct IRSimpleMSStatus
	{
		bool hSUpSW, lSUpSW, hSDownSW, lSDownSW, hSLeftSW, lSLeftSW, hSRightSW, lSRightSW;
	};

	public value struct IRTiltSWStatus
	{
		bool tiltUpSW, tiltDownSW;
	};

	public value struct IRComplexMSStatus
	{
		bool autoCenterSW, autoTrackSW, autoPosSW, autoCenter2SW;
	};

	public value struct IRColPSSWStatus
	{
		bool pre1SW, pre2SW, pre3SW;
	};

	public value struct IRColBPSWStatus
	{
		bool horiOpenSW, horiCloseSW, vertOpenSW, vertCloseSW;
	};

	public value struct IRStaUpdateReason
	{
		bool simMSStaChange, tiltStaChange, comMSStaChange, colPSStaChange, colBSStaChange, lampStaChange, scbRequest;
	};

	/// the service class for  IR Remote Status Message (0x260)
	public ref class OTCIRStaMsgPS
	{
	private:
		IRSimpleMSStatus simpleMSStatus;
		IRTiltSWStatus tiltSWStatus;
		IRComplexMSStatus complexMSStatus;
		IRColPSSWStatus colPSSWStatus;
		IRColBPSWStatus colBPSWStatus;
		bool LampSWStatus;
		IRStaUpdateReason updateReason;

	public:
		OTCIRStaMsg data;

		property IRSimpleMSStatus SIM_MS_STA
		{
			IRSimpleMSStatus get();
			void set(IRSimpleMSStatus value);
		}

		property IRTiltSWStatus TILT_SW_STA
		{
			IRTiltSWStatus get();
			void set(IRTiltSWStatus value);
		}

		property IRComplexMSStatus COM_MS_STA
		{
			IRComplexMSStatus get();
			void set(IRComplexMSStatus value);
		}

		property IRColPSSWStatus COL_PS_STA
		{
			IRColPSSWStatus get();
			void set(IRColPSSWStatus value);
		}

		property IRColBPSWStatus COL_BP_STA
		{
			IRColBPSWStatus get();
			void set(IRColBPSWStatus value);
		}

		property bool LAMP_STA
		{
			bool get();
			void set(bool value);
		}

		property IRStaUpdateReason UPDATE_REASON
		{
			IRStaUpdateReason get();
			void set(IRStaUpdateReason value);
		}

		OTCIRStaMsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};

	/********************************************************************/
	//   4. OTC XYZ Heartbeat Message (0x300) /
	/********************************************************************/
	/// OTC XYZ Heartbeat Message (0x300) message
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct OTCXYZHBMsg
	{
		[FieldOffset(0)] Byte XPosL;
		[FieldOffset(1)] Byte XposM;
		[FieldOffset(0)] WORD Xpos;

		[FieldOffset(2)] Byte YPosL;
		[FieldOffset(3)] Byte YPosM;
		[FieldOffset(2)] WORD Ypos;

		[FieldOffset(4)] Byte ZPosL;
		[FieldOffset(5)] Byte ZPosM;
		[FieldOffset(4)] WORD Zpos;

		[FieldOffset(6)] Byte AxisCalStatus;
		[FieldOffset(7)] Byte UpdataReason;
		[FieldOffset(8)] WORD id;
	};

	public value struct OTCXYZHBUpdateReason
	{
		bool hbTimeOut, scbRequest, axisMove, CalStaChange;
	};

	/// the service class for OTC XYZ Heartbeat Message (0x300)	
	public ref class OTCXYZHBMsgPS
	{
	private:
		bool XMinSet, XMaxSet, YMinSet, YMaxSet, ZMinSet, ZMaxSet, RmCalDone, PinCalDone;

		double xPositon, yPosition, zPosition;

		OTCXYZHBUpdateReason updateReason;

	public:

		OTCXYZHBMsg data;

		property bool XMIN_SET
		{
			bool get();
			void set(bool value);
		}

		property bool YMIN_SET
		{
			bool get();
			void set(bool value);
		}

		property bool ZMIN_SET
		{
			bool get();
			void set(bool value);
		}

		property bool XMAX_SET
		{
			bool get();
			void set(bool value);
		}

		property bool YMAX_SET
		{
			bool get();
			void set(bool value);
		}

		property bool ZMAX_SET
		{
			bool get();
			void set(bool value);
		}

		property bool RMCAL_DONE
		{
			bool get();
			void set(bool value);
		}

		property bool PINCAL_DONE
		{
			bool get();
			void set(bool value);
		}

		property double X_POSITION
		{
			double get();
			void set(double value);
		}

		property double Y_POSITION
		{
			double get();
			void set(double value);
		}

		property double Z_POSITION
		{
			double get();
			void set(double value);
		}

		property OTCXYZHBUpdateReason UPDATE_REASON
		{
			OTCXYZHBUpdateReason get();
			void set(OTCXYZHBUpdateReason value);
		}

		OTCXYZHBMsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};	


	/**************************************************/
	//   5. OTC XYZ LLI Status Message (0x305)        //
	/**************************************************/
	// OTC XYZ LLI Status Message (0x305) message
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct OTCLLIStatusMsg
	{
		[FieldOffset(0)] Byte XYTravEndL;
		[FieldOffset(1)] Byte XYTravEndM;
		[FieldOffset(0)] WORD XYTravEnd;

		[FieldOffset(2)] Byte XYTravStartL;
		[FieldOffset(3)] Byte XYTravStartM;
		[FieldOffset(2)] WORD XYTravStart;

		[FieldOffset(4)] Byte ZTravEndL;
		[FieldOffset(5)] Byte ZTravEndM;
		[FieldOffset(4)] WORD ZTravEnd;

		[FieldOffset(6)] Byte ZTravStartL;
		[FieldOffset(7)] Byte ZTravStartM;
		[FieldOffset(6)] WORD ZTravStart;

		[FieldOffset(8)] WORD id;
	};

	public ref class OTCLLIStatusMsgPS
	{
	private:

		double xyTravEnd, xyTravStart, zTravEnd, zTravStart;

	public:

		OTCLLIStatusMsg data;

		property double XYTRAV_END
		{
			double get();
			void set(double);
		}

		property double XYTRAV_START
		{
			double get();
			void set(double);
		}

		property double ZTRAV_START
		{
			double get();
			void set(double);
		}

		property double ZTRAV_END
		{
			double get();
			void set(double);
		}

		OTCLLIStatusMsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};


	/*************************  OTC AB Messages  *************************/

	/********************************************************************/
	//   1. OTC AB Status Message (0x210) /
	/********************************************************************/
	/// OTC AB Status Message (0x210) message
	[StructLayout(LayoutKind::Explicit, Size = 48)]
	public value struct ABStatusMsg
	{
		[FieldOffset(0)] Byte AStatus;
		[FieldOffset(1)] Byte BStatus;
		[FieldOffset(2)] Byte OpMode;
		[FieldOffset(3)] Byte UpdataReason;
		[FieldOffset(4)] WORD id;
	};

	public value struct ABStaUpdateReason
	{
		bool aStatusChange, bStatusChange, opModeChange, scbRequest;
	};

	public value struct AStatus
	{
		bool mtRun, brakeEn, hmSensor, inDetent;
	};

	public value struct BStatus
	{
		bool mtRun, dtSoEn, dtEn;
	};

	/// the service class for OTC AB Status Message(0x210)
	public ref class ABStatusMsgPS
	{
	private:
		AStatus aStatus;
		BStatus bStatus;
		ControlOpMode opMode;
		ABStaUpdateReason updateReason;

	public:
		ABStatusMsg data;

		property AStatus A_STA
		{
			AStatus get();
			void set(AStatus value);
		}

		property BStatus B_STA
		{
			BStatus get();
			void set(BStatus value);
		}

		property ControlOpMode OP_MODE
		{
			ControlOpMode get();
			void set(ControlOpMode value);
		}

		property ABStaUpdateReason UPDATE_REASON
		{
			ABStaUpdateReason get();
			void set(ABStaUpdateReason value);
		}

		ABStatusMsgPS();
		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();

	};	

	/********************************************************************/
	//   2. OTC AB HeartBeat Message (0x310) /
	/********************************************************************/
	//  2. OTC AB HeartBeat Message (0x310) message
	[StructLayout(LayoutKind::Explicit, Size = 64)]
	public value struct OTCABHBMsg
	{
		[FieldOffset(0)] Byte APosL;
		[FieldOffset(1)] Byte APosM;
		[FieldOffset(0)] WORD APos;

		[FieldOffset(2)] Byte BPosL;
		[FieldOffset(3)] Byte BPosM;
		[FieldOffset(2)] WORD BPos;

		[FieldOffset(4)] Byte AxisCalStatus;
		[FieldOffset(7)] Byte UpdataReason;
		[FieldOffset(8)] WORD id;
	};

	public value struct OTCABHBUpdateReason
	{
		bool hbTimeOut, scbRequest, axisMove, CalStaChange;
	};

	public value struct OTCABCalStatus
	{
		bool AMinSet, BMinSet, AMaxSet, BMaxSet, PinCalSet;
	};


	//the service class for  OTC AB HeartBeat Message
	public ref class OTCABHBMsgPS
	{
	private:
		OTCABCalStatus calStatus;
		double aPosition, bPosition;
		OTCABHBUpdateReason updateReason;

	public:
		OTCABHBMsg data;

		property OTCABCalStatus CAL_STA
		{
			OTCABCalStatus get();
			void set(OTCABCalStatus value);
		}

		property double A_POSITION
		{
			double get();
			void set(double value);
		}

		property double B_POSITION
		{
			double get();
			void set(double value);
		}

		property OTCABHBUpdateReason UPDATE_REASON
		{
			OTCABHBUpdateReason get();
			void set(OTCABHBUpdateReason value);
		}

		OTCABHBMsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};

	/*************************  TBL Messages  *************************/
	/********************************************************************/
	//   1. TBL Sync Move Start (0x120) /
	/********************************************************************/
	//   1. TBL Sync Move Start (0x120) 
	[StructLayout(LayoutKind::Explicit, Size = 32)]
	public value struct TBLSyncMoveStart
	{
		[FieldOffset(0)] Byte AtSpeedDelayM;
		[FieldOffset(1)] Byte AtSpeedDelayL;
		[FieldOffset(0)] WORD AtSpeedDelay;
		[FieldOffset(2)] WORD id;
	};

	public ref class TBLSyncMoveStartPS
	{
	private:

		UInt16 atSpeedDelay;

	public:

		TBLSyncMoveStart data;

		property UInt16 AT_SPEED_DELAY
		{
			UInt16 get();
			void set(UInt16);
		}

		//ctor 
		TBLSyncMoveStartPS();

		//ctor
		TBLSyncMoveStartPS(AxisCtlProtoStuct);

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();

	};

	/********************************************************************/
	//   2. TBL Status Message (0x230) /
	/********************************************************************/
	//   2. TBL Status Message (0x230) message
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct TBLStatusMsg
	{
		[FieldOffset(0)] Byte SwStatus;
		[FieldOffset(1)] Byte MtControl;
		[FieldOffset(2)] Byte MtSensor;
		[FieldOffset(3)] Byte BuckyZdt;
		[FieldOffset(4)] Byte OpMode;
		[FieldOffset(5)] Byte UpdateReason;
		[FieldOffset(6)] Byte KeyPadStatus;
		[FieldOffset(7)] Byte IndDetctorId;
		[FieldOffset(8)] WORD id;
	};

	public value struct TBLSwStatus
	{
		bool autoCenterSW, autoTrackSW, estopSW, zUpPedal, zDownPedal, floatPedal;
	};

	public value struct TBLMtControls
	{
		bool longMotorRun, zMotorRun, floatSolenAssert;
	};

	public value struct TBLMtSensors
	{
		bool longBuckySensor, gridPresent, dtIntable, dtExtend, buckyInPortrait, buckyInLand, floatCollison;
	};

	public value struct TBLStatusUpdateReason
	{
		bool swStatusChange, xAxisStatusChange, mtSensorChange, softDtChange, opModeChange, scbRequest, detectorIdCHANGE;
	};

	public value struct TBLKeyPadStatus
	{
		bool relTransSW, relLongSW, relBothSW, tblDownSW, tblUpSW;
	};

	public value struct TBLIndicatorStatus
	{
		bool gridPresent, autoTrack;
	};


	public ref class TBLStatusMsgPS
	{
	private:
		TBLSwStatus swStatus;
		TBLMtControls mtControls;
		TBLMtSensors mtSensors;
		Byte buckyDetent;
		Byte zDetent;
		ControlOpMode operateMode;
		TBLStatusUpdateReason updateReason;
		TBLKeyPadStatus keypadStatus;
		TBLIndicatorStatus indicatorStatus;
		Byte detectorID;

	public:
		TBLStatusMsg data;

		property TBLSwStatus SW_STATUS
		{
			TBLSwStatus get();
			void set(TBLSwStatus value);
		}

		property TBLMtControls MTCONTROLS
		{
			TBLMtControls get();
			void set(TBLMtControls value);
		}

		property TBLMtSensors MTSENSORS
		{
			TBLMtSensors get();
			void set(TBLMtSensors value);
		}

		property Byte BUCKY_DT
		{
			BYTE get();
			void set(Byte value);
		}

		property Byte Z_DT
		{
			Byte get();
			void set(Byte value);
		}

		property ControlOpMode OP_MODE
		{
			ControlOpMode get();
			void set(ControlOpMode value);
		}

		property TBLStatusUpdateReason UPDATE_REASON
		{
			TBLStatusUpdateReason get();
			void set(TBLStatusUpdateReason value);
		}

		property TBLKeyPadStatus KEYPAD_STATUS
		{
			TBLKeyPadStatus get();
			void set(TBLKeyPadStatus value);
		}

		property TBLIndicatorStatus INDICATOR_STATUS
		{
			TBLIndicatorStatus get();
			void set(TBLIndicatorStatus value);
		}

		property Byte DETECTOR_ID
		{
			Byte get();
			void set(Byte value);
		}

		// ctor
		TBLStatusMsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};


	/*************************************/
	//  3.TBL Heartbeat Message (0x330)    /
	/*************************************/
	//  3. TBL Heartbeat Message (0x330)  message
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct TBLHBMsg
	{
		[FieldOffset(0)] Byte XPosL;
		[FieldOffset(1)] Byte XPosM;
		[FieldOffset(0)] WORD XPos;

		[FieldOffset(2)] Byte YPosL;
		[FieldOffset(3)] Byte YPosM;
		[FieldOffset(2)] WORD YPos;

		[FieldOffset(4)] Byte ZPosL;
		[FieldOffset(5)] Byte ZPosM;
		[FieldOffset(4)] WORD ZPos;

		[FieldOffset(6)] Byte AxisCalStatus;
		[FieldOffset(7)] Byte UpdatereasonOr;
		[FieldOffset(8)] WORD id;
	};

	public value struct TBLHBUpdateReason
	{
		bool hbTimeOut, scbRequest, movementOccur, calStatusChange;
	};

	public value struct TBLCalStatus
	{
		bool zMinSet, longMinSet, zMaxSet, longMaxSet, tblLocEst, rmCalDone, pinCalDone;
	};

	public enum struct TBLRoomLayout :int{ NotSet, Layout1, Layout2, Layout3, Layout4 };


	public ref class TBLHBMsgPS
	{
	private:
		double xPosition, yPosition, zPosition;
		TBLCalStatus calStatus;
		TBLHBUpdateReason updateReason;
		TBLRoomLayout layout;

	public:
		TBLHBMsg data;

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

		property TBLCalStatus CALSTATUS
		{
			TBLCalStatus get();
			void set(TBLCalStatus value);
		}

		property TBLHBUpdateReason UPDATEREASON
		{
			TBLHBUpdateReason get();
			void set(TBLHBUpdateReason value);
		}

		property TBLRoomLayout LAYOUT
		{
			TBLRoomLayout get();
			void set(TBLRoomLayout value);
		}

		// ctor
		TBLHBMsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};


	/**************************************************/
	//   4. TBL LLI Status Message (0x335)            //
	/**************************************************/
	//  4. TBL LLI Status Message (0x335) message
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct TBLLLIStatusMsg
	{
		[FieldOffset(0)] Byte BuckyLLIPosByte4;
		[FieldOffset(1)] Byte BuckyLLIPosByte3;
		[FieldOffset(2)] Byte BuckyLLIPosByte2;
		[FieldOffset(3)] Byte BuckyLLIPosByte1;

		[FieldOffset(0)] DWORD BuckyLLIPos;

		[FieldOffset(4)] Byte BuckyTravEndL;
		[FieldOffset(5)] Byte BuckyTravEndM;
		[FieldOffset(4)] WORD BuckyTravEnd;

		[FieldOffset(6)] Byte BuckyTravStartL;
		[FieldOffset(7)] Byte BuckyTravStartM;
		[FieldOffset(6)] WORD BuckyTravStart;

		[FieldOffset(8)] WORD id;
	};

	public ref class TBLLLIStatusMsgPS
	{
	private:
		double buckyTravEnd, buckyTravStart;
		Int32 buckyLLIPos;

	public:
		TBLLLIStatusMsg data;

		property Int32 BUCKY_LLIPOS
		{
			Int32 get();
			void set(Int32);
		}

		property double BUCKYTRAV_END
		{
			double get();
			void set(double);
		}

		property double BUCKYTRAV_START
		{
			double get();
			void set(double);
		}

		TBLLLIStatusMsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};

	/*************************  WSD Messages  *************************/
	/********************************************************************/
	//   1. WSD Status Message (0x220) /
	/********************************************************************/
	//  1. WSD Status Message (0x220)  message
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct WSDStatusMsg
	{
		[FieldOffset(0)] Byte SWIndStatus;
		[FieldOffset(1)] Byte MtControl;
		[FieldOffset(2)] Byte DetGrSensor;
		[FieldOffset(3)] Byte Sensors;
		[FieldOffset(4)] Byte OpMode;
		[FieldOffset(5)] Byte ZXDetent;
		[FieldOffset(6)] Byte TlDtDetectId;
		[FieldOffset(7)] Byte UpdateReason;
		[FieldOffset(8)] WORD id;
	};


	// the switch and indicator status
	public value struct WSDSWStatus
	{
		bool zPressed, xPressed, estopPressed;
	};

	// Motion Controls
	public value struct WSDMotionControls
	{
		bool zMotorRun, zBrakeEn, xMotorRun, tiltMotorRun;
	};

	public enum class GridType :UINT8 { NotPresent = 0,Type1, Type2, Type3, Type4, Type5, Type6, Type7};

	// The detector and grid sensors
	public value struct DetGridSensor
	{
		bool buckyInPort, buckyInLand, detInPort, detInLand;
	};

	// The Sensors
	public value struct Sensors
	{
		bool isTrayIn, buckyRetract, buckyExtend;
	};

	// The update reasons
	public value struct WSDStatusMsgUpdateReason
	{
		bool swIndStateChange, motionCtlChange, buckyGridChange, sensorChange, opModeChange, detIdChange, detentChange, scbRequest;
	};

	// The service class for WSD Status Message
	public ref class WSDStatusMsgPS
	{
	private:
		WSDSWStatus swStatus;
		WSDMotionControls motionControls;
		DetGridSensor detGridSensors;
		Sensors sensors;
		ControlOpMode opMode;
		UINT8 xSoftDetent, zSoftDetent,tiltDetent,detectorID;
		WSDStatusMsgUpdateReason updateReason;
		GridType gridType;
	public:
		WSDStatusMsg data;

		property WSDSWStatus SW_STATUS
		{
			WSDSWStatus get();
			void set(WSDSWStatus value);
		}

		property WSDMotionControls MT_CONTROL
		{
			WSDMotionControls get();
			void set(WSDMotionControls value);
		}

		property DetGridSensor DT_SENSOR
		{
			DetGridSensor get();
			void set(DetGridSensor value);
		}

		property GridType GRID_TYPE
		{
			GridType get();
			void set(GridType value);
		}

		property Sensors SENSORS
		{
			Sensors get();
			void set(Sensors value);
		}

		property ControlOpMode OP_MODE
		{
			ControlOpMode get();
			void set(ControlOpMode value);
		}

		property UINT8 X_DT
		{
			UINT8 get();
			void set(UINT8 value);
		}

		property UINT8 Z_DT
		{
			UINT8 get();
			void set(UINT8 value);
		}

		property UINT8 TILT_DT
		{
			UINT8 get();
			void set(UINT8 value);
		}

		property UINT8 DETECTOR_ID
		{
			UINT8 get();
			void set(UINT8 value);
		}

		property WSDStatusMsgUpdateReason UPDATE_REASON
		{
			WSDStatusMsgUpdateReason get();
			void set(WSDStatusMsgUpdateReason value);
		}

		//ctor
		WSDStatusMsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};


	/********************************************************************/
	//   2. WSD Keypad Status Message (0x250) /
	/********************************************************************/
	//  2. WSD Keypad Status Message (0x250)  message
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct WSDKeyStatusMsg
	{
		[FieldOffset(0)] Byte SimMtSW;
		[FieldOffset(1)] Byte TiltSW;
		[FieldOffset(2)] Byte ComMtSW;
		[FieldOffset(3)] Byte KeyIndt;
		[FieldOffset(4)] Byte ColPsetSW;
		[FieldOffset(5)] Byte ColBPSW;
		[FieldOffset(6)] Byte ColLampSW;
		[FieldOffset(7)] Byte UpdateReason;
		[FieldOffset(8)] WORD id;
	};

	// The Simple Motion Switch Status
	public value struct SimpleMotionSW
	{
		bool hsUpSW, lsUpSW, hsDownSW, lsDownSW, hsLeftSW, lsLeftSW, hsRightSW, lsRightSW;
	};

	// The Tilt Switch Status
	public value struct TiltSW
	{
		bool tiltUpSW, tiltDownSW;
	};

	// The Complex Motion Switch Status
	public value struct ComplexMotionSW
	{
		bool autoCenterSW, autoTrackSW, autoPosSW, autoCenter2SW;
	};

	// The keypad Indicator Status
	public value struct KeyIndicator
	{
		bool autoTrackInd, autoPosInd, autoCenterInd;
	};

	// The Collimator Preset Switch Status
	public value struct ColPresetSW
	{
		bool Pre1SW, Pre2SW, Pre3SW;
	};

	// The Collimator Blade Postion Switch Status
	public value struct ColBladePosSW
	{
		bool openHBSW, closeHBSW, openVBSW, closeVBSW, openHBTapped, closeHBTapped, openVBTapped, closeVBTapped;
	};

	// The WSD Key Status Update Reason
	public value struct WSDKEYStatusMsgUpdateReason
	{
		bool simMtSWChange, tiltSWChange, comMtSWChange, colPSChange, colBSChange, colLampSWChange, scbRequest;
	};

	// The service class for WSD Keypad Status Message
	public ref class WSDKeyStatusMsgPS
	{
	private:
		SimpleMotionSW simMotionSW;
		TiltSW tiltSW;
		ComplexMotionSW comMotionSW;
		KeyIndicator keyIndicators;
		ColPresetSW colPresetSW;
		ColBladePosSW colBPSW;
		bool colLampSW;
		WSDKEYStatusMsgUpdateReason updateReason;

	public:
		WSDKeyStatusMsg data;

		property SimpleMotionSW SIM_MT_SW
		{
			SimpleMotionSW get();
			void set(SimpleMotionSW value);
		}

		property TiltSW TILT_SW
		{
			TiltSW get();
			void set(TiltSW value);
		}

		property ComplexMotionSW COM_MT_SW
		{
			ComplexMotionSW get();
			void set(ComplexMotionSW value);
		}

		property KeyIndicator KEY_INDICATOR
		{
			KeyIndicator get();
			void set(KeyIndicator value);
		}

		property ColPresetSW COL_PRESET_SW
		{
			ColPresetSW get();
			void set(ColPresetSW value);
		}

		property ColBladePosSW COL_BLADE_SW
		{
			ColBladePosSW get();
			void set(ColBladePosSW value);
		}

		property bool LAMP_SW
		{
			bool get();
			void set(bool value);
		}

		property WSDKEYStatusMsgUpdateReason UPDATE_REASON
		{
			WSDKEYStatusMsgUpdateReason get();
			void set(WSDKEYStatusMsgUpdateReason value);
		}


		//ctor
		WSDKeyStatusMsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};


	/*************************************/
	//   3. WSD Heartbeat Message (0x320) /
	/*************************************/
	//  3. WSD Heartbeat Message (0x320)  message
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct WSDHBMsg
	{
		[FieldOffset(0)] Byte XPosL;
		[FieldOffset(1)] Byte XPosM;
		[FieldOffset(0)] WORD XPos;

		[FieldOffset(2)] Byte YPosL;
		[FieldOffset(3)] Byte YPosM;
		[FieldOffset(2)] WORD YPos;

		[FieldOffset(4)] Byte ZPosL;
		[FieldOffset(5)] Byte ZPosM;
		[FieldOffset(4)] WORD ZPos;

		[FieldOffset(6)] Byte AxisCalStatus;
		[FieldOffset(7)] Byte UpdatereasonOr;
		[FieldOffset(8)] WORD id;
	};

	// WSD Cal Status
	public value struct WSDCalStatus
	{
		bool ZMinSet, ZMaxSet, XMinSet, XMaxSet, HorRMCalDone, VertRMCalDone,PinCalDone;
	};

	// WSD Heartbeat update reason
	public value struct WSDHBUpdateReason
	{
		bool hbTimeOut, scbRequest, movementOccur, calStatusChange;
	};

	// WSD Orientation
	public enum struct WSDRoomLayout :int{NotSet,Layout1,Layout2,Layout3,Layout4};

	// The service class for WSD heartbeat message
	public ref class WSDHBMsgPS
	{
	private:

		double xPos, yPos, zPos;
		WSDCalStatus calStatus;
		WSDHBUpdateReason updateReason;
		WSDRoomLayout layout;

	public:

		WSDHBMsg data;

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

		property WSDCalStatus CAL_STATUS
		{
			WSDCalStatus get();
			void set(WSDCalStatus value);
		}

		property WSDHBUpdateReason UPDATE_REASON
		{
			WSDHBUpdateReason get();
			void set(WSDHBUpdateReason value);
		}

		property WSDRoomLayout LAYOUT
		{
			WSDRoomLayout get();
			void set(WSDRoomLayout value);
		}

		WSDHBMsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};	


	/*******************************************************/
	//   4. WSD Tilt and Angulation Postion Message (0x321) /
	/*******************************************************/

	// 4. WSD Tilt and Angulation Postion Message (0x321) message
	[StructLayout(LayoutKind::Explicit, Size = 64)]
	public value struct WSDTiltPosMsg
	{
		[FieldOffset(0)] Byte TiltPosL;
		[FieldOffset(1)] Byte TiltPosM;
		[FieldOffset(0)] WORD TiltPos;

		[FieldOffset(2)] Byte AngPosL;
		[FieldOffset(3)] Byte AngPosM;
		[FieldOffset(2)] WORD AngPos;


		[FieldOffset(4)] Byte AxisCalStatus;
		[FieldOffset(5)] Byte UpdateReason;
		[FieldOffset(6)] WORD id;
	};

	// Tilt Calculation Status
	public value struct WSDTiltCalStatus
	{
		bool TiltMinSet, TiltVertSet, TiltHorSet;
	};

	// Update Reason
	public value struct WSDTiltPosUpdateReason
	{
		bool hbTimeOut, scbRequest, movementOccur, calStaChange;
	};

	// The service class for WSD Tilt and Angulation Postion Message
	public ref class WSDTiltPosMsgPS
	{
	private:
		double tiltPos, angPos;
		WSDTiltCalStatus calStatus;
		WSDTiltPosUpdateReason updateReason;

	public:
		WSDTiltPosMsg data;

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

		property WSDTiltCalStatus CAL_STATUS
		{
			WSDTiltCalStatus get();
			void set(WSDTiltCalStatus value);
		}

		property WSDTiltPosUpdateReason UPDATE_REASON
		{
			WSDTiltPosUpdateReason get();
			void set(WSDTiltPosUpdateReason value);
		}

		WSDTiltPosMsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};


	/**************************************************/
	//   5. WSD LLI Status Message (0x325)            //
	/**************************************************/
	//  5. WSD LLI Status Message (0x325) message
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct WSDLLIStatusMsg
	{
		[FieldOffset(0)] Byte ZLLIPosByte4;
		[FieldOffset(1)] Byte ZLLIPosByte3;
		[FieldOffset(2)] Byte ZLLIPosByte2;
		[FieldOffset(3)] Byte ZLLIPosByte1;

		[FieldOffset(0)] DWORD ZLLIPos;

		[FieldOffset(4)] Byte ZTravEndL;
		[FieldOffset(5)] Byte ZTravEndM;
		[FieldOffset(4)] WORD ZTravEnd;

		[FieldOffset(6)] Byte ZTravStartL;
		[FieldOffset(7)] Byte ZTravStartM;
		[FieldOffset(6)] WORD ZTravStart;

		[FieldOffset(8)] WORD id;
	};

	public ref class WSDLLIStatusMsgPS
	{
	private:
		double zTravEnd, zTravStart;
		Int32 zLLIPos;

	public:
		WSDLLIStatusMsg data;

		property Int32 Z_LLIPOS
		{
			Int32 get();
			void set(Int32);
		}

		property double ZTRAV_END
		{
			double get();
			void set(double);
		}

		property double ZTRAV_START
		{
			double get();
			void set(double);
		}

		WSDLLIStatusMsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};


	/*************************  Collimator to SCB  Messages  *************************/

	/***********************************************/
	//     1. ASR1 Board Model Message (0x700)      /
	/***********************************************/

	///  1. ASR1 Board Model Message (0x700)
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct ASR1ModelMsg
	{
		[FieldOffset(0)] Byte Byte0;
		[FieldOffset(1)] Byte Byte1;
		[FieldOffset(2)] Byte Byte2;
		[FieldOffset(3)] Byte Byte3;
		[FieldOffset(4)] Byte Byte4;
		[FieldOffset(5)] Byte Byte5;
		[FieldOffset(6)] Byte Byte6;
		[FieldOffset(7)] Byte Byte7;
		[FieldOffset(8)] WORD id;
	};

	public ref class ASR1ModelMsgPS
	{
	private:

		String ^ model;

	public:

		ASR1ModelMsg data;

		property String^ MODEL
		{
			String^ get();
			void set(String ^);
		}

		// ctor
		ASR1ModelMsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};

	/***********************************************/
	//     2. ASR2 Board Model Message (0x7A6)      /
	/***********************************************/

	///  2. ASR2 Board Model Message (0x7A6)
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct ASR2ModelMsg
	{
		[FieldOffset(0)] Byte Byte0;
		[FieldOffset(1)] Byte Byte1;
		[FieldOffset(2)] Byte Byte2;
		[FieldOffset(3)] Byte Byte3;
		[FieldOffset(4)] Byte Byte4;
		[FieldOffset(5)] Byte Byte5;
		[FieldOffset(6)] Byte Byte6;
		[FieldOffset(7)] Byte Byte7;
		[FieldOffset(8)] WORD id;
	};

	public ref class ASR2ModelMsgPS
	{
	private:

		String ^ model;

	public:

		ASR2ModelMsg data;

		property String^ MODEL
		{
			String^ get();
			void set(String ^);
		}

		// ctor
		ASR2ModelMsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};

	/***********************************************/
	//     3. ASR1 Board Serial Message (0x703)      /
	/***********************************************/

	///  3. ASR1 Board Serial Message (0x703)
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct ASR1SNMsg
	{
		[FieldOffset(0)] Byte Byte0;
		[FieldOffset(1)] Byte Byte1;
		[FieldOffset(2)] Byte Byte2;
		[FieldOffset(3)] Byte Byte3;
		[FieldOffset(4)] Byte Byte4;
		[FieldOffset(5)] Byte Byte5;
		[FieldOffset(6)] Byte Byte6;
		[FieldOffset(7)] Byte Byte7;
		[FieldOffset(8)] WORD id;
	};

	public ref class ASR1SNMsgPS
	{
	private:

		String ^ sn;

	public:

		ASR1SNMsg data;

		property String^ SN
		{
			String^ get();
			void set(String ^);
		}

		// ctor
		ASR1SNMsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};


	/***********************************************/
	//     4. ASR2 Board Status Message 1 (0x7F0)      /
	/***********************************************/

	///  4. ASR2 Board Status Message 1 (0x7F0)
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct ASR2Status1Msg
	{
		[FieldOffset(0)] Byte Mode_Lamp;
		[FieldOffset(1)] Byte Byte1;
		[FieldOffset(2)] Byte Byte2;
		[FieldOffset(3)] Byte VSID;

		[FieldOffset(4)] Byte VCrossL;
		[FieldOffset(5)] Byte VCrossM;
		[FieldOffset(4)] WORD VCross;

		[FieldOffset(6)] Byte VLongL;
		[FieldOffset(7)] Byte VLongM;
		[FieldOffset(6)] WORD VLong;

		[FieldOffset(8)] WORD id;
	};

	public enum class CollimatorMode :int{Auto,Manual};

	public ref class ASR2Status1MsgPS
	{
	private:
		CollimatorMode mode;
		int vsid,crossSize, longSize;
		bool lampOn;

	public:
		ASR2Status1Msg data;

		property CollimatorMode Mode
		{
			CollimatorMode get();
			void set(CollimatorMode);
		}

		property int V_SID
		{
			int get();
			void set(int);
		}

		property int CROSS_SIZE
		{
			int get();
			void set(int);
		}

		property int LONG_SIZE
		{
			int get();
			void set(int);
		}

		property bool LAMP_ON
		{
			bool get();
			void set(bool);
		}

		//ctor
		ASR2Status1MsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};
	

	/***********************************************/
	//     5. ASR2 Board Status Message 2 (0x7F1)      /
	/***********************************************/

	///  5. ASR2 Board Status Message 2 (0x7F1)
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct ASR2Status2Msg
	{
		[FieldOffset(0)] Byte Byte0;
		[FieldOffset(1)] Byte Byte1;
		[FieldOffset(2)] Byte Byte2;
		[FieldOffset(3)] Byte Filter;

		[FieldOffset(4)] Byte LSidL;
		[FieldOffset(5)] Byte LSidM;
		[FieldOffset(4)] WORD LSid;

		[FieldOffset(6)] Byte Byte6;
		[FieldOffset(7)] Byte Byte7;
		[FieldOffset(8)] WORD id;
	};

	public ref class ASR2Status2MsgPS
	{
	private:

		Byte filterType;
		int lsid;

	public:
		ASR2Status2Msg data;

		property Byte FILTER_TYPE
		{
			Byte get();
			void set(Byte);
		}

		property int L_SID
		{
			int get();
			void set(int);
		}

		//ctor
		ASR2Status2MsgPS();

		//get the protocal data for sending
		AxisCtlProtoStuct GetFrame();
	};


	

	/*************************  SCB to Collimator  Messages  *************************/
	/***********************************************/
	//     1. ASR2 Board Control Message (0x7A0)      /
	/***********************************************/

	///  1. ASR2 Board Control Message (0x7A0)
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct ASR2CtlMsg
	{
		[FieldOffset(0)] Byte Byte0;
		[FieldOffset(1)] Byte Byte1;
		[FieldOffset(2)] Byte Byte2;
		[FieldOffset(3)] Byte VSid;
		[FieldOffset(4)] Byte CrossSizeL;
		[FieldOffset(5)] Byte CrossSizeM;
		[FieldOffset(4)] WORD CrossSize;
		[FieldOffset(6)] Byte LongSizeL;
		[FieldOffset(7)] Byte LongSizeM;
		[FieldOffset(6)] WORD LongSize;
		[FieldOffset(8)] WORD id;
	};

	public ref class ASR2CtlMsgPS
	{
	private:

		CollimatorMode mode;
		bool crossOpen, crossClose, longOpen, longClose,filterChange, lampOn;
		int vSid, crossSize, longSize;

	public:

		ASR2CtlMsg data;

		property CollimatorMode MODE
		{
			CollimatorMode get();
		}

		property bool LAMP_ON
		{
			bool get();
		}

		property int V_SID
		{
			int get();
		}

		property int CROSS_SIZE
		{
			int get();
		}

		property int LONG_SIZE
		{
			int get();
		}

		//ctor
		ASR2CtlMsgPS(AxisCtlProtoStuct);
	};


	/***********************************************/
	//     2. ASR2 Board Control Message 2 (0x702)    /
	/***********************************************/

	///   2. ASR2 Board Control Message 2 (0x702) 
	[StructLayout(LayoutKind::Explicit, Size = 80)]
	public value struct ASR2Ctl2Msg
	{
		[FieldOffset(0)] Byte CMD;
		[FieldOffset(1)] Byte Byte1;
		[FieldOffset(2)] Byte Byte2;
		[FieldOffset(1)] WORD LampTime;
		[FieldOffset(3)] Byte RampTime;
		[FieldOffset(4)] Byte Byte4;
		[FieldOffset(5)] Byte Byte5;
		[FieldOffset(6)] Byte Byte6;
		[FieldOffset(7)] Byte Byte7;
		[FieldOffset(8)] WORD id;
	};

	public ref class ASR2Ctl2MsgPS
	{
	private:

		bool isLampCycle, laserOn, laserOff, fanOn, fanOff, lampAbort;

		double lampTime, rampTime;

	public:

		ASR2Ctl2Msg data;

		property bool IS_LAMPCYCLE
		{
			bool get();
		}

		property bool LASER_ON
		{
			bool get();
		}

		property bool LASER_OFF
		{
			bool get();
		}

		property bool FAN_ON
		{
			bool get();
		}

		property bool FAN_OFF
		{
			bool get();
		}

		property bool LAMP_ABORT
		{
			bool get();
		}

		property double LAMP_TIME
		{
			double get();
		}

		property double RAMP_TIME
		{
			double get();
		}

		ASR2Ctl2MsgPS(AxisCtlProtoStuct);
	};

}
