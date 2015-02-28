// Define all the scb protocols service classes implementation
// SCBprotocol service class will parse the protocol and get the information or translate information into protocol data.

#include "stdafx.h"
#include "SCBProtocalWrapper.h"

namespace SCBProtocalWrapper
{

	/*************************  All Controllers to SCB Messages  *************************/

	/******************************************/
	//     1. SCB Controller Reset(0x100)      /
	/******************************************/
	SCBCtlResetPS::SCBCtlResetPS()
	{
		data.id = 0x100;
		data.Byte0 = 0x00;
	}

	SCBCtlResetPS::SCBCtlResetPS(Byte segment)
	{
		data.id = 0x100;
		data.Byte0 = segment;
		if ((data.Byte0 & 0x1) != 0x0)
		{
			OTCXYZReset = true;
		}
		if ((data.Byte0 & 0x2) != 0x0)
		{
			OTCABReset = true;
		}
		if ((data.Byte0 & 0x4) != 0x0)
		{
			WSDReset = true;
		}
		if ((data.Byte0 & 0x8) != 0x0)
		{
			TBLReset = true;
		}
	}

	bool SCBCtlResetPS::RESET_OTCXYZ::get()
	{
		return OTCXYZReset;
	}

	void SCBCtlResetPS::RESET_OTCXYZ::set(bool value)
	{
		if (value == true)
		{
			data.Byte0 |= 0x01;
			OTCXYZReset = true;
		}
		else
		{
			data.Byte0 &= 0xFE;
			OTCXYZReset = false;
		}
	}

	bool SCBCtlResetPS::RESET_OTCAB::get()
	{
		return OTCABReset;
	}

	void SCBCtlResetPS::RESET_OTCAB::set(bool value)
	{
		if (value == true)
		{
			data.Byte0 |= 0x02;
			OTCABReset = true;
		}
		else
		{
			data.Byte0 &= 0xFD;
			OTCABReset = false;
		}
	}

	bool SCBCtlResetPS::RESET_WSD::get()
	{
		return WSDReset;
	}

	void SCBCtlResetPS::RESET_WSD::set(bool value)
	{
		if (value == true)
		{
			data.Byte0 |= 0x04;
			WSDReset = true;
		}
		else
		{
			data.Byte0 &= 0xFB;
			WSDReset = false;
		}
	}

	bool SCBCtlResetPS::RESET_OTCTBL::get()
	{
		return TBLReset;
	}

	void SCBCtlResetPS::RESET_OTCTBL::set(bool value)
	{
		if (value == true)
		{
			data.Byte0 |= 0x08;
			TBLReset = true;
		}
		else
		{
			data.Byte0 &= 0xF7;
			TBLReset = false;
		}
	}


	/******************************************/
	//     2. SCB Auto Move Abort(0x101)      /
	/******************************************/

	SCBAutoAbtPS::SCBAutoAbtPS()
	{
		data.id = 0x101;
	}


	/******************************************/
	//     3. SCB Auto Move Enable(0x102)      /
	/******************************************/

	SCBAutoEnPS::SCBAutoEnPS(AxisCtlProtoStuct frame)
	{
		data.id = frame.id;
		data.Byte0 = frame.frameData[0];

		if (data.Byte0 == 1)
		{
			ContinueNeed = true;
		}
		else
		{
			ContinueNeed = false;
		}
	}

	bool SCBAutoEnPS::CONTINUE_NEED::get()
	{
		return ContinueNeed;
	}

	void SCBAutoEnPS::CONTINUE_NEED::set(bool value)
	{
		if (value == true)
		{
			data.Byte0 = 1;
		}
		else
		{
			data.Byte0 = 0;
		}
		ContinueNeed = value;
	}


	/******************************************/
	//      4. SCB Verify Auto Move(0x103)     /
	/******************************************/

	SCBAutoVfyPS::SCBAutoVfyPS()
	{
		data.id = 0x103;
	}

	/******************************************/
	//     5. SCB Auto Move Continue(0x104)    /
	/******************************************/

	SCBAutoContPS::SCBAutoContPS()
	{
		data.id = 0x104;
	}

	/**********************************************************/
	//      6. SCB Set Tomographic Parameters (0X124)          /
	/**********************************************************/

	SCBTomoParaPS::SCBTomoParaPS(AxisCtlProtoStuct frame)
	{
		data.id = frame.id;
		if ((frame.frameData[0] & 0x01) == 1)
		{
			data.RequestType = true;
			tomoTrue = true;
		}
		else
		{
			data.RequestType = false;
			tomoTrue = false;
		}
		data.SweepAngle = frame.frameData[1];
		sweepAngle = data.SweepAngle;
		data.LayerHeightM = frame.frameData[2];
		data.LayerHeightL = frame.frameData[3];
		layerHeight = data.LayerHeight;
		data.SweepTimeM = frame.frameData[4];
		data.SweepTimeL = frame.frameData[5];
		sweepTime = data.SweepTime;
		data.DelayTimeM = frame.frameData[6];
		data.DelayTimeL = frame.frameData[7];
		delayTime = data.DelayTime;
	}

	bool SCBTomoParaPS::TOMO_TRUE::get()
	{
		return tomoTrue;
	}

	UInt16 SCBTomoParaPS::SWEEP_ANGLE::get()
	{
		return sweepAngle;
	}

	UInt16 SCBTomoParaPS::SWEEP_TIME::get()
	{
		return sweepTime;
	}

	UInt16 SCBTomoParaPS::DELAY_TIME::get()
	{
		return delayTime;
	}

	UInt16 SCBTomoParaPS::LAYER_HEIGHT::get()
	{
		return layerHeight;
	}


	/************************************************/
	//    7. SCB Controller Status Request(0x410)    /
	/************************************************/

	SCBCtrlStatusReqPS::SCBCtrlStatusReqPS()
	{
		data.CommandTarget = 0x00;
		data.RequestType = 0x00;
		data.id = 0x410;

		OTCXYZrt = OTCABrt = WSDrt = TBLrt = PosUpdateReq = ContStaReq = KeyStaReq = ErrStaReq = LLIPosReq = false;
	}

	SCBCtrlStatusReqPS::SCBCtrlStatusReqPS(AxisCtlProtoStuct package)
	{
		data.id = 0x410;
		data.CommandTarget = package.frameData[0];
		data.RequestType = package.frameData[1];
		if ((data.CommandTarget & 0x01) != 0)
		{
			OTCXYZrt = true;
		}
		else
		{
			OTCXYZrt = false;
		}
		if ((data.CommandTarget & 0x02) != 0)
		{
			OTCABrt = true;
		}
		else
		{
			OTCABrt = false;
		}
		if ((data.CommandTarget & 0x04) != 0)
		{
			WSDrt = true;
		}
		else
		{
			WSDrt = false;
		}
		if ((data.CommandTarget & 0x08) != 0)
		{
			TBLrt = true;
		}
		else
		{
			TBLrt = false;
		}
		if ((data.RequestType & 0x01) != 0)
		{
			PosUpdateReq = true;
		}
		else
		{
			PosUpdateReq = false;
		}
		if ((data.RequestType & 0x02) != 0)
		{
			ContStaReq = true;
		}
		else
		{
			ContStaReq = false;
		}
		if ((data.RequestType & 0x04) != 0)
		{
			KeyStaReq = true;
		}
		else
		{
			KeyStaReq = false;
		}
		if ((data.RequestType & 0x08) != 0)
		{
			ErrStaReq = true;
		}
		else
		{
			ErrStaReq = false;
		}
		if ((data.RequestType & 0x10) != 0)
		{
			LLIPosReq = true;
		}
		else
		{
			LLIPosReq = false;
		}
	}

	bool SCBCtrlStatusReqPS::OTCXYZ_RT::get()
	{
		return OTCXYZrt;
	}

	void SCBCtrlStatusReqPS::OTCXYZ_RT::set(bool value)
	{
		if (value == true)
		{
			data.CommandTarget |= 0x01;
			OTCXYZrt = true;
		}
		else
		{
			data.CommandTarget &= 0xFE;
			OTCXYZrt = false;
		}
	}

	bool SCBCtrlStatusReqPS::OTCAB_RT::get()
	{
		return OTCABrt;
	}

	void SCBCtrlStatusReqPS::OTCAB_RT::set(bool value)
	{
		if (value == true)
		{
			data.CommandTarget |= 0x02;
			OTCABrt = true;
		}
		else
		{
			data.CommandTarget &= 0xFD;
			OTCABrt = false;
		}
	}

	bool SCBCtrlStatusReqPS::WSD_RT::get()
	{
		return WSDrt;
	}

	void SCBCtrlStatusReqPS::WSD_RT::set(bool value)
	{
		if (value == true)
		{
			data.CommandTarget |= 0x04;
			WSDrt = true;
		}
		else
		{
			data.CommandTarget &= 0xFB;
			WSDrt = false;
		}
	}

	bool SCBCtrlStatusReqPS::TBL_RT::get()
	{
		return TBLrt;
	}

	void SCBCtrlStatusReqPS::TBL_RT::set(bool value)
	{
		if (value == true)
		{
			data.CommandTarget |= 0x08;
			TBLrt = true;
		}
		else
		{
			data.CommandTarget &= 0xF7;
			TBLrt = false;
		}
	}

	bool SCBCtrlStatusReqPS::POSUPDATE_REQ::get()
	{
		return PosUpdateReq;
	}

	void SCBCtrlStatusReqPS::POSUPDATE_REQ::set(bool value)
	{
		if (value == true)
		{
			data.RequestType |= 0x01;
			PosUpdateReq = true;
		}
		else
		{
			data.RequestType &= 0xFE;
			PosUpdateReq = false;
		}
	}

	bool SCBCtrlStatusReqPS::CONTSTA_REQ::get()
	{
		return ContStaReq;
	}

	void SCBCtrlStatusReqPS::CONTSTA_REQ::set(bool value)
	{
		if (value == true)
		{
			data.RequestType |= 0x02;
			ContStaReq = true;
		}
		else
		{
			data.RequestType &= 0xFD;
			ContStaReq = false;
		}
	}

	bool SCBCtrlStatusReqPS::KEYSTA_REQ::get()
	{
		return KeyStaReq;
	}

	void SCBCtrlStatusReqPS::KEYSTA_REQ::set(bool value)
	{
		if (value == true)
		{
			data.RequestType |= 0x04;
			KeyStaReq = true;
		}
		else
		{
			data.RequestType &= 0xFB;
			KeyStaReq = false;
		}
	}

	bool SCBCtrlStatusReqPS::ERRSTA_REQ::get()
	{
		return ErrStaReq;
	}

	void SCBCtrlStatusReqPS::ERRSTA_REQ::set(bool value)
	{
		if (value == true)
		{
			data.RequestType |= 0x08;
			ErrStaReq = true;
		}
		else
		{
			data.RequestType &= 0xF7;
			ErrStaReq = false;
		}
	}

	bool SCBCtrlStatusReqPS::LLIPOS_REQ::get()
	{
		return LLIPosReq;
	}

	void SCBCtrlStatusReqPS::LLIPOS_REQ::set(bool value)
	{
		if (value == true)
		{
			data.RequestType |= 0x10;
			LLIPosReq = true;
		}
		else
		{
			data.RequestType &= 0xEF;
			LLIPosReq = false;
		}
	}

	AxisCtlProtoStuct SCBCtrlStatusReqPS::GetFrame()
	{
			AxisCtlProtoStuct frame;
			frame.id = data.id;
			frame.size = 2;
			frame.frameData = gcnew array<Byte>(frame.size);
			frame.frameData[0] = data.CommandTarget;
			frame.frameData[1] = data.RequestType;
			return frame;
	}


	/*************************************************************/
	//    8. SCB Controller Soft Detent Status Request (0x411)    /
	/*************************************************************/
	SCBSoftDtReqPS::SCBSoftDtReqPS()
	{
		data.id = 0x411;
		data.Frame = 0;
		OTCXYZdt = WSDdt = TBLdt = false;
		XPlane = YPlane = ZPlane = false;
	}

	SCBSoftDtReqPS::SCBSoftDtReqPS(WORD segment)
	{
		data.id = 0x411;
		data.Frame = segment;
		if ((data.CommandTarget & 0x01) != 0)
		{
			OTCXYZdt = true;
		}
		else
		{
			OTCXYZdt = false;
		}
		if ((data.CommandTarget & 0x04) != 0)
		{
			WSDdt = true;
		}
		else
		{
			WSDdt = false;
		}
		if ((data.CommandTarget & 0x08) != 0)
		{
			TBLdt = true;
		}
		else
		{
			TBLdt = false;
		}

		if ((data.MotionPlane & 0x01) != 0)
		{
			ZPlane = true;
		}
		else
		{
			ZPlane = false;
		}
		if ((data.MotionPlane & 0x02) != 0)
		{
			XPlane = true;
		}
		else
		{
			XPlane = false;
		}
		if ((data.MotionPlane & 0x04) != 0)
		{
			YPlane = true;
		}
		else
		{
			YPlane = false;
		}
	}

	bool SCBSoftDtReqPS::OTCXYZ_DT::get()
	{
		return OTCXYZdt;
	}

	void SCBSoftDtReqPS::OTCXYZ_DT::set(bool value)
	{
		if (value == true)
		{
			data.CommandTarget |= 0x01;
			OTCXYZdt = true;
		}
		else
		{
			data.CommandTarget &= 0xFE;
			OTCXYZdt = false;
		}
	}

	bool SCBSoftDtReqPS::WSD_DT::get()
	{
		return WSDdt;
	}

	void SCBSoftDtReqPS::WSD_DT::set(bool value)
	{
		if (value == true)
		{
			data.CommandTarget |= 0x04;
			WSDdt = true;
		}
		else
		{
			data.CommandTarget &= 0xFB;
			WSDdt = false;
		}
	}

	bool SCBSoftDtReqPS::TBL_DT::get()
	{
		return TBLdt;
	}

	void SCBSoftDtReqPS::TBL_DT::set(bool value)
	{
		if (value == true)
		{
			data.CommandTarget |= 0x08;
			TBLdt = true;
		}
		else
		{
			data.CommandTarget &= 0xF7;
			TBLdt = false;
		}
	}

	bool SCBSoftDtReqPS::ZPLANE::get()
	{
		return ZPlane;
	}

	void SCBSoftDtReqPS::ZPLANE::set(bool value)
	{
		if (value == true)
		{
			data.MotionPlane |= 0x01;
			ZPlane = true;
		}
		else
		{
			data.MotionPlane &= 0xFE;
			ZPlane = false;
		}
	}

	bool SCBSoftDtReqPS::XPLANE::get()
	{
		return XPlane;
	}

	void SCBSoftDtReqPS::XPLANE::set(bool value)
	{
		if (value == true)
		{
			data.MotionPlane |= 0x02;
			XPlane = true;
		}
		else
		{
			data.MotionPlane &= 0xFD;
			XPlane = false;
		}
	}

	bool SCBSoftDtReqPS::YPLANE::get()
	{
		return YPlane;
	}

	void SCBSoftDtReqPS::YPLANE::set(bool value)
	{
		if (value == true)
		{
			data.MotionPlane |= 0x04;
			YPlane = true;
		}
		else
		{
			data.MotionPlane &= 0xFB;
			YPlane = false;
		}
	}


	/*************************************************/
	//    9. SCB Controller Set Soft Detent (0x420)   /
	/*************************************************/

	SCBSoftDtSetPS::SCBSoftDtSetPS()
	{
		data.id = 0x420;
		data.CommandTarget = data.MotionPlane = 0x0;
		data.DetentIndex = 1;
		OTCXYZdt = WSDdt = TBLdt = false;
		XPlane = YPlane = ZPlane = false;
	}

	SCBSoftDtSetPS::SCBSoftDtSetPS(array<Byte> ^ segment)
	{
		data.id = 0x420;
		data.CommandTarget = segment[0];
		data.MotionPlane = segment[1];
		data.DetentIndex = segment[2];

		if ((data.CommandTarget & 0x01) != 0)
		{
			OTCXYZdt = true;
		}
		else
		{
			OTCXYZdt = false;
		}
		if ((data.CommandTarget & 0x04) != 0)
		{
			WSDdt = true;
		}
		else
		{
			WSDdt = false;
		}
		if ((data.CommandTarget & 0x08) != 0)
		{
			TBLdt = true;
		}
		else
		{
			TBLdt = false;
		}

		if ((data.MotionPlane & 0x01) != 0)
		{
			ZPlane = true;
		}
		else
		{
			ZPlane = false;
		}
		if ((data.MotionPlane & 0x02) != 0)
		{
			XPlane = true;
		}
		else
		{
			XPlane = false;
		}
		if ((data.MotionPlane & 0x04) != 0)
		{
			YPlane = true;
		}
		else
		{
			YPlane = false;
		}
	}

	bool SCBSoftDtSetPS::OTCXYZ_DT::get()
	{
		return OTCXYZdt;
	}

	void SCBSoftDtSetPS::OTCXYZ_DT::set(bool value)
	{
		if (value == true)
		{
			data.CommandTarget |= 0x01;
			OTCXYZdt = true;
		}
		else
		{
			data.CommandTarget &= 0xFE;
			OTCXYZdt = false;
		}
	}

	bool SCBSoftDtSetPS::WSD_DT::get()
	{
		return WSDdt;
	}

	void SCBSoftDtSetPS::WSD_DT::set(bool value)
	{
		if (value == true)
		{
			data.CommandTarget |= 0x04;
			WSDdt = true;
		}
		else
		{
			data.CommandTarget &= 0xFB;
			WSDdt = false;
		}
	}

	bool SCBSoftDtSetPS::TBL_DT::get()
	{
		return TBLdt;
	}

	void SCBSoftDtSetPS::TBL_DT::set(bool value)
	{
		if (value == true)
		{
			data.CommandTarget |= 0x08;
			TBLdt = true;
		}
		else
		{
			data.CommandTarget &= 0xF7;
			TBLdt = false;
		}
	}

	bool SCBSoftDtSetPS::ZPLANE::get()
	{
		return ZPlane;
	}

	void SCBSoftDtSetPS::ZPLANE::set(bool value)
	{
		if (value == true)
		{
			data.MotionPlane |= 0x01;
			ZPlane = true;
		}
		else
		{
			data.MotionPlane &= 0xFE;
			ZPlane = false;
		}
	}

	bool SCBSoftDtSetPS::XPLANE::get()
	{
		return XPlane;
	}

	void SCBSoftDtSetPS::XPLANE::set(bool value)
	{
		if (value == true)
		{
			data.MotionPlane |= 0x02;
			XPlane = true;
		}
		else
		{
			data.MotionPlane &= 0xFD;
			XPlane = false;
		}
	}

	bool SCBSoftDtSetPS::YPLANE::get()
	{
		return YPlane;
	}

	void SCBSoftDtSetPS::YPLANE::set(bool value)
	{
		if (value == true)
		{
			data.MotionPlane |= 0x04;
			YPlane = true;
		}
		else
		{
			data.MotionPlane &= 0xFB;
			YPlane = false;
		}
	}

	UINT8 SCBSoftDtSetPS::SOFT_INDEX::get()
	{
		return data.DetentIndex;
	}

	void SCBSoftDtSetPS::SOFT_INDEX::set(Byte index)
	{
		data.DetentIndex = index;
	}


	/*************************************************/
	//    10. SCB Controller Clear Soft Detent (0x421)   /
	/*************************************************/
	SCBSoftDtClearPS::SCBSoftDtClearPS()
	{
		data.id = 0x421;
		data.CommandTarget = data.MotionPlane = 0x0;
		data.DetentIndex = 1;
		OTCXYZdt = WSDdt = TBLdt = false;
		XPlane = YPlane = ZPlane = false;
	}

	SCBSoftDtClearPS::SCBSoftDtClearPS(array<Byte> ^ segment)
	{
		data.id = 0x421;
		data.CommandTarget = segment[0];
		data.MotionPlane = segment[1];
		data.DetentIndex = segment[2];

		if ((data.CommandTarget & 0x01) != 0)
		{
			OTCXYZdt = true;
		}
		else
		{
			OTCXYZdt = false;
		}
		if ((data.CommandTarget & 0x04) != 0)
		{
			WSDdt = true;
		}
		else
		{
			WSDdt = false;
		}
		if ((data.CommandTarget & 0x08) != 0)
		{
			TBLdt = true;
		}
		else
		{
			TBLdt = false;
		}

		if ((data.MotionPlane & 0x01) != 0)
		{
			ZPlane = true;
		}
		else
		{
			ZPlane = false;
		}
		if ((data.MotionPlane & 0x02) != 0)
		{
			XPlane = true;
		}
		else
		{
			XPlane = false;
		}
		if ((data.MotionPlane & 0x04) != 0)
		{
			YPlane = true;
		}
		else
		{
			YPlane = false;
		}
	}

	bool SCBSoftDtClearPS::OTCXYZ_DT::get()
	{
		return OTCXYZdt;
	}

	void SCBSoftDtClearPS::OTCXYZ_DT::set(bool value)
	{
		if (value == true)
		{
			data.CommandTarget |= 0x01;
			OTCXYZdt = true;
		}
		else
		{
			data.CommandTarget &= 0xFE;
			OTCXYZdt = false;
		}
	}

	bool SCBSoftDtClearPS::WSD_DT::get()
	{
		return WSDdt;
	}

	void SCBSoftDtClearPS::WSD_DT::set(bool value)
	{
		if (value == true)
		{
			data.CommandTarget |= 0x04;
			WSDdt = true;
		}
		else
		{
			data.CommandTarget &= 0xFB;
			WSDdt = false;
		}
	}

	bool SCBSoftDtClearPS::TBL_DT::get()
	{
		return TBLdt;
	}

	void SCBSoftDtClearPS::TBL_DT::set(bool value)
	{
		if (value == true)
		{
			data.CommandTarget |= 0x08;
			TBLdt = true;
		}
		else
		{
			data.CommandTarget &= 0xF7;
			TBLdt = false;
		}
	}

	bool SCBSoftDtClearPS::ZPLANE::get()
	{
		return ZPlane;
	}

	void SCBSoftDtClearPS::ZPLANE::set(bool value)
	{
		if (value == true)
		{
			data.MotionPlane |= 0x01;
			ZPlane = true;
		}
		else
		{
			data.MotionPlane &= 0xFE;
			ZPlane = false;
		}
	}

	bool SCBSoftDtClearPS::XPLANE::get()
	{
		return XPlane;
	}

	void SCBSoftDtClearPS::XPLANE::set(bool value)
	{
		if (value == true)
		{
			data.MotionPlane |= 0x02;
			XPlane = true;
		}
		else
		{
			data.MotionPlane &= 0xFD;
			XPlane = false;
		}
	}

	bool SCBSoftDtClearPS::YPLANE::get()
	{
		return YPlane;
	}

	void SCBSoftDtClearPS::YPLANE::set(bool value)
	{
		if (value == true)
		{
			data.MotionPlane |= 0x04;
			YPlane = true;
		}
		else
		{
			data.MotionPlane &= 0xFB;
			YPlane = false;
		}
	}

	UINT8 SCBSoftDtClearPS::SOFT_INDEX::get()
	{
		return data.DetentIndex;
	}

	void SCBSoftDtClearPS::SOFT_INDEX::set(Byte index)
	{
		data.DetentIndex = index;
	}

	/*******************************/
	//   11. SCB Clear Cal (0x422 ) /
	/*******************************/
	SCBClrCalPS::SCBClrCalPS()
	{
		data.id = 0x422;
		data.CommandTarget = 0x00;
		data.CalibrationType = 0x00;

		OTCClr = WSDClr = TBLClr = false;
		PinCal = CalType1 = CalType2 = CalType3 = CalType4 = CalType5 = CalType6 = CalType7 = false;
	}

	SCBClrCalPS::SCBClrCalPS(AxisCtlProtoStuct segment)
	{
		data.id = 0x422;
		data.CommandTarget = segment.frameData[0];
		data.CalibrationType = segment.frameData[1];

		if (data.CommandTarget == 0x00)
		{
			OTCClr = true;
		}
		else
		{
			OTCClr = false;
		}

		if (data.CommandTarget == 0x02)
		{
			WSDClr = true;
		}
		else
		{
			WSDClr = false;
		}

		if (data.CommandTarget == 0x03 )
		{
			TBLClr = true;
		}
		else
		{
			TBLClr = false;
		}

		if ((data.CalibrationType & 0x01) != 0)
		{
			CalType1 = true;
		}
		else
		{
			CalType1 = false;
		}

		if ((data.CalibrationType & 0x02) != 0)
		{
			CalType2 = true;
		}
		else
		{
			CalType2 = false;
		}

		if ((data.CalibrationType & 0x04) != 0)
		{
			CalType3 = true;
		}
		else
		{
			CalType3 = false;
		}

		if ((data.CalibrationType & 0x08) != 0)
		{
			CalType4 = true;
		}
		else
		{
			CalType4 = false;
		}

		if ((data.CalibrationType & 0x10) != 0)
		{
			CalType5 = true;
		}
		else
		{
			CalType5 = false;
		}

		if ((data.CalibrationType & 0x20) != 0)
		{
			CalType6 = true;
		}
		else
		{
			CalType6 = false;
		}

		if ((data.CalibrationType & 0x40) != 0)
		{
			CalType7 = true;
		}
		else
		{
			CalType7 = false;
		}

		if ((data.CalibrationType & 0x80) != 0)
		{
			PinCal = true;
		}
		else
		{
			PinCal = false;
		}
	}

	bool SCBClrCalPS::OTC_CLEAR::get()
	{
		return OTCClr;
	}

	void SCBClrCalPS::OTC_CLEAR::set(bool value)
	{
		if (value == true)
		{
			data.CommandTarget = 0x00;
			OTCClr = true;
		}
		else
		{
			data.CommandTarget = 0x01;
			OTCClr = false;
		}
	}

	bool SCBClrCalPS::WSD_CLEAR::get()
	{
		return WSDClr;
	}

	void SCBClrCalPS::WSD_CLEAR::set(bool value)
	{
		if (value == true)
		{
			data.CommandTarget = 0x02;
			WSDClr = true;
		}
		else
		{
			data.CommandTarget = 0x01;
			WSDClr = false;
		}
	}

	bool SCBClrCalPS::TBL_Clear::get()
	{
		return TBLClr;
	}

	void SCBClrCalPS::TBL_Clear::set(bool value)
	{
		if (value == true)
		{
			data.CommandTarget = 0x03;
			TBLClr = true;
		}
		else
		{
			data.CommandTarget = 0x01;
			TBLClr = false;
		}
	}

	bool SCBClrCalPS::CAL_TYPE1::get()
	{
		return CalType1;
	}

	void SCBClrCalPS::CAL_TYPE1::set(bool value)
	{
		if (value == true)
		{
			data.CalibrationType |= 0x01;
			CalType1 = true;
		}
		else
		{
			data.CalibrationType &= 0xFE;
			CalType1 = false;
		}
	}

	bool SCBClrCalPS::CAL_TYPE2::get()
	{
		return CalType2;
	}

	void SCBClrCalPS::CAL_TYPE2::set(bool value)
	{
		if (value == true)
		{
			data.CalibrationType |= 0x02;
			CalType2 = true;
		}
		else
		{
			data.CalibrationType &= 0xFD;
			CalType2 = false;
		}
	}

	bool SCBClrCalPS::CAL_TYPE3::get()
	{
		return CalType3;
	}

	void SCBClrCalPS::CAL_TYPE3::set(bool value)
	{
		if (value == true)
		{
			data.CalibrationType |= 0x04;
			CalType3 = true;
		}
		else
		{
			data.CalibrationType &= 0xFB;
			CalType3 = false;
		}
	}

	bool SCBClrCalPS::CAL_TYPE4::get()
	{
		return CalType4;
	}

	void SCBClrCalPS::CAL_TYPE4::set(bool value)
	{
		if (value == true)
		{
			data.CalibrationType |= 0x08;
			CalType4 = true;
		}
		else
		{
			data.CalibrationType &= 0xF7;
			CalType4 = false;
		}
	}

	bool SCBClrCalPS::CAL_TYPE5::get()
	{
		return CalType5;
	}

	void SCBClrCalPS::CAL_TYPE5::set(bool value)
	{
		if (value == true)
		{
			data.CalibrationType |= 0x10;
			CalType5 = true;
		}
		else
		{
			data.CalibrationType &= 0xEF;
			CalType5 = false;
		}
	}

	bool SCBClrCalPS::CAL_TYPE6::get()
	{
		return CalType6;
	}

	void SCBClrCalPS::CAL_TYPE6::set(bool value)
	{
		if (value == true)
		{
			data.CalibrationType |= 0x20;
			CalType6 = true;
		}
		else
		{
			data.CalibrationType &= 0xDF;
			CalType6 = false;
		}
	}

	bool SCBClrCalPS::CAL_TYPE7::get()
	{
		return CalType7;
	}

	void SCBClrCalPS::CAL_TYPE7::set(bool value)
	{
		if (value == true)
		{
			data.CalibrationType |= 0x40;
			CalType7 = true;
		}
		else
		{
			data.CalibrationType &= 0xBF;
			CalType7 = false;
		}
	}

	bool SCBClrCalPS::PIN_TYPE::get()
	{
		return PinCal;
	}

	void SCBClrCalPS::PIN_TYPE::set(bool value)
	{
		if (value == true)
		{
			data.CalibrationType |= 0x80;
			PinCal = true;
		}
		else
		{
			data.CalibrationType &= 0x7F;
			PinCal = false;
		}
	}

	AxisCtlProtoStuct SCBClrCalPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 2;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.CommandTarget;
		frame.frameData[1] = data.CalibrationType;

		return frame;
	}


	/*******************************/
	//   12. SCB Auto Track Mode (0x430 ) /
	/*******************************/
	TrackMode SCBAutoTrackPS::Mode::get()
	{
		return this->mode;
	}

	void SCBAutoTrackPS::Mode::set(TrackMode newMode)
	{
		switch (newMode)
		{
		case TrackMode::ABORT_AT:
			data.TrackTarget = 0x00;
			mode = TrackMode::ABORT_AT;
			break;
		case TrackMode::OTC_ATWSD:
			data.TrackTarget = 0x01;
			mode = TrackMode::OTC_ATWSD;
			break;
		case TrackMode::OTC_ATTBL:
			data.TrackTarget = 0x02;
			mode = TrackMode::OTC_ATTBL;
			break;
		case TrackMode::WSD_ATOTC:
			data.TrackTarget = 0x03;
			mode = TrackMode::WSD_ATOTC;
			break;
		case TrackMode::TBL_ATOTC:
			data.TrackTarget = 0x04;
			mode = TrackMode::TBL_ATOTC;
			break;
		default:
			//if mode not correct,what to do?
			break;
		}
	}

	SCBAutoTrackPS::SCBAutoTrackPS(TrackMode newMode)
	{
		this->data.id = 0x430;
		this->mode = newMode;
	}


	/*******************************************/
	//   16. SCB Configuration Request (0x461 ) /
	/*******************************************/
	bool SCBConfigReqPS::OTCXYZ_REQ::get()
	{
		return this->otcxyzReq;
	}

	void SCBConfigReqPS::OTCXYZ_REQ::set(bool value)
	{
		if (value == true)
		{
			this->data.ResponseReq |= 0x01;
			this->otcxyzReq = true;
		}
		else
		{
			this->data.ResponseReq &= 0xFE;
			this->otcxyzReq = false;
		}
	}

	bool SCBConfigReqPS::OTCAB_REQ::get()
	{
		return this->otcabReq;
	}

	void SCBConfigReqPS::OTCAB_REQ::set(bool value)
	{
		if (value == true)
		{
			this->data.ResponseReq |= 0x02;
			otcabReq = true;
		}
		else
		{
			data.ResponseReq &= 0xFD;
			otcabReq = false;
		}
	}

	bool SCBConfigReqPS::WSD_REQ::get()
	{
		return wsdReq;
	}

	void SCBConfigReqPS::WSD_REQ::set(bool value)
	{
		if (value == true)
		{
			data.ResponseReq |= 0x04;
			wsdReq = true;
		}
		else
		{
			data.ResponseReq &= 0xFB;
			wsdReq = false;
		}
	}

	bool SCBConfigReqPS::TBL_REQ::get()
	{
		return tblReq;
	}

	void SCBConfigReqPS::TBL_REQ::set(bool value)
	{
		if (value == true)
		{
			data.ResponseReq |= 0x08;
			tblReq = true;
		}
		else
		{
			data.ResponseReq &= 0xF7;
			tblReq = false;
		}
	}

	SCBConfigReqPS::SCBConfigReqPS(AxisCtlProtoStuct package)
	{
		data.id = 0x461;
		data.ResponseReq = package.frameData[0];
		if ((data.ResponseReq & 0x01) != 0)
		{
			otcxyzReq = true;
		}
		else
		{
			otcxyzReq = false;
		}

		if ((data.ResponseReq & 0x02) != 0)
		{
			otcabReq = true;
		}
		else
		{
			otcabReq = false;
		}
		if ((data.ResponseReq & 0x04) != 0)
		{
			wsdReq = true;
		}
		else
		{
			wsdReq = false;
		}
		if ((data.ResponseReq & 0x08) != 0)
		{
			tblReq = true;
		}
		else
		{
			tblReq = false;
		}
	}


	/*******************************************/
	//   17. SCB File Transfer Request (0x470)  /
	/*******************************************/

	SCBFileTranReqMsgPS::SCBFileTranReqMsgPS(AxisCtlProtoStuct frame)
	{
		data.id = 0x470;
		data.DownloadTarget = frame.frameData[0];
		data.TransferType = frame.frameData[1];
		data.PlaceHolder = 0x00;
		data.TransSizeM = frame.frameData[2];
		data.TransSizeC = frame.frameData[3];
		data.TransSizeL = frame.frameData[4];
		data.SFCode = frame.frameData[5];

		switch (data.DownloadTarget)
		{
		case 0:
			downloadTarget = SCBDownloadTarget::OTCXYZ;
			break;
		case 1:
			downloadTarget = SCBDownloadTarget::OTCAB;
			break;
		case 2:
			downloadTarget = SCBDownloadTarget::WSD;
			break;
		case 3:
			downloadTarget = SCBDownloadTarget::TBL;
			break;
		default:
			break;
		}

		switch (data.TransferType)
		{
		case 0:
			transferType = SCBTransferType::BootStrap;
			break;
		case 1:
			transferType = SCBTransferType::BootCode;
			break;
		case 2:
			transferType = SCBTransferType::AppCode;
			break;
		case 3:
			transferType = SCBTransferType::FPGACode;
			break;
		case 4:
			transferType = SCBTransferType::MotorPara;
			break;
		case 5:
			transferType = SCBTransferType::CalFileDown;
			break;
		case 10:
			transferType = SCBTransferType::CalFileUp;
			break;
		default:
			break;
		}

		transferSize = data.TransSize;

		switch (data.SFCode)
		{
		case 0x00:
			sfCode = SCBSFCode::TransferStart;
			break;
		case 0x01:
			sfCode = SCBSFCode::TransferFinish;
			break;
		default:
			break;
		}
	}

	SCBDownloadTarget SCBFileTranReqMsgPS::DOWNLOAD_TARGET::get()
	{
		return downloadTarget;
	}

	void SCBFileTranReqMsgPS::DOWNLOAD_TARGET::set(SCBDownloadTarget value)
	{
		switch (value)
		{
		case SCBDownloadTarget::OTCXYZ:
			data.DownloadTarget = 0x00;
			break;
		case SCBDownloadTarget::OTCAB:
			data.DownloadTarget = 0x01;
			break;
		case SCBDownloadTarget::WSD:
			data.DownloadTarget = 0x02;
			break;
		case SCBDownloadTarget::TBL:
			data.DownloadTarget = 0x03;
			break;
		default:
			break;

		}
		downloadTarget = value;
	}

	SCBTransferType SCBFileTranReqMsgPS::TRANSFER_TYPE::get()
	{
		return transferType;
	}

	void SCBFileTranReqMsgPS::TRANSFER_TYPE::set(SCBTransferType value)
	{
		switch (value)
		{
		case SCBTransferType::BootStrap:
			data.TransferType = 0x00;
			break;
		case SCBTransferType::BootCode:
			data.TransferType = 0x01;
			break;
		case SCBTransferType::AppCode:
			data.TransferType = 0x02;
			break;
		case SCBTransferType::FPGACode:
			data.TransferType = 0x03;
			break;
		case SCBTransferType::MotorPara:
			data.TransferType = 0x04;
			break;
		case SCBTransferType::CalFileDown:
			data.TransferType = 0x05;
			break;
		case SCBTransferType::CalFileUp:
			data.TransferType = 0x0A;
			break;
		default:
			break;
		}

		transferType = value;
	}

	UInt32 SCBFileTranReqMsgPS::TRANSFER_SIZE::get()
	{
		return transferSize;
	}

	void SCBFileTranReqMsgPS::TRANSFER_SIZE::set(UInt32 value)
	{
		data.TransSize = value;
		transferSize = value;
	}

	SCBSFCode SCBFileTranReqMsgPS::SFCODE::get()
	{
		return sfCode;
	}

	void SCBFileTranReqMsgPS::SFCODE::set(SCBSFCode value)
	{
		if (value == SCBSFCode::TransferStart)
		{
			data.SFCode = 0x00;
		}
		else
		{
			data.SFCode = 0x01;
		}
		sfCode = value;
	}


	/*******************************************/
	//   22. SCB File Transfer Response (0x490) /
	/*******************************************/
	SCBFileTranRespMsgPS::SCBFileTranRespMsgPS(AxisCtlProtoStuct frame)
	{
		data.id = 0x490;
		data.TransferSource = frame.frameData[0];
		data.TransferType = frame.frameData[1];
		data.Status = frame.frameData[2];

		switch (data.TransferSource)
		{
		case 0:
			transferSource = SCBDownloadTarget::OTCXYZ;
			break;
		case 1:
			transferSource = SCBDownloadTarget::OTCAB;
			break;
		case 2:
			transferSource = SCBDownloadTarget::WSD;
			break;
		case 3:
			transferSource = SCBDownloadTarget::TBL;
			break;
		default:
			break;
		}

		switch (data.TransferType)
		{
		case 0:
			transferType = SCBTransferType::BootStrap;
			break;
		case 1:
			transferType = SCBTransferType::BootCode;
			break;
		case 2:
			transferType = SCBTransferType::AppCode;
			break;
		case 3:
			transferType = SCBTransferType::FPGACode;
			break;
		case 4:
			transferType = SCBTransferType::MotorPara;
			break;
		case 5:
			transferType = SCBTransferType::CalFileDown;
			break;
		case 10:
			transferType = SCBTransferType::CalFileUp;
			break;
		}

		status = data.Status;
	}

	SCBDownloadTarget SCBFileTranRespMsgPS::TRANS_SOURCE::get()
	{
		return transferSource;
	}

	void SCBFileTranRespMsgPS::TRANS_SOURCE::set(SCBDownloadTarget value)
	{
		switch (value)
		{
			case SCBDownloadTarget::OTCXYZ:
				data.TransferSource = 0;
				break;
			case SCBDownloadTarget::OTCAB:
				data.TransferSource = 1;
				break;
			case SCBDownloadTarget::WSD:
				data.TransferSource = 2;
				break;
			case SCBDownloadTarget::TBL:
				data.TransferSource = 3;
				break;
			default:
				break;
		}

		transferSource = value;
	}

	SCBTransferType SCBFileTranRespMsgPS::TRANS_TYPE::get()
	{
		return transferType;
	}

	void SCBFileTranRespMsgPS::TRANS_TYPE::set(SCBTransferType value)
	{
		switch (value)
		{
		case SCBTransferType::BootStrap:
			data.TransferType = 0;
			break;
		case SCBTransferType::BootCode:
			data.TransferType = 1;
			break;
		case SCBTransferType::AppCode:
			data.TransferType = 2;
			break;
		case SCBTransferType::FPGACode:
			data.TransferType = 3;
			break;
		case SCBTransferType::MotorPara:
			data.TransferType = 4;
			break;
		case SCBTransferType::CalFileDown:
			data.TransferType = 5;
			break;
		case SCBTransferType::CalFileUp:
			data.TransferType = 10;
			break;
		default:
			break;
		}
		transferType = value;
	}

	Byte SCBFileTranRespMsgPS::STATUS::get()
	{
		return status;
	}

	void SCBFileTranRespMsgPS::STATUS::set(Byte value)
	{
		data.Status = value;
		status = value;
	}


	/*******************************************/
	//  23. SCB Block Transfer Response (0x491) /
	/*******************************************/

	SCBBLKTranRespMsgPS::SCBBLKTranRespMsgPS(AxisCtlProtoStuct frame)
	{
		data.id = 0x491;
		data.TransferSource = frame.frameData[0];
		data.TransferSize = frame.frameData[1];
		data.BLKSeqNumM = frame.frameData[2];
		data.BLKSeqNumL = frame.frameData[3];
		data.Status = frame.frameData[4];

		switch (data.TransferSource)
		{
		case 0x00:
			transferSource = SCBDownloadTarget::OTCXYZ;
			break;
		case 0x01:
			transferSource = SCBDownloadTarget::OTCAB;
			break;
		case 0x02:
			transferSource = SCBDownloadTarget::WSD;
			break;
		case 0x03:
			transferSource = SCBDownloadTarget::TBL;
			break;
		default:
			break;
		}

		transferSize = data.TransferSize + 1;
		blockSeqNum = data.BLKSeqNum;
		Status = data.Status;
	}


	SCBDownloadTarget SCBBLKTranRespMsgPS::TRAN_SOURCE::get()
	{
		return transferSource;
	}

	void SCBBLKTranRespMsgPS::TRAN_SOURCE::set(SCBDownloadTarget value)
	{
		switch (value)
		{
		case SCBDownloadTarget::OTCXYZ:
			data.TransferSource = 0x00;
			break;
		case SCBDownloadTarget::OTCAB:
			data.TransferSource = 0x01;
			break;
		case SCBDownloadTarget::WSD:
			data.TransferSource = 0x02;
			break;
		case SCBDownloadTarget::TBL:
			data.TransferSource = 0x03;
			break;
		default:
			break;
		}

		transferSource = value;
	}

	UInt16 SCBBLKTranRespMsgPS::TRAN_SIZE::get()
	{
		return transferSize;
	}

	void SCBBLKTranRespMsgPS::TRAN_SIZE::set(UInt16 value)
	{
		data.TransferSize = value - 1;
		transferSize = value;
	}

	UInt16 SCBBLKTranRespMsgPS::BLK_SEQNUM::get()
	{
		return blockSeqNum;
	}

	void SCBBLKTranRespMsgPS::BLK_SEQNUM::set(UInt16 value)
	{
		data.BLKSeqNum = value;
		blockSeqNum = value;
	}

	Byte SCBBLKTranRespMsgPS::STATUS::get()
	{
		return Status;
	}

	void SCBBLKTranRespMsgPS::STATUS::set(Byte value)
	{
		data.Status = value;
		Status = value;
	}

/*************************  All Controllers to SCB Messages  *************************/
	/*****************************************************************/
	//   1. Axis Controller Sync Move Responses (0x125 through 0x127) /
	/*****************************************************************/
	AxisSyncRespMsgPS::AxisSyncRespMsgPS(ControlType type)
	{
		switch (type)
		{
		case ControlType::OTCXYZ:
			data.id = 0x125;
			break;
		case ControlType::OTCAB:
			data.id = 0x126;
			break;
		case ControlType::TBL:
			data.id = 0x127;
			break;
		default:
			break;
		}
	}

	void AxisSyncRespMsgPS::SYNC_STATUS::set(SyncStatus status)
	{
		syncStatus = status;
		data.SyncMoveStatus = (int)syncStatus;
	}

	void AxisSyncRespMsgPS::ERROR_CODE::set(Byte error)
	{
		errorCode = error;
		data.ErrorCode = errorCode;
	}

	AxisCtlProtoStuct AxisSyncRespMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 2;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.SyncMoveStatus;
		frame.frameData[1] = data.ErrorCode;

		return frame;
	}

	/*************************************************************/
	//   3. Axis Controller File Transfer Response (0x280)        /
	/*************************************************************/
	AxisFileTranRespMsgPS::AxisFileTranRespMsgPS()
	{
		data.id = 0x280;
	}

	SCBDownloadTarget AxisFileTranRespMsgPS::DOWNLOAD_TARGET::get()
	{
		return downloadTarget;
	}

	void AxisFileTranRespMsgPS::DOWNLOAD_TARGET::set(SCBDownloadTarget value)
	{
		switch (value)
		{
		case SCBDownloadTarget::OTCXYZ:
			data.DownloadTarget = 0x00;
			break;
		case SCBDownloadTarget::OTCAB:
			data.DownloadTarget = 0x01;
			break;
		case SCBDownloadTarget::WSD:
			data.DownloadTarget = 0x02;
			break;
		case SCBDownloadTarget::TBL:
			data.DownloadTarget = 0x03;
			break;
		default:
			break;
		}
		downloadTarget = value;
	}


	SCBTransferType AxisFileTranRespMsgPS::DOWNLOAD_TYPE::get()
	{
		return downloadType;

	}

	void AxisFileTranRespMsgPS::DOWNLOAD_TYPE::set(SCBTransferType value)
	{
		switch (value)
		{
		case SCBTransferType::BootStrap:
			data.DownloadType = 0x00;
			break;
		case SCBTransferType::BootCode:
			data.DownloadType = 0x01;
			break;
		case SCBTransferType::AppCode:
			data.DownloadType = 0x02;
			break;
		case SCBTransferType::FPGACode:
			data.DownloadType = 0x03;
			break;
		case SCBTransferType::MotorPara:
			data.DownloadType = 0x04;
			break;
		case SCBTransferType::CalFileDown:
			data.DownloadType = 0x05;
			break;
		case SCBTransferType::CalFileUp:
			data.DownloadType = 0x0A;
			break;
		default:
			break;
		}

		downloadType = value;
	}

	Byte AxisFileTranRespMsgPS::STATUSCODE::get()
	{
		return status;
	}

	void AxisFileTranRespMsgPS::STATUSCODE::set(Byte value)
	{
		data.Status = value;
		status = value;
	}

	AxisCtlProtoStuct AxisFileTranRespMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 3;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.DownloadTarget;
		frame.frameData[1] = data.DownloadType;
		frame.frameData[2] = data.Status;

		return frame;
	}

	/*************************************************************/
	//   4. Axis Controller File Transfer Requests (0x295-0x298)  /
	/*************************************************************/
	AxisFileTranReqMsgPS::AxisFileTranReqMsgPS(ControlType controlType)
	{
		switch (controlType)
		{
		case ControlType::OTCXYZ:
			data.id = 0x295;
			break;
		case ControlType::OTCAB:
			data.id = 0x296;
			break;
		case ControlType::WSD:
			data.id = 0x297;
			break;
		case ControlType::TBL:
			data.id = 0x298;
			break;
		default:
			break;
		}
		data.PlaceHolder = 0x00;
		data.TransferType = 10;
	}

	UInt32 AxisFileTranReqMsgPS::TRANSFER_SIZE::get()
	{
		return transferSize;
	}

	void AxisFileTranReqMsgPS::TRANSFER_SIZE::set(UInt32 value)
	{
		data.TransferSize = value;
		transferSize = value;
	}

	SCBSFCode AxisFileTranReqMsgPS::SFCODE::get()
	{
		return sfCode;
	}

	void AxisFileTranReqMsgPS::SFCODE::set(SCBSFCode value)
	{
		if (value == SCBSFCode::TransferStart)
		{
			data.SFCode = 0;
		}
		else
		{
			data.SFCode = 1;
		}
		sfCode = value;
	}

	AxisCtlProtoStuct AxisFileTranReqMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 5;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.TransferType;
		frame.frameData[1] = data.TransferSizeM;
		frame.frameData[2] = data.TransferSizeC;
		frame.frameData[3] = data.TransferSizeL;
		frame.frameData[4] = data.SFCode;

		return frame;
	}


	/*********************************************************************/
	//   5. Axis Controller File Transfer Axis Configuration (0x291)      /
	/*********************************************************************/
	AxisFileTranCfgMsgPS::AxisFileTranCfgMsgPS(Byte currentCode)
	{
		data.id = 0x291;
		swCode = currentCode;
		data.SWCodeB7 = ((currentCode & 0x80) == 0) ? '0' : '1';
		data.SWCodeB6 = ((currentCode & 0x40) == 0) ? '0' : '1';
		data.SWCodeB5 = ((currentCode & 0x20) == 0) ? '0' : '1';
		data.SWCodeB4 = ((currentCode & 0x10) == 0) ? '0' : '1';
		data.SWCodeB3 = ((currentCode & 0x08) == 0) ? '0' : '1';
		data.SWCodeB2 = ((currentCode & 0x04) == 0) ? '0' : '1';
		data.SWCodeB1 = ((currentCode & 0x02) == 0) ? '0' : '1';
		data.SWCodeB0 = ((currentCode & 0x01) == 0) ? '0' : '1';
	}

	Byte AxisFileTranCfgMsgPS::SWCODE::get()
	{
		return swCode;
	}

	void AxisFileTranCfgMsgPS::SWCODE::set(Byte code)
	{
		swCode = code;
		data.SWCodeB7 = (((code & 0x80) == 0) ? '0' : '1');
		data.SWCodeB6 = (((code & 0x40) == 0) ? '0' : '1');
		data.SWCodeB5 = (((code & 0x20) == 0) ? '0' : '1');
		data.SWCodeB4 = (((code & 0x10) == 0) ? '0' : '1');
		data.SWCodeB3 = (((code & 0x08) == 0) ? '0' : '1');
		data.SWCodeB2 = (((code & 0x04) == 0) ? '0' : '1');
		data.SWCodeB1 = (((code & 0x02) == 0) ? '0' : '1');
		data.SWCodeB0 = (((code & 0x01) == 0) ? '0' : '1');
	}

	AxisCtlProtoStuct AxisFileTranCfgMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 8;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.SWCodeB7;
		frame.frameData[1] = data.SWCodeB6;
		frame.frameData[2] = data.SWCodeB5;
		frame.frameData[3] = data.SWCodeB4;
		frame.frameData[4] = data.SWCodeB3;
		frame.frameData[5] = data.SWCodeB2;
		frame.frameData[6] = data.SWCodeB1;
		frame.frameData[7] = data.SWCodeB0;

		return frame;
	}



	/******************************************************/
	//   6. Axis Controller Block Upload Request (0x292)   /
	/******************************************************/
	CtlBLKUPReqMsgPS::CtlBLKUPReqMsgPS()
	{
		data.id = 0x292;
	}

	SCBDownloadTarget CtlBLKUPReqMsgPS::TRANS_SOURCE::get()
	{
		return transferSource;
	}

	void CtlBLKUPReqMsgPS::TRANS_SOURCE::set(SCBDownloadTarget value)
	{
		switch (value)
		{
			case SCBDownloadTarget::OTCXYZ:
				data.TransferSource = 0x00;
				break;
			case SCBDownloadTarget::OTCAB:
				data.TransferSource = 0x01;
				break;
			case SCBDownloadTarget::WSD:
				data.TransferSource = 0x02;
				break;
			case SCBDownloadTarget::TBL:
				data.TransferSource = 0x03;
				break;
			default:
				break;
		}
		transferSource = value;
	}

	UInt16 CtlBLKUPReqMsgPS::TRANS_SIZE::get()
	{
		return transferSize;
	}

	void CtlBLKUPReqMsgPS::TRANS_SIZE::set(UInt16 value)
	{
		transferSize = value;
		data.TransferSize = value - 1;
	}

	UInt16 CtlBLKUPReqMsgPS::BLOCK_SEQNUM::get()
	{
		return blockSeqNum;
	}

	void CtlBLKUPReqMsgPS::BLOCK_SEQNUM::set(UInt16 value)
	{
		data.BKLSeqNum = value;
		blockSeqNum = value;
	}

	UInt16 CtlBLKUPReqMsgPS::CHECKSUM::get()
	{
		return checkSum;
	}

	void CtlBLKUPReqMsgPS::CHECKSUM::set(UInt16 value)
	{
		data.CheckSum = value;
		checkSum = value;
	}

	AxisCtlProtoStuct CtlBLKUPReqMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 6;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.TransferSource;
		frame.frameData[1] = data.TransferSize;
		frame.frameData[2] = data.BKLSeqNumM;
		frame.frameData[3] = data.BKLSeqNumL;
		frame.frameData[4] = data.CheckSumM;
		frame.frameData[5] = data.CheckSumL;

		return frame;
	}

	/**********************************************/
	//   7. Axis Controller Data Upload (0x293)    /
	/**********************************************/
	AxisDataUpMsgPS::AxisDataUpMsgPS()
	{
		data.id = 0x293;
		dataCount = 0;
	}

	Byte AxisDataUpMsgPS::RD_BYTE0::get()
	{
		return recordByte0;
	}

	void AxisDataUpMsgPS::RD_BYTE0::set(Byte value)
	{
		data.RecordByte0 = value;
		recordByte0 = value;
	}

	Byte AxisDataUpMsgPS::RD_BYTE1::get()
	{
		return recordByte1;
	}

	void AxisDataUpMsgPS::RD_BYTE1::set(Byte value)
	{
		data.RecordByte1 = value;
		recordByte1 = value;
	}

	Byte AxisDataUpMsgPS::RD_BYTE2::get()
	{
		return recordByte2;
	}

	void AxisDataUpMsgPS::RD_BYTE2::set(Byte value)
	{
		data.RecordByte2 = value;
		recordByte2 = value;
	}

	Byte AxisDataUpMsgPS::RD_BYTE3::get()
	{
		return recordByte3;
	}

	void AxisDataUpMsgPS::RD_BYTE3::set(Byte value)
	{
		data.RecordByte3 = value;
		recordByte3 = value;
	}

	Byte AxisDataUpMsgPS::RD_BYTE4::get()
	{
		return recordByte4;
	}

	void AxisDataUpMsgPS::RD_BYTE4::set(Byte value)
	{
		data.RecordByte4 = value;
		recordByte4 = value;
	}

	Byte AxisDataUpMsgPS::RD_BYTE5::get()
	{
		return recordByte5;
	}

	void AxisDataUpMsgPS::RD_BYTE5::set(Byte value)
	{
		data.RecordByte5 = value;
		recordByte5 = value;
	}

	Byte AxisDataUpMsgPS::RD_BYTE6::get()
	{
		return recordByte6;
	}

	void AxisDataUpMsgPS::RD_BYTE6::set(Byte value)
	{
		data.RecordByte6 = value;
		recordByte6 = value;
	}

	Byte AxisDataUpMsgPS::RD_BYTE7::get()
	{
		return recordByte7;
	}

	void AxisDataUpMsgPS::RD_BYTE7::set(Byte value)
	{
		data.RecordByte7 = value;
		recordByte7 = value;
	}

	AxisCtlProtoStuct AxisDataUpMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = dataCount;
		frame.frameData = gcnew array<Byte>(frame.size);
		for (UINT i = 0; i < dataCount; i++)
		{
			frame.frameData[i] = *(&(data.RecordByte0) + i);
		}
		return frame;
	}

	/*************************************************************/
	//   9. Axis Controller Configuration Responses (0x2A0-0x2A3) /
	/*************************************************************/

	ControlHWMode CtlConfigRespPS::MODE::get()
	{
		return mode;
	}

	void CtlConfigRespPS::MODE::set(ControlHWMode value)
	{
		switch (value)
		{
		case ControlHWMode::BOOT:
			data.BoardInfo &= 0x7F;
			break;
		default:
			data.BoardInfo |= 0x80;
			break;
		}
		mode = value;
	}

	ControlHardType CtlConfigRespPS::BoardType::get()
	{
		return type;
	}

	void CtlConfigRespPS::BoardType::set(ControlHardType value)
	{
		switch (value)
		{
		case ControlHardType::DMC:
			data.BoardInfo &= 0x8F;
			data.BoardInfo |= DMCBoardType;
			break;
		case ControlHardType::TMC:
			data.BoardInfo &= 0x8F;
			data.BoardInfo |= TMCBoardType;
			break;
		default:
			data.BoardInfo &= 0x8F;
			data.BoardInfo |= InvalidType;
			break;
		}

		type = value;
	}

	UINT8 CtlConfigRespPS::HARDWARE_VERSION::get()
	{
		return hardwareVer;
	}

	void CtlConfigRespPS::HARDWARE_VERSION::set(UINT8 value)
	{
		data.BoardInfo &= 0xF0;
		data.BoardInfo += value;
		hardwareVer = value;
	}

	Byte CtlConfigRespPS::SWITCH_CODE::get()
	{
		return swCode;
	}

	void CtlConfigRespPS::SWITCH_CODE::set(Byte value)
	{
		data.SWCode = value;
		swCode = value;
	}

	Byte CtlConfigRespPS::BOOT_TAG::get()
	{
		return bootTagVer;
	}

	void CtlConfigRespPS::BOOT_TAG::set(Byte value)
	{
		data.BootTagVer = value;
		bootTagVer = value;
	}

	WORD CtlConfigRespPS::BOOT_REV::get()
	{
		return bootRevision;
	}

	void CtlConfigRespPS::BOOT_REV::set(WORD value)
	{
		data.BootRev = value;
		bootRevision = value;
	}

	Byte CtlConfigRespPS::FPGA_TAG::get()
	{
		return fpgaTagVer;
	}

	void CtlConfigRespPS::FPGA_TAG::set(Byte value)
	{
		data.FPGATagVer = value;
		fpgaTagVer = value;
	}

	WORD CtlConfigRespPS::FPGA_REV::get()
	{
		return fpgaRevision;
	}

	void CtlConfigRespPS::FPGA_REV::set(WORD value)
	{
		data.FPGARev = value;
		fpgaRevision = value;
	}

	CtlConfigRespPS::CtlConfigRespPS(ControlType controlType)
	{
		switch (controlType)
		{
		case ControlType::OTCXYZ:
			data.id = 0x2A0;
			break;
		case ControlType::OTCAB:
			data.id = 0x2A1;
			break;
		case ControlType::WSD:
			data.id = 0x2A2;
			break;
		case ControlType::TBL:
			data.id = 0x2A3;
			break;
		default:
			break;
		}

	}

	AxisCtlProtoStuct CtlConfigRespPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 8;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.BoardInfo;
		frame.frameData[1] = data.SWCode;
		frame.frameData[2] = data.BootTagVer;
		frame.frameData[3] = data.BootRevM;
		frame.frameData[4] = data.BootRevL;
		frame.frameData[5] = data.FPGATagVer;
		frame.frameData[6] = data.FPGARevM;
		frame.frameData[7] = data.FPGARevL;

		return frame;
	}


	/********************************************************************/
	//   10. Axis Controller Application Version Responses (0x2B0-0x2B3) /
	/********************************************************************/

	Byte CtlAppVerRspPS::APP_TAG_VER::get()
	{
		return appTagVer;
	}

	void CtlAppVerRspPS::APP_TAG_VER::set(Byte value)
	{
		data.AppTagVer = value;
		appTagVer = value;
	}

	WORD CtlAppVerRspPS::APP_REVISION::get()
	{
		return appRev;
	}

	void CtlAppVerRspPS::APP_REVISION::set(WORD value)
	{
		data.AppRev = value;
		appRev = value;
	}

	Byte CtlAppVerRspPS::MT_TAG_VER::get()
	{
		return motorParaTagVer;
	}

	void CtlAppVerRspPS::MT_TAG_VER::set(Byte value)
	{
		data.MtParaTagVer = value;
		motorParaTagVer = value;
	}

	WORD CtlAppVerRspPS::MT_PARA_REV::get()
	{
		return motorParaRev;
	}

	void CtlAppVerRspPS::MT_PARA_REV::set(WORD value)
	{
		data.MtParaRev = value;
		motorParaRev = value;
	}

	Byte CtlAppVerRspPS::OPTIONS::get()
	{
		return options;
	}

	void CtlAppVerRspPS::OPTIONS::set(Byte value)
	{
		data.Options = value;
		options = value;
	}

	CtlAppVerRspPS::CtlAppVerRspPS(ControlType controlType)
	{
		switch (controlType)
		{
		case ControlType::OTCXYZ:
			data.id = 0x2B0;
			break;
		case ControlType::OTCAB:
			data.id = 0x2B1;
			break;
		case ControlType::WSD:
			data.id = 0x2B2;
			break;
		case ControlType::TBL:
			data.id = 0x2B3;
			break;
		default:
			break;
		}

		data.Options = 0x00;
		options = 0x00;
	}

	AxisCtlProtoStuct CtlAppVerRspPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 7;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.AppTagVer;
		frame.frameData[1] = data.AppRevM;
		frame.frameData[2] = data.AppRevL;
		frame.frameData[3] = data.MtParaTagVer;
		frame.frameData[4] = data.MtParaRevM;
		frame.frameData[5] = data.MtParaRevL;
		frame.frameData[6] = data.Options;

		return frame;
	}



	/********************************************************************/
	//   11. Axis Controller Boot Strap Version Responses (0x2B5-0x2B8) /
	/********************************************************************/
	Byte CtlBootStrapVerRspPS::BOOTSTRAP_TAG_VER::get()
	{
		return bootStrapTagVer;
	}

	void CtlBootStrapVerRspPS::BOOTSTRAP_TAG_VER::set(Byte value)
	{
		data.BootStrapTag = value;
		bootStrapTagVer = value;
	}

	WORD CtlBootStrapVerRspPS::BOOTSTRAP_REV::get()
	{
		return bootStrapRev;
	}

	void CtlBootStrapVerRspPS::BOOTSTRAP_REV::set(WORD value)
	{
		data.BootStrapRev = value;
		bootStrapRev = value;
	}

	CtlBootStrapVerRspPS::CtlBootStrapVerRspPS(ControlType controlType)
	{
		switch (controlType)
		{
		case ControlType::OTCXYZ:
			data.id = 0x2B5;
			break;
		case ControlType::OTCAB:
			data.id = 0x2B6;
			break;
		case ControlType::WSD:
			data.id = 0x2B7;
			break;
		case ControlType::TBL:
			data.id = 0x2B8;
			break;
		default:
			break;
		}
	}

	AxisCtlProtoStuct  CtlBootStrapVerRspPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 3;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.BootStrapTag;
		frame.frameData[1] = data.BootStrapRevM;
		frame.frameData[2] = data.BootStrapRevL;

		return frame;
	}
	

	/***********************************************/
	//   14. WSD Tilt Center Vector Message (0x2D2) /
	/***********************************************/

	WSDTiltCenterVectMsgPS::WSDTiltCenterVectMsgPS()
	{
		data.id = 0x2D2;
	}

	double WSDTiltCenterVectMsgPS::RAD_TILTCENTER::get()
	{
		return radTiltCenter;
	}

	void WSDTiltCenterVectMsgPS::RAD_TILTCENTER::set(double value)
	{
		data.RadTiltCenter = (DWORD)(value * 10000000);
		radTiltCenter = value;
	}

	double WSDTiltCenterVectMsgPS::ANG_TILTCENTER::get()
	{
		return angTiltCenter;
	}

	void WSDTiltCenterVectMsgPS::ANG_TILTCENTER::set(double value)
	{
		data.AngTiltCenter = (DWORD)(value * 10000000); //TBD
		angTiltCenter = value;
	}

	AxisCtlProtoStuct WSDTiltCenterVectMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 8;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.RadTiltCenterByte3;
		frame.frameData[1] = data.RadTiltCenterByte2;
		frame.frameData[2] = data.RadTiltCenterByte1;
		frame.frameData[3] = data.RadTiltCenterByte0;
		frame.frameData[4] = data.AngTiltCenterByte3;
		frame.frameData[5] = data.AngTiltCenterByte2;
		frame.frameData[6] = data.AngTiltCenterByte1;
		frame.frameData[7] = data.AngTiltCenterByte0;

		return frame;
	}


	/********************************************************************/
	//   15. Axis Controller Auto Movement Responses (0x2E0 thru 0x2E3)   /
	/********************************************************************/
	AutoMoveRespMsgPS::AutoMoveRespMsgPS(ControlType type)
	{
		switch (type)
		{
		case ControlType::OTCXYZ:
			data.id = 0x2E0;
			break;
		case ControlType::OTCAB:
			data.id = 0x2E1;
			break;
		case ControlType::WSD:
			data.id = 0x2E2;
			break;
		case ControlType::TBL:
			data.id = 0x2E3;
			break;
		default:
			break;
		}
	}

	AutoMoveRespMsgPS::AutoMoveRespMsgPS(AxisCtlProtoStuct frame)
	{
		data.id = frame.id;
		data.AutoMoveStatus = frame.frameData[0];
		data.SWSetting = frame.frameData[1];

		switch (data.AutoMoveStatus)
		{
		case 0:
			autoMoveStatus = AMoveStatus::Complete;
			break;
		case 1:
			autoMoveStatus = AMoveStatus::Abort;
			break;
		case 2:
			autoMoveStatus = AMoveStatus::Error;
			break;
		case 3:
			autoMoveStatus = AMoveStatus::EnAccepted;
			break;
		case 4:
			autoMoveStatus = AMoveStatus::AModeAccepted;
			break;
		default:
			break;
		}

		swSetting = data.SWSetting;
	}

	AMoveStatus AutoMoveRespMsgPS::AUTOMOVE_STATUS::get()
	{
		return autoMoveStatus;
	}

	void AutoMoveRespMsgPS::AUTOMOVE_STATUS::set(AMoveStatus value)
	{
		switch (value)
		{
		case AMoveStatus::Complete:
			data.AutoMoveStatus = 0;
			break;
		case AMoveStatus::Abort:
			data.AutoMoveStatus = 1;
			break;
		case AMoveStatus::Error:
			data.AutoMoveStatus = 2;
			break;
		case AMoveStatus::EnAccepted:
			data.AutoMoveStatus = 3;
			break;
		case AMoveStatus::AModeAccepted:
			data.AutoMoveStatus = 4;
			break;
		default:
			break;
		}
		autoMoveStatus = value;
	}
	
	Byte AutoMoveRespMsgPS::SW_SETTING::get()
	{
		return swSetting;
	}

	void AutoMoveRespMsgPS::SW_SETTING::set(Byte value)
	{
		data.SWSetting = value;
		swSetting = value;
	}

	AxisCtlProtoStuct AutoMoveRespMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 2;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.AutoMoveStatus;
		frame.frameData[1] = data.SWSetting;

		return frame;
	}

	/********************************************************************/
	//   16. Axis Controller Error Status Responses (0x2F0 thru 0x2F3)   /
	/********************************************************************/

	CtlErrStatRespMsgPS::CtlErrStatRespMsgPS(ControlType controlType)
	{
		switch (controlType)
		{
		case ControlType::OTCXYZ:
			data.id = 0x2F0;
			break;
		case ControlType::OTCAB:
			data.id = 0x2F1;
			break;
		case ControlType::WSD:
			data.id = 0x2F2;
			break;
		case ControlType::TBL:
			data.id = 0x2F3;
			break;
		default:
			break;
		}

		data.ErrorCode = 0x00;
		data.UpdateReason = 0x80;
	}

	WORD CtlErrStatRespMsgPS::ERROR_CODE::get()
	{
		return errorCode;
	}

	void CtlErrStatRespMsgPS::ERROR_CODE::set(WORD value)
	{
		data.ErrorCode = value;
		errorCode = value;
	}

	CtlErrStatRespUpdateReason CtlErrStatRespMsgPS::UPDATEREASON::get()
	{
		return updateReason;
	}

	void CtlErrStatRespMsgPS::UPDATEREASON::set(CtlErrStatRespUpdateReason value)
	{
		if (value == CtlErrStatRespUpdateReason::PeriodicUpdate)
		{
			data.UpdateReason = 0x00;
		}
		else
		{
			data.UpdateReason = 0x80;
		}

		updateReason = value;
	}

	AxisCtlProtoStuct CtlErrStatRespMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 3;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.ErrorCodeM;
		frame.frameData[1] = data.ErrorCodeL;
		frame.frameData[2] = data.UpdateReason;

		return frame;
	}

	/*************************  SCB to OTC Controllers Messages  *************************/
	/*****************************************************/
	//   1. SCB OTC XYZ Set Auto Move Destination (0x400) /
	/*****************************************************/
	SCBXYZSetAutoDesMsgPS::SCBXYZSetAutoDesMsgPS(AxisCtlProtoStuct frame)
	{
		data.id = frame.id;
		data.XPosM = frame.frameData[0];
		data.XPosL = frame.frameData[1];
		data.YPosM = frame.frameData[2];
		data.YPosL = frame.frameData[3];
		data.ZPosM = frame.frameData[4];
		data.ZPosL = frame.frameData[5];

		xPos = (Int16)data.XPos*0.25;
		yPos = (Int16)data.YPos*0.25;
		zPos = (Int16)data.ZPos*0.25;
	}


	double SCBXYZSetAutoDesMsgPS::X_POS::get()
	{
		return xPos;
	}

	void SCBXYZSetAutoDesMsgPS::X_POS::set(double value)
	{
		data.XPos = (WORD)(value * 4);
		xPos = value;
	}

	double SCBXYZSetAutoDesMsgPS::Y_POS::get()
	{
		return yPos;
	}

	void SCBXYZSetAutoDesMsgPS::Y_POS::set(double value)
	{
		data.YPos = (WORD)(value * 4);
		yPos = value;
	}

	double SCBXYZSetAutoDesMsgPS::Z_POS::get()
	{
		return zPos;
	}

	void SCBXYZSetAutoDesMsgPS::Z_POS::set(double value)
	{
		data.ZPos = (WORD)(value * 4);
		zPos = value;
	}

	/*****************************************************/
	//   2.  SCB OTC AB Set Auto Move Destination (0x401) /
	/*****************************************************/
	SCBABSetAutoDesMsgPS::SCBABSetAutoDesMsgPS(AxisCtlProtoStuct frame)
	{
		data.id = frame.id;
		data.APosM = frame.frameData[0];
		data.APosL = frame.frameData[1];
		data.BPosM = frame.frameData[2];
		data.BPosL = frame.frameData[3];
		if (data.APos != 0x8000)
		{
			aPos = (Int16)data.APos * 0.1;
		}
		else
		{
			aPos = -32768;
		}
		if (data.BPos != 0x8000)
		{
			bPos = (Int16)data.BPos * 0.1;
		}
		else
		{
			bPos = -32768;
		}
		
	}

	double SCBABSetAutoDesMsgPS::A_POS::get()
	{
		return aPos;
	}

	void SCBABSetAutoDesMsgPS::A_POS::set(double value)
	{
		data.APos = (WORD)(value * 10);
		aPos = value;
	}

	double SCBABSetAutoDesMsgPS::B_POS::get()
	{
		return bPos;
	}

	void SCBABSetAutoDesMsgPS::B_POS::set(double value)
	{
		data.BPos = (WORD)(value * 10);
		bPos = value;
	}

	/****************************************/
	//   3. SCB OTC XYZ Set Room Cal (0x500) /
	/****************************************/

	OTCXYZSetRMCalMsgPS::OTCXYZSetRMCalMsgPS(AxisCtlProtoStuct frame)
	{
		data.id = 0x500;
		data.XPosM = frame.frameData[0];
		data.XPosL = frame.frameData[1];
		data.YPosM = frame.frameData[2];
		data.YPosL = frame.frameData[3];
		data.ZPosM = frame.frameData[4];
		data.ZPosL = frame.frameData[5];

		xPos = (Int16)data.XPos*0.25;
		yPos = (Int16)data.YPos*0.25;
		zPos = (Int16)data.ZPos*0.25;
	}

	double OTCXYZSetRMCalMsgPS::X_POS::get()
	{
		return xPos;
	}

	void OTCXYZSetRMCalMsgPS::X_POS::set(double value)
	{
		data.XPos = (WORD)(value * 4);
		xPos = value;
	}

	double OTCXYZSetRMCalMsgPS::Y_POS::get()
	{
		return yPos;
	}

	void OTCXYZSetRMCalMsgPS::Y_POS::set(double value)
	{
		data.YPos = (WORD)(value * 4);
		yPos = value;
	}

	double OTCXYZSetRMCalMsgPS::Z_POS::get()
	{
		return zPos;
	}

	void OTCXYZSetRMCalMsgPS::Z_POS::set(double value)
	{
		data.ZPos = (WORD)(value * 4);
		zPos = value;
	}

	AxisCtlProtoStuct OTCXYZSetRMCalMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 6;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.XPosM;
		frame.frameData[1] = data.XPosL;
		frame.frameData[2] = data.YPosM;
		frame.frameData[3] = data.YPosL;
		frame.frameData[4] = data.ZPosM;
		frame.frameData[5] = data.ZPosL;

		return frame;
	}



	/****************************************/
	//   4. SCB OTC Set Axis Cal (0x501)     /
	/****************************************/
	OTCXYZSetAxisCalMsgPS::OTCXYZSetAxisCalMsgPS(AxisCtlProtoStuct frame)
	{
		data.id = frame.id;
		data.PinAxisCal = frame.frameData[0];
		data.ABCal = frame.frameData[1];

		if ((data.PinAxisCal & 0x01) != 0)
		{
			zMinSet = true;
		}
		if ((data.PinAxisCal & 0x02) != 0)
		{
			xMinSet = true;
		}
		if ((data.PinAxisCal & 0x04) != 0)
		{
			yMinSet = true;
		}
		if ((data.PinAxisCal & 0x08) != 0)
		{
			zMaxSet = true;
		}
		if ((data.PinAxisCal & 0x10) != 0)
		{
			xMaxSet = true;
		}
		if ((data.PinAxisCal & 0x20) != 0)
		{
			yMaxSet = true;
		}
		if ((data.PinAxisCal & 0x80) != 0)
		{
			pinSet = true;
		}

		if ((data.ABCal & 0x01) != 0)
		{
			aMinSet = true;
		}
		if ((data.ABCal & 0x02) != 0)
		{
			bMinSet = true;
		}
		if ((data.ABCal & 0x04) != 0)
		{
			aMaxSet = true;
		}
		if ((data.ABCal & 0x08) != 0)
		{
			bMaxSet = true;
		}
	}

	bool OTCXYZSetAxisCalMsgPS::AMIN_SET::get()
	{
		return aMinSet;
	}

	void OTCXYZSetAxisCalMsgPS::AMIN_SET::set(bool value)
	{
		if (value == true)
		{
			data.ABCal |= 0x01;
			aMinSet = true;
		}
		else
		{
			data.ABCal &= 0xFE;
			aMinSet = false;
		}
	}

	bool OTCXYZSetAxisCalMsgPS::AMAX_SET::get()
	{
		return aMaxSet;
	}

	void OTCXYZSetAxisCalMsgPS::AMAX_SET::set(bool value)
	{
		if (value == true)
		{
			data.ABCal |= 0x04;
			aMaxSet = true;
		}
		else
		{
			data.ABCal &= 0xFB;
			aMaxSet = false;
		}
	}

	bool OTCXYZSetAxisCalMsgPS::BMIN_SET::get()
	{
		return bMinSet;
	}

	void OTCXYZSetAxisCalMsgPS::BMIN_SET::set(bool value)
	{
		if (value == true)
		{
			data.ABCal |= 0x02;
			bMinSet = true;
		}
		else
		{
			data.ABCal &= 0xFD;
			bMinSet = false;
		}
	}

	bool OTCXYZSetAxisCalMsgPS::BMAX_SET::get()
	{
		return bMaxSet;
	}

	void OTCXYZSetAxisCalMsgPS::BMAX_SET::set(bool value)
	{
		if (value == true)
		{
			data.ABCal |= 0x08;
			bMaxSet = true;
		}
		else
		{
			data.ABCal &= 0xF7;
			bMaxSet = false;
		}
	}

	bool OTCXYZSetAxisCalMsgPS::ZMIN_SET::get()
	{
		return zMinSet;
	}

	void OTCXYZSetAxisCalMsgPS::ZMIN_SET::set(bool value)
	{
		if (value == true)
		{
			data.PinAxisCal |= 0x01;
			zMinSet = true;
		}
		else
		{
			data.PinAxisCal &= 0xFE;
			zMinSet = false;
		}
	}

	bool OTCXYZSetAxisCalMsgPS::XMIN_SET::get()
	{
		return xMinSet;
	}

	void OTCXYZSetAxisCalMsgPS::XMIN_SET::set(bool value)
	{
		if (value == true)
		{
			data.PinAxisCal |= 0x02;
			xMinSet = true;
		}
		else
		{
			data.PinAxisCal &= 0xFD;
			xMinSet = false;
		}
	}

	bool OTCXYZSetAxisCalMsgPS::YMIN_SET::get()
	{
		return yMinSet;
	}

	void OTCXYZSetAxisCalMsgPS::YMIN_SET::set(bool value)
	{
		if (value == true)
		{
			data.PinAxisCal |= 0x04;
			yMinSet = true;
		}
		else
		{
			data.PinAxisCal &= 0xFB;
			yMinSet = false;
		}
	}

	bool OTCXYZSetAxisCalMsgPS::ZMAX_SET::get()
	{
		return zMaxSet;
	}

	void OTCXYZSetAxisCalMsgPS::ZMAX_SET::set(bool value)
	{
		if (value == true)
		{
			data.PinAxisCal |= 0x08;
			zMaxSet = true;
		}
		else
		{
			data.PinAxisCal &= 0xF7;
			zMaxSet = false;
		}
	}

	bool OTCXYZSetAxisCalMsgPS::XMAX_SET::get()
	{
		return xMaxSet;
	}

	void OTCXYZSetAxisCalMsgPS::XMAX_SET::set(bool value)
	{
		if (value == true)
		{
			data.PinAxisCal |= 0x10;
			xMaxSet = true;
		}
		else
		{
			data.PinAxisCal &= 0xEF;
			xMaxSet = false;
		}
	}

	bool OTCXYZSetAxisCalMsgPS::YMAX_SET::get()
	{
		return yMaxSet;
	}

	void OTCXYZSetAxisCalMsgPS::YMAX_SET::set(bool value)
	{
		if (value == true)
		{
			data.PinAxisCal |= 0x20;
			yMaxSet = true;
		}
		else
		{
			data.PinAxisCal &= 0xDF;
			yMaxSet = false;
		}
	}

	bool OTCXYZSetAxisCalMsgPS::PIN_SET::get()
	{
		return pinSet;
	}

	void OTCXYZSetAxisCalMsgPS::PIN_SET::set(bool value)
	{
		if (value == true)
		{
			data.PinAxisCal |= 0x80;
			pinSet = true;
		}
		else
		{
			data.PinAxisCal &= 0x7F;
			pinSet = false;
		}
	}

	AxisCtlProtoStuct OTCXYZSetAxisCalMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 2;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.PinAxisCal;
		frame.frameData[1] = data.ABCal;
		
		return frame;
	}


	/*************************  SCB to TBL Controllers Messages  *************************/
	/****************************************************/
	//   1. SCB TBL Set Auto Move Destination (0x403)    /
	/****************************************************/
	SCBTBLSetAutoDesMsgPS::SCBTBLSetAutoDesMsgPS(AxisCtlProtoStuct frame)
	{
		data.id = frame.id;
		data.XPosM = frame.frameData[0];
		data.XPosL = frame.frameData[1];
		data.YPosM = frame.frameData[2];
		data.YPosL = frame.frameData[3];
		data.ZPosM = frame.frameData[4];
		data.ZPosL = frame.frameData[5];

		xPos = (Int16)data.XPos * 0.25;
		yPos = (Int16)data.YPos * 0.25;
		zPos = (Int16)data.ZPos * 0.25;
	}

	double SCBTBLSetAutoDesMsgPS::X_POS::get()
	{
		return xPos;
	}

	double SCBTBLSetAutoDesMsgPS::Y_POS::get()
	{
		return yPos;
	}

	double SCBTBLSetAutoDesMsgPS::Z_POS::get()
	{
		return zPos;
	}

	/****************************************/
	//   2. SCB TBL Set Room Cal (0x700)     /
	/****************************************/
	TBLSetRMCalMsgPS::TBLSetRMCalMsgPS(AxisCtlProtoStuct frame)
	{
		data.id = 0x700;
		data.XPosM = frame.frameData[0];
		data.XPosL = frame.frameData[1];
		data.YPosM = frame.frameData[2];
		data.YPosL = frame.frameData[3];
		data.ZPosM = frame.frameData[4];
		data.ZPosL = frame.frameData[5];
		data.Orientation = frame.frameData[6];

		xPos = (Int16)data.XPos * 0.25;
		yPos = (Int16)data.YPos * 0.25;
		zPos = (Int16)data.ZPos * 0.25;

		switch (data.Orientation)
		{
		case 0x01:
			orientation = TBLRMOrientation::LeftWall;
			break;
		case 0x02:
			orientation = TBLRMOrientation::BackWall;
			break;
		case 0x03:
			orientation = TBLRMOrientation::RightWall;
			break;
		case 0x04:
			orientation = TBLRMOrientation::FrontWall;
			break;
		default:
			break;
		}
	}


	double TBLSetRMCalMsgPS::X_POS::get()
	{
		return xPos;
	}

	void TBLSetRMCalMsgPS::X_POS::set(double value)
	{
		data.XPos = (WORD)(value * 4);
		xPos = value;
	}

	double TBLSetRMCalMsgPS::Y_POS::get()
	{
		return yPos;
	}

	void TBLSetRMCalMsgPS::Y_POS::set(double value)
	{
		data.YPos = (WORD)(value * 4);
		yPos = value;
	}

	double TBLSetRMCalMsgPS::Z_POS::get()
	{
		return zPos;
	}

	void TBLSetRMCalMsgPS::Z_POS::set(double value)
	{
		data.ZPos = (WORD)(value * 4);
		zPos = value;
	}

	TBLRMOrientation TBLSetRMCalMsgPS::ORIENTATION::get()
	{
		return orientation;
	}

	void TBLSetRMCalMsgPS::ORIENTATION::set(TBLRMOrientation value)
	{
		switch (value)
		{
		case TBLRMOrientation::LeftWall:
			data.Orientation = 0x01;
			orientation = value;
			break;
		case TBLRMOrientation::BackWall:
			data.Orientation = 0x02;
			orientation = value;
			break;
		case TBLRMOrientation::RightWall:
			data.Orientation = 0x03;
			orientation = value;
			break;
		case TBLRMOrientation::FrontWall:
			data.Orientation = 0x04;
			orientation = value;
			break;
		default:
			break;
		}
	}

	/****************************************/
	//   3. SCB TBL Set Axis Cal (0x701)     /
	/****************************************/

	// ctor
	TBLSetAxisCalMsgPS::TBLSetAxisCalMsgPS(AxisCtlProtoStuct frame)
	{
		data.id = frame.id;
		data.AxisCal = frame.frameData[0];

		if (data.AxisCal & 0x01)
		{
			ZMinSet = true;
		}
		else
		{
			ZMinSet = false;
		}
		if (data.AxisCal & 0x02)
		{
			LongMinSet = true;
		}
		else
		{
			LongMinSet = false;
		}
		if (data.AxisCal & 0x04)
		{
			ZMaxSet = true;
		}
		else
		{
			ZMaxSet = false;
		}
		if (data.AxisCal & 0x08)
		{
			LongMaxSet = true;
		}
		else
		{
			LongMaxSet = false;
		}
		if (data.AxisCal & 0x80)
		{
			PinCal = true;
		}
		else
		{
			PinCal = false;
		}
	}

	bool TBLSetAxisCalMsgPS::ZMIN_SET::get()
	{
		return ZMinSet;
	}

	void TBLSetAxisCalMsgPS::ZMIN_SET::set(bool value)
	{
		if (value)
		{
			data.AxisCal |= 0x01;
			ZMinSet = true;
		}
		else
		{
			data.AxisCal &= 0xFE;
			ZMinSet = false;
		}
	}

	bool TBLSetAxisCalMsgPS::LONGMIN_SET::get()
	{
		return LongMinSet;
	}

	void TBLSetAxisCalMsgPS::LONGMIN_SET::set(bool value)
	{
		if (value)
		{
			data.AxisCal |= 0x02;
			LongMinSet = true;
		}
		else
		{
			data.AxisCal &= 0xFD;
			LongMinSet = false;
		}
	}

	bool TBLSetAxisCalMsgPS::ZMAX_SET::get()
	{
		return ZMaxSet;
	}

	void TBLSetAxisCalMsgPS::ZMAX_SET::set(bool value)
	{
		if (value)
		{
			data.AxisCal |= 0x04;
			ZMaxSet = true;
		}
		else
		{
			data.AxisCal &= 0xFB;
			ZMaxSet = false;
		}
	}

	bool TBLSetAxisCalMsgPS::LONGMAX_SET::get()
	{
		return LongMaxSet;
	}

	void TBLSetAxisCalMsgPS::LONGMAX_SET::set(bool value)
	{
		if (value)
		{
			data.AxisCal |= 0x08;
			LongMaxSet = true;
		}
		else
		{
			data.AxisCal &= 0xF7;
			LongMaxSet = false;
		}
	}

	bool TBLSetAxisCalMsgPS::PIN_CAL::get()
	{
		return PinCal;
	}

	void TBLSetAxisCalMsgPS::PIN_CAL::set(bool value)
	{
		if (value)
		{
			data.AxisCal |= 0x80;
			PinCal = true;
		}
		else
		{
			data.AxisCal &= 0x7F;
			PinCal = false;
		}
	}

	/*************************  SCB to WSD Controllers Messages  *************************/
	/**************************************************/
	//   1.  SCB WSD Set Auto Move Destination (0x402) /
	/**************************************************/
	SCBWSDSetAutoDesMsgPS::SCBWSDSetAutoDesMsgPS(AxisCtlProtoStuct frame)
	{
		data.id = frame.id;
		data.ZPosM = frame.frameData[0];
		data.ZPosL = frame.frameData[1];
		data.TiltPosM = frame.frameData[2];
		data.TiltPosL = frame.frameData[3];
		data.XPosM = frame.frameData[4];
		data.XPosL = frame.frameData[5];
		data.YPosM = frame.frameData[6];
		data.YPosL = frame.frameData[7];

		xPos = (Int16)data.XPos * 0.25;
		yPos = (Int16)data.YPos * 0.25;
		zPos = (Int16)data.ZPos * 0.25;
		if (data.TiltPos != 0x8000)
		{
			tiltPos = (Int16)data.TiltPos * 0.1;
		}
		else
		{
			tiltPos = -32768;
		}
		
	}

	double SCBWSDSetAutoDesMsgPS::TILT_POS::get()
	{
		return tiltPos;
	}

	double SCBWSDSetAutoDesMsgPS::X_POS::get()
	{
		return xPos;
	}
	double SCBWSDSetAutoDesMsgPS::Y_POS::get()
	{
		return yPos;
	}
	double SCBWSDSetAutoDesMsgPS::Z_POS::get()
	{
		return zPos;
	}

	/****************************************/
	//   2. SCB WSD Set Room Cal (0x600)     /
	/****************************************/

	WSDSetRMCalMsgPS::WSDSetRMCalMsgPS(AxisCtlProtoStuct frame)
	{
		data.id = 0x600;
		data.XPosM = frame.frameData[0];
		data.XPosL = frame.frameData[1];
		data.YPosM = frame.frameData[2];
		data.YPosL = frame.frameData[3];
		data.ZPosM = frame.frameData[4];
		data.ZPosL = frame.frameData[5];
		data.Orientation = frame.frameData[6];
		data.UpdateType = frame.frameData[7];

		xPos = (Int16)data.XPos * 0.25;
		yPos = (Int16)data.YPos * 0.25;
		zPos = (Int16)data.ZPos * 0.25;

		switch (data.Orientation)
		{
		case 0x01:
			orientation = WSDRMOrientation::LeftWall;
			break;
		case 0x02:
			orientation = WSDRMOrientation::BackWall;
			break;
		case 0x03:
			orientation = WSDRMOrientation::RightWall;
			break;
		case 0x04:
			orientation = WSDRMOrientation::FrontWall;
			break;
		default:
			break;
		}

		switch (data.UpdateType)
		{
		case 0x00:
			updateType = WSDRMCalUpdateType::Vertical;
			break;
		case 0x01:
			updateType = WSDRMCalUpdateType::Horizontal;
			break;
		case 0x02:
			updateType = WSDRMCalUpdateType::VertToHoriz;
			break;
		case 0x03:
			updateType = WSDRMCalUpdateType::HorizToVert;
			break;
		default:
			break;
		}
	}
		
	double WSDSetRMCalMsgPS::X_POS::get()
	{
		return xPos;
	}

	void WSDSetRMCalMsgPS::X_POS::set(double value)
	{
		data.XPos = (WORD)(value * 4);
		xPos = value;
	}

	double WSDSetRMCalMsgPS::Y_POS::get()
	{
		return yPos;
	}

	void WSDSetRMCalMsgPS::Y_POS::set(double value)
	{
		data.YPos = (WORD)(value * 4);
		yPos = value;
	}

	double WSDSetRMCalMsgPS::Z_POS::get()
	{
		return zPos;
	}

	void WSDSetRMCalMsgPS::Z_POS::set(double value)
	{
		data.ZPos = (WORD)(value * 4);
		zPos = value;
	}

	WSDRMOrientation WSDSetRMCalMsgPS::ORIENTATION::get()
	{
		return orientation;
	}

	void WSDSetRMCalMsgPS::ORIENTATION::set(WSDRMOrientation value)
	{
		switch (value)
		{
		case WSDRMOrientation::LeftWall:
			data.Orientation = 0x01;
			orientation = value;
			break;
		case WSDRMOrientation::BackWall:
			data.Orientation = 0x02;
			orientation = value;
			break;
		case WSDRMOrientation::RightWall:
			data.Orientation = 0x03;
			orientation = value;
			break;
		case WSDRMOrientation::FrontWall:
			data.Orientation = 0x04;
			orientation = value;
			break;
		default:
			break;
		}
	}


	WSDRMCalUpdateType WSDSetRMCalMsgPS::UPDATETYPE::get()
	{
		return updateType;
	}

	void WSDSetRMCalMsgPS::UPDATETYPE::set(WSDRMCalUpdateType value)
	{
		switch (value)
		{
		case WSDRMCalUpdateType::Vertical:
			data.UpdateType = 0x00;
			updateType = value;
			break;
		case WSDRMCalUpdateType::Horizontal:
			data.UpdateType = 0x01;
			updateType = value;
			break;
		case WSDRMCalUpdateType::VertToHoriz:
			data.UpdateType = 0x02;
			updateType = value;
			break;
		case WSDRMCalUpdateType::HorizToVert:
			data.UpdateType = 0x03;
			updateType = value;
			break;

		default:
			break;
		}
	}

	AxisCtlProtoStuct WSDSetRMCalMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 8;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.XPosM;
		frame.frameData[1] = data.XPosL;
		frame.frameData[2] = data.YPosM;
		frame.frameData[3] = data.YPosL;
		frame.frameData[4] = data.ZPosM;
		frame.frameData[5] = data.ZPosL;
		frame.frameData[6] = data.Orientation;
		frame.frameData[7] = data.UpdateType;

		return frame;
	}



	/****************************************/
	//   3. SCB WSD Set Axis Cal (0x601)     /
	/****************************************/
	WSDSetAxisCalMsgPS::WSDSetAxisCalMsgPS(AxisCtlProtoStuct frame)
	{
		data.id = frame.id;
		data.AxisCal = frame.frameData[0];

		if ((data.AxisCal & 0x01) != 0)
		{
			zMinSet = true;
		}
		else
		{
			zMinSet = false;
		}

		if ((data.AxisCal & 0x04) != 0)
		{
			zMaxSet = true;
		}
		else
		{
			zMaxSet = false;
		}

		if ((data.AxisCal & 0x08) != 0)
		{
			xRailMinSet = true;
		}
		else
		{
			xRailMinSet = false;
		}

		if ((data.AxisCal & 0x10) != 0)
		{
			xRailMaxSet = true;
		}
		else
		{
			xRailMaxSet = false;
		}

		if ((data.AxisCal & 0x80) != 0)
		{
			pinSet = true;
		}
		else
		{
			pinSet = false;
		}
	}


	bool WSDSetAxisCalMsgPS::ZMIN_SET::get()
	{
		return zMinSet;
	}

	void WSDSetAxisCalMsgPS::ZMIN_SET::set(bool value)
	{
		if (value == true)
		{
			data.AxisCal |= 0x01;
			zMinSet = true;
		}
		else
		{
			data.AxisCal &= 0xFE;
			zMinSet = false;
		}
	}

	bool WSDSetAxisCalMsgPS::ZMAX_SET::get()
	{
		return zMaxSet;
	}

	void WSDSetAxisCalMsgPS::ZMAX_SET::set(bool value)
	{
		if (value == true)
		{
			data.AxisCal |= 0x04;
			zMaxSet = true;
		}
		else
		{
			data.AxisCal &= 0xFB;
			zMaxSet = false;
		}
	}


	bool WSDSetAxisCalMsgPS::XRAILMIN_SET::get()
	{
		return xRailMinSet;
	}

	void WSDSetAxisCalMsgPS::XRAILMIN_SET::set(bool value)
	{
		if (value == true)
		{
			data.AxisCal |= 0x08;
			xRailMinSet = true;
		}
		else
		{
			data.AxisCal &= 0xF7;
			xRailMinSet = false;
		}
	}

	bool WSDSetAxisCalMsgPS::XRAILMAX_SET::get()
	{
		return xRailMaxSet;
	}

	void WSDSetAxisCalMsgPS::XRAILMAX_SET::set(bool value)
	{
		if (value == true)
		{
			data.AxisCal |= 0x10;
			xRailMaxSet = true;
		}
		else
		{
			data.AxisCal &= 0xEF;
			xRailMaxSet = false;
		}
	}

	bool WSDSetAxisCalMsgPS::PIN_SET::get()
	{
		return pinSet;
	}

	void WSDSetAxisCalMsgPS::PIN_SET::set(bool value)
	{
		if (value == true)
		{
			data.AxisCal |= 0x80;
			pinSet = true;
		}
		else
		{
			data.AxisCal &= 0x7F;
			pinSet = false;
		}
	}

	AxisCtlProtoStuct WSDSetAxisCalMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 1;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.AxisCal;

		return frame;
	}

	/*************************  OTC XYZ Messages  *************************/

	/***********************************************/
	//  1. XYZ Set Sync Move Parameters (0x123)       /
	/***********************************************/
	XYZSyncMoveParaPS::XYZSyncMoveParaPS()
	{
		data.id = 0x123;
	}

	XYZSyncMoveParaPS::XYZSyncMoveParaPS(AxisCtlProtoStuct frame)
	{
		data.id = frame.id;
		data.SyncMoveDelayM = frame.frameData[0];
		data.SyncMoveDelayL = frame.frameData[1];
		data.TBLMoveDirection = frame.frameData[2];

		syncMoveDelay = data.SyncMoveDelay;
		if (data.TBLMoveDirection == 0)
		{
			tblMoveDirection = TBLMoveDict::LTR;
		}
		else
		{
			tblMoveDirection = TBLMoveDict::RTL;
		}

	}

	UINT16 XYZSyncMoveParaPS::SYNC_DELAY::get()
	{
		return syncMoveDelay;
	}

	void XYZSyncMoveParaPS::SYNC_DELAY::set(UINT16 value)
	{
		data.SyncMoveDelay = value;
		syncMoveDelay = value;
	}

	TBLMoveDict XYZSyncMoveParaPS::TBLMOVE_DICT::get()
	{
		return tblMoveDirection;
	}

	void XYZSyncMoveParaPS::TBLMOVE_DICT::set(TBLMoveDict value)
	{
		tblMoveDirection = value;
		if (value == TBLMoveDict::LTR)
		{
			data.TBLMoveDirection = 0;
		}
		else
		{
			data.TBLMoveDirection = 1;
		}
	}

	AxisCtlProtoStuct XYZSyncMoveParaPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 3;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.SyncMoveDelayM;
		frame.frameData[1] = data.SyncMoveDelayL;
		frame.frameData[2] = data.TBLMoveDirection;

		return frame;
	}
	/********************************************************************/
	//   2. OTC XYZ Status Message (0x200) /
	/********************************************************************/
	OTCXYZStaMsgPS::OTCXYZStaMsgPS()
	{
		data.id = 0x200;
		resSWStatus.aPressed = resSWStatus.bPressed = resSWStatus.dtSwPressed = resSWStatus.xPressed = resSWStatus.yPressed = resSWStatus.zPressed = resSWStatus.xyzPressed = false;
		xMSStatus.mtEn = xMSStatus.clutchEn = xMSStatus.breakEn = false;
		yMSStatus.mtEn = yMSStatus.clutchEn = yMSStatus.breakEn = false;
		zMSStatus.mtEn = zMSStatus.clutchEn = zMSStatus.breakEn = false;
		opMode = ControlOpMode::NoInit;
		xDtStatus = yDtStatus = zDtStatus = 0;
		estopSWStatus.conAutoPosEn = estopSWStatus.conAutoPosIdOn = estopSWStatus.conEstopEn = estopSWStatus.otcEstopEn = false;
		updataReason.auxSWChange = updataReason.OpModeChange = updataReason.relSWChange = updataReason.scbRequest = updataReason.softDTChange = updataReason.xStaChange = updataReason.yStaChange = updataReason.zStaChange = false;
	}

	MSwStatus OTCXYZStaMsgPS::RES_SW_STA::get()
	{
		return resSWStatus;
	}

	void OTCXYZStaMsgPS::RES_SW_STA::set(MSwStatus value)
	{
		if (value.xPressed == true)
		{
			data.MResSWStatus |= 0x01;
			resSWStatus.xPressed = true;
		}
		else
		{
			data.MResSWStatus &= 0xFE;
			resSWStatus.xPressed = false;
		}

		if (value.yPressed == true){
			data.MResSWStatus |= 0x02;
			resSWStatus.yPressed = true;
		}
		else
		{
			data.MResSWStatus &= 0xFD;
			resSWStatus.yPressed = false;
		}

		if (value.zPressed == true)
		{
			data.MResSWStatus |= 0x04;
			resSWStatus.zPressed = true;
		}
		else
		{
			data.MResSWStatus &= 0xFB;
			resSWStatus.zPressed = false;
		}
		if (value.xyzPressed == true)
		{
			data.MResSWStatus |= 0x08;
			resSWStatus.xyzPressed = true;
		}
		else
		{
			data.MResSWStatus &= 0xF7;
			resSWStatus.xyzPressed = false;
		}
		if (value.aPressed == true)
		{
			data.MResSWStatus |= 0x10;
			resSWStatus.aPressed = true;
		}
		else
		{
			data.MResSWStatus &= 0xEF;
			resSWStatus.aPressed = false;
		}
		if (value.bPressed == true)
		{
			data.MResSWStatus |= 0x20;
			resSWStatus.bPressed = true;
		}
		else
		{
			data.MResSWStatus &= 0xDF;
			resSWStatus.bPressed = false;
		}
		if (value.dtSwPressed == true)
		{
			data.MResSWStatus |= 0x40;
			resSWStatus.dtSwPressed = true;
		}
		else
		{
			data.MResSWStatus &= 0xBF;
			resSWStatus.dtSwPressed = false;
		}
	}


	ControlMSStatus OTCXYZStaMsgPS::X_STATUS::get()
	{
		return xMSStatus;
	}

	void OTCXYZStaMsgPS::X_STATUS::set(ControlMSStatus value)
	{
		if (value.mtEn == true)
		{
			data.XAxisStatus |= 0x01;
			xMSStatus.mtEn = true;
		}
		else
		{
			data.XAxisStatus &= 0xFE;
			xMSStatus.mtEn = false;
		}

		if (value.clutchEn == true)
		{
			data.XAxisStatus |= 0x02;
			xMSStatus.clutchEn = true;
		}
		else
		{
			data.XAxisStatus &= 0xFD;
			xMSStatus.clutchEn = false;
		}

		if (value.breakEn == true)
		{
			data.XAxisStatus |= 0x04;
			xMSStatus.breakEn = true;
		}
		else
		{
			data.XAxisStatus &= 0xFB;
			xMSStatus.breakEn = false;
		}
	}

	ControlMSStatus OTCXYZStaMsgPS::Y_STATUS::get()
	{
		return yMSStatus;
	}

	void OTCXYZStaMsgPS::Y_STATUS::set(ControlMSStatus value)
	{
		if (value.mtEn == true)
		{
			data.YAxisStatus |= 0x01;
			yMSStatus.mtEn = true;
		}
		else
		{
			data.YAxisStatus &= 0xFE;
			yMSStatus.mtEn = false;
		}

		if (value.clutchEn == true)
		{
			data.YAxisStatus |= 0x02;
			yMSStatus.clutchEn = true;
		}
		else
		{
			data.YAxisStatus &= 0xFD;
			yMSStatus.clutchEn = false;
		}

		if (value.breakEn == true)
		{
			data.YAxisStatus |= 0x04;
			yMSStatus.breakEn = true;
		}
		else
		{
			data.YAxisStatus &= 0xFB;
			yMSStatus.breakEn = false;
		}
	}

	ControlMSStatus OTCXYZStaMsgPS::Z_STATUS::get()
	{
		return zMSStatus;
	}

	void OTCXYZStaMsgPS::Z_STATUS::set(ControlMSStatus value)
	{
		if (value.mtEn == true)
		{
			data.ZAxisStatus |= 0x01;
			zMSStatus.mtEn = true;
		}
		else
		{
			data.ZAxisStatus &= 0xFE;
			zMSStatus.mtEn = false;
		}

		if (value.clutchEn == true)
		{
			data.ZAxisStatus |= 0x02;
			zMSStatus.clutchEn = true;
		}
		else
		{
			data.ZAxisStatus &= 0xFD;
			zMSStatus.clutchEn = false;
		}

		if (value.breakEn == true)
		{
			data.ZAxisStatus |= 0x04;
			zMSStatus.breakEn = true;
		}
		else
		{
			data.ZAxisStatus &= 0xFB;
			zMSStatus.breakEn = false;
		}
	}

	ControlOpMode OTCXYZStaMsgPS::OP_MODE::get()
	{
		return opMode;
	}

	void OTCXYZStaMsgPS::OP_MODE::set(ControlOpMode value)
	{
		switch (value)
		{
		case ControlOpMode::NoInit:
			data.OpMode = 0x00;
			opMode = value;
			break;
		case ControlOpMode::Init:
			data.OpMode = 0x01;
			opMode = value;
			break;
		case ControlOpMode::NMtPara:
			data.OpMode = 0x02;
			opMode = value;
			break;
		case ControlOpMode::MLock:
			data.OpMode = 0x03;
			opMode = value;
			break;
		case ControlOpMode::ESTOP:
			data.OpMode = 0x04;
			opMode = value;
			break;
		case ControlOpMode::NRmCal:
			data.OpMode = 0x05;
			opMode = value;
			break;
		case ControlOpMode::SoftDTMod:
			data.OpMode = 0x06;
			opMode = value;
			break;
		case ControlOpMode::MMReq:
			data.OpMode = 0x07;
			opMode = value;
			break;
		case ControlOpMode::AMTP:
			data.OpMode = 0x08;
			opMode = value;
			break;
		case ControlOpMode::TkWSD:
			data.OpMode = 0x0A;
			opMode = value;
			break;
		case ControlOpMode::TkTBL:
			data.OpMode = 0x0B;
			opMode = value;
			break;
		case ControlOpMode::MtNoInit:
			data.OpMode = 0x0C;
			opMode = value;
			break;
		case ControlOpMode::LT:
			data.OpMode = 0x0D;
			opMode = value;
			break;
		case ControlOpMode::AsyCollim:
			data.OpMode = 0x0E;
			opMode = value;
			break;
		case ControlOpMode::Idle:
			data.OpMode = 0x0F;
			opMode = value;
			break;
		case ControlOpMode::AsyTkWSD:
			data.OpMode = 0x10;
			opMode = value;
			break;
		default:
			data.OpMode = 0x00;
			opMode = ControlOpMode::NoInit;
			break;
		}
	}

	UINT OTCXYZStaMsgPS::X_DT_STA::get()
	{
		return xDtStatus;
	}

	void OTCXYZStaMsgPS::X_DT_STA::set(UINT value)
	{
		data.XYSoftDtStatus = (data.XYSoftDtStatus & 0xF0) + value;
		xDtStatus = value;
	}

	UINT OTCXYZStaMsgPS::Y_DT_STA::get()
	{
		return yDtStatus;
	}

	void OTCXYZStaMsgPS::Y_DT_STA::set(UINT value)
	{
		data.XYSoftDtStatus = (data.XYSoftDtStatus & 0x0F) + (value << 4);
		yDtStatus = value;
	}

	UINT OTCXYZStaMsgPS::Z_DT_STA::get()
	{
		return zDtStatus;
	}

	void OTCXYZStaMsgPS::Z_DT_STA::set(UINT value)
	{
		data.ZSoftDtAuxSWStatus = (data.ZSoftDtAuxSWStatus & 0xF0) + value;
		zDtStatus = value;
	}

	EstopSW OTCXYZStaMsgPS::ESTOP_SW::get()
	{
		return estopSWStatus;
	}

	void OTCXYZStaMsgPS::ESTOP_SW::set(EstopSW value)
	{
		if (value.conEstopEn == true)
		{
			data.ZSoftDtAuxSWStatus |= 0x10;
			estopSWStatus.conAutoPosIdOn = true;
		}
		else
		{
			data.ZSoftDtAuxSWStatus &= 0xEF;
			estopSWStatus.conAutoPosIdOn = false;
		}

		if (value.otcEstopEn == true)
		{
			data.ZSoftDtAuxSWStatus |= 0x20;
			estopSWStatus.otcEstopEn = true;
		}
		else
		{
			data.ZSoftDtAuxSWStatus &= 0xDF;
			estopSWStatus.otcEstopEn = false;
		}

		if (value.conAutoPosEn == true)
		{
			data.ZSoftDtAuxSWStatus |= 0x40;
			estopSWStatus.conAutoPosEn = true;
		}
		else
		{
			data.ZSoftDtAuxSWStatus &= 0xBF;
			estopSWStatus.conAutoPosEn = false;
		}

		if (value.conEstopEn == true)
		{
			data.ZSoftDtAuxSWStatus |= 0x80;
			estopSWStatus.conEstopEn = true;
		}
		else
		{
			data.ZSoftDtAuxSWStatus &= 0x7F;
			estopSWStatus.conEstopEn = false;
		}
	}

	OTCXYZStaMsgUpdateReason OTCXYZStaMsgPS::UPDATE_REASON::get()
	{
		return updataReason;
	}

	void OTCXYZStaMsgPS::UPDATE_REASON::set(OTCXYZStaMsgUpdateReason value)
	{
		if (value.relSWChange == true)
		{
			data.UpdateReason |= 0x01;
			updataReason.relSWChange = true;
		}
		else
		{
			data.UpdateReason &= 0xFE;
			updataReason.relSWChange = false;
		}

		if (value.xStaChange == true)
		{
			data.UpdateReason |= 0x02;
			updataReason.xStaChange = true;
		}
		else
		{
			data.UpdateReason &= 0xFD;
			updataReason.xStaChange = false;
		}

		if (value.yStaChange == true)
		{
			data.UpdateReason |= 0x04;
			updataReason.yStaChange = true;
		}
		else
		{
			data.UpdateReason &= 0xFB;
			updataReason.yStaChange = false;
		}

		if (value.zStaChange == true)
		{
			data.UpdateReason |= 0x08;
			updataReason.zStaChange = true;
		}
		else
		{
			data.UpdateReason &= 0xF7;
			updataReason.zStaChange = false;
		}

		if (value.OpModeChange == true)
		{
			data.UpdateReason |= 0x10;
			updataReason.OpModeChange = true;
		}
		else
		{
			data.UpdateReason &= 0xEF;
			updataReason.OpModeChange = false;
		}

		if (value.auxSWChange == true)
		{
			data.UpdateReason |= 0x20;
			updataReason.auxSWChange = true;
		}
		else
		{
			data.UpdateReason &= 0xDF;
			updataReason.auxSWChange = false;
		}

		if (value.softDTChange == true)
		{
			data.UpdateReason |= 0x40;
			updataReason.softDTChange = true;
		}
		else
		{
			data.UpdateReason &= 0xBF;
			updataReason.softDTChange = false;
		}

		if (value.scbRequest == true)
		{
			data.UpdateReason |= 0x80;
			updataReason.scbRequest = true;
		}
		else
		{
			data.UpdateReason &= 0x7F;
			updataReason.scbRequest = false;
		}
	}


	AxisCtlProtoStuct OTCXYZStaMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 8;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.MResSWStatus;
		frame.frameData[1] = data.XAxisStatus;
		frame.frameData[2] = data.YAxisStatus;
		frame.frameData[3] = data.ZAxisStatus;
		frame.frameData[4] = data.OpMode;
		frame.frameData[5] = data.XYSoftDtStatus;
		frame.frameData[6] = data.ZSoftDtAuxSWStatus;
		frame.frameData[7] = data.UpdateReason;
		
		return frame;
	}


	/********************************************************************/
	//   3. IR Remote Status Message (0x260) /
	/********************************************************************/
	OTCIRStaMsgPS::OTCIRStaMsgPS()
	{
		data.id = 0x260;
		simpleMSStatus.hSDownSW = simpleMSStatus.hSLeftSW = simpleMSStatus.hSRightSW = simpleMSStatus.hSUpSW = simpleMSStatus.lSDownSW = simpleMSStatus.lSLeftSW = simpleMSStatus.lSRightSW = simpleMSStatus.lSUpSW = false;
		tiltSWStatus.tiltDownSW = tiltSWStatus.tiltUpSW = false;
		complexMSStatus.autoTrackSW = complexMSStatus.autoPosSW = complexMSStatus.autoCenterSW = complexMSStatus.autoCenter2SW = false;
		colPSSWStatus.pre1SW = colPSSWStatus.pre2SW = colPSSWStatus.pre3SW = false;
		colBPSWStatus.vertCloseSW = colBPSWStatus.horiOpenSW = false;
		LampSWStatus = false;
		updateReason.colBSStaChange = updateReason.colPSStaChange = updateReason.comMSStaChange = updateReason.lampStaChange = updateReason.scbRequest = updateReason.simMSStaChange = updateReason.tiltStaChange = false;

	}

	IRSimpleMSStatus OTCIRStaMsgPS::SIM_MS_STA::get()
	{
		return simpleMSStatus;
	}

	void OTCIRStaMsgPS::SIM_MS_STA::set(IRSimpleMSStatus value)
	{
		if (value.hSUpSW == true)
		{
			data.SimpleMSStatus |= 0x01;
			simpleMSStatus.hSUpSW = true;
		}
		else
		{
			data.SimpleMSStatus &= 0xFE;
			simpleMSStatus.hSUpSW = false;
		}

		if (value.lSUpSW == true)
		{
			data.SimpleMSStatus |= 0x02;
			simpleMSStatus.lSUpSW = true;
		}
		else
		{
			data.SimpleMSStatus &= 0xFD;
			simpleMSStatus.lSUpSW = false;
		}

		if (value.hSDownSW == true)
		{
			data.SimpleMSStatus |= 0x04;
			simpleMSStatus.hSDownSW = true;
		}
		else
		{
			data.SimpleMSStatus &= 0xFB;
			simpleMSStatus.hSDownSW = false;
		}
		if (value.lSDownSW == true)
		{
			data.SimpleMSStatus |= 0x08;
			simpleMSStatus.lSDownSW = true;
		}
		else
		{
			data.SimpleMSStatus &= 0xF7;
			simpleMSStatus.lSDownSW = false;
		}
		if (value.hSLeftSW == true)
		{
			data.SimpleMSStatus |= 0x10;
			simpleMSStatus.hSLeftSW = true;
		}
		else
		{
			data.SimpleMSStatus &= 0xEF;
			simpleMSStatus.hSLeftSW = false;
		}
		if (value.lSLeftSW == true)
		{
			data.SimpleMSStatus |= 0x20;
			simpleMSStatus.lSLeftSW = true;
		}
		else
		{
			data.SimpleMSStatus &= 0xDF;
			simpleMSStatus.lSLeftSW = false;
		}
		if (value.hSRightSW == true)
		{
			data.SimpleMSStatus |= 0x40;
			simpleMSStatus.hSRightSW = true;
		}
		else
		{
			data.SimpleMSStatus &= 0xBF;
			simpleMSStatus.hSRightSW = false;
		}
		if (value.lSRightSW == true)
		{
			data.SimpleMSStatus |= 0x80;
			simpleMSStatus.lSRightSW = true;
		}
		else
		{
			data.SimpleMSStatus &= 0x7F;
			simpleMSStatus.lSRightSW = false;
		}
	
	}

	IRTiltSWStatus OTCIRStaMsgPS::TILT_SW_STA::get()
	{
		return tiltSWStatus;
	}

	void OTCIRStaMsgPS::TILT_SW_STA::set(IRTiltSWStatus value)
	{
		if (value.tiltUpSW == true)
		{
			data.TiltSWStatus |= 0x01;
			tiltSWStatus.tiltUpSW = true;
		}
		else
		{
			data.TiltSWStatus &= 0xFE;
			tiltSWStatus.tiltUpSW = false;
		}

		if (value.tiltDownSW == true)
		{
			data.TiltSWStatus |= 0x02;
			tiltSWStatus.tiltDownSW = true;
		}
		else
		{
			data.TiltSWStatus &= 0xFD;
			tiltSWStatus.tiltDownSW = false;
		}
	}

	IRComplexMSStatus OTCIRStaMsgPS::COM_MS_STA::get()
	{
		return complexMSStatus;
	}

	void OTCIRStaMsgPS::COM_MS_STA::set(IRComplexMSStatus value)
	{
		if (value.autoCenterSW == true)
		{
			data.ComplexMSStatus |= 0x01;
			complexMSStatus.autoCenterSW = true;
		}
		else
		{
			data.ComplexMSStatus &= 0xFE;
			complexMSStatus.autoCenterSW = false;
		}

		if (value.autoTrackSW == true)
		{
			data.ComplexMSStatus |= 0x02;
			complexMSStatus.autoTrackSW = true;
		}
		else
		{
			data.ComplexMSStatus &= 0xFD;
			complexMSStatus.autoTrackSW = false;
		}

		if (value.autoPosSW == true)
		{
			data.ComplexMSStatus |= 0x04;
			complexMSStatus.autoPosSW = true;
		}
		else
		{
			data.ComplexMSStatus &= 0xFB;
			complexMSStatus.autoPosSW = false;
		}
		if (value.autoCenter2SW == true)
		{
			data.ComplexMSStatus |= 0x08;
			complexMSStatus.autoCenter2SW = true;
		}
		else
		{
			data.ComplexMSStatus &= 0xF7;
			complexMSStatus.autoCenter2SW = false;
		}
		
	}

	IRColPSSWStatus OTCIRStaMsgPS::COL_PS_STA::get()
	{
		return colPSSWStatus;
	}

	void OTCIRStaMsgPS::COL_PS_STA::set(IRColPSSWStatus value)
	{
		if (value.pre1SW == true)
		{
			data.ColPSSWStatus |= 0x01;
			colPSSWStatus.pre1SW = true;
		}
		else
		{
			data.ColPSSWStatus &= 0xFE;
			colPSSWStatus.pre1SW = false;
		}

		if (value.pre2SW == true)
		{
			data.ColPSSWStatus |= 0x02;
			colPSSWStatus.pre2SW = true;
		}
		else
		{
			data.ColPSSWStatus &= 0xFD;
			colPSSWStatus.pre2SW = false;
		}

		if (value.pre3SW == true)
		{
			data.ColPSSWStatus |= 0x04;
			colPSSWStatus.pre3SW = true;
		}
		else
		{
			data.ColPSSWStatus &= 0xFB;
			colPSSWStatus.pre3SW = false;
		}
	}

	IRColBPSWStatus OTCIRStaMsgPS::COL_BP_STA::get()
	{
		return colBPSWStatus;
	}

	void OTCIRStaMsgPS::COL_BP_STA::set(IRColBPSWStatus value)
	{
		if (value.horiOpenSW == true)
		{
			data.ColBPSWStatus |= 0x01;
			colBPSWStatus.horiOpenSW = true;
		}
		else
		{
			data.ColBPSWStatus &= 0xFE;
			colBPSWStatus.horiOpenSW = false;
		}

		if (value.horiCloseSW == true)
		{
			data.ColBPSWStatus |= 0x02;
			colBPSWStatus.horiCloseSW = true;
		}
		else
		{
			data.ColBPSWStatus &= 0xFD;
			colBPSWStatus.horiCloseSW = false;
		}

		if (value.vertOpenSW == true)
		{
			data.ColBPSWStatus |= 0x04;
			colBPSWStatus.vertOpenSW = true;
		}
		else
		{
			data.ColBPSWStatus &= 0xFB;
			colBPSWStatus.vertOpenSW = false;
		}
		if (value.vertCloseSW == true)
		{
			data.ColBPSWStatus |= 0x08;
			colBPSWStatus.vertCloseSW = true;
		}
		else
		{
			data.ColBPSWStatus &= 0xF7;
			colBPSWStatus.vertCloseSW = false;
		}
	}

	bool OTCIRStaMsgPS::LAMP_STA::get()
	{
		return LampSWStatus;
	}

	void OTCIRStaMsgPS::LAMP_STA::set(bool value)
	{
		if (value == true)
		{
			data.ColLPSWStatus |= 0x01;
			LampSWStatus = true;
		}
		else
		{
			data.ColLPSWStatus &= 0xFE;
			LampSWStatus = false;
		}
	}

	IRStaUpdateReason OTCIRStaMsgPS::UPDATE_REASON::get()
	{
		return updateReason;
	}

	void OTCIRStaMsgPS::UPDATE_REASON::set(IRStaUpdateReason value)
	{
		if (value.simMSStaChange == true)
		{
			data.UpdataReason |= 0x01;
			updateReason.simMSStaChange = true;
		}
		else
		{
			data.UpdataReason &= 0xFE;
			updateReason.simMSStaChange = false;
		}

		if (value.tiltStaChange == true)
		{
			data.UpdataReason |= 0x02;
			updateReason.tiltStaChange = true;
		}
		else
		{
			data.UpdataReason &= 0xFD;
			updateReason.tiltStaChange = false;
		}
		if (value.comMSStaChange == true)
		{
			data.UpdataReason |= 0x04;
			updateReason.comMSStaChange = true;
		}
		else
		{
			data.UpdataReason &= 0xFB;
			updateReason.comMSStaChange = false;
		}
		if (value.colPSStaChange == true)
		{
			data.UpdataReason |= 0x08;
			updateReason.colPSStaChange = true;
		}
		else
		{
			data.UpdataReason &= 0xF7;
			updateReason.colPSStaChange = false;
		}
		if (value.colBSStaChange == true)
		{
			data.UpdataReason |= 0x10;
			updateReason.colBSStaChange = true;
		}
		else
		{
			data.UpdataReason &= 0xEF;
			updateReason.colBSStaChange = false;
		}
		if (value.lampStaChange == true)
		{
			data.UpdataReason |= 0x20;
			updateReason.lampStaChange = true;
		}
		else
		{
			data.UpdataReason &= 0xDF;
			updateReason.lampStaChange = false;
		}
		if (value.scbRequest == true)
		{
			data.UpdataReason |= 0x40;
			updateReason.scbRequest = true;
		}
		else
		{
			data.UpdataReason &= 0xBF;
			updateReason.scbRequest = false;
		}
		
	}

	AxisCtlProtoStuct OTCIRStaMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 7;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.SimpleMSStatus;
		frame.frameData[1] = data.TiltSWStatus;
		frame.frameData[2] = data.ComplexMSStatus;
		frame.frameData[3] = data.ColPSSWStatus;
		frame.frameData[4] = data.ColBPSWStatus;
		frame.frameData[5] = data.ColLPSWStatus;
		frame.frameData[6] = data.UpdataReason;

		return frame;
	}



	/********************************************************************/
	//   4. OTC XYZ Heartbeat Message (0x300) /
	/********************************************************************/
	OTCXYZHBMsgPS::OTCXYZHBMsgPS()
	{
		data.id = 0x300;
		data.Xpos = data.Ypos = data.Zpos = 0x8000;
		xPositon = yPosition = zPosition = 0;
		XMaxSet = XMinSet = YMinSet = YMaxSet = ZMaxSet = ZMinSet = false;
		RmCalDone = PinCalDone = false;
		updateReason.axisMove = updateReason.CalStaChange = updateReason.hbTimeOut = updateReason.scbRequest = false;
	}
	
	double OTCXYZHBMsgPS::X_POSITION::get()
	{
		return xPositon;
	}

	void OTCXYZHBMsgPS::X_POSITION::set(double value)
	{
		WORD tempValue = (WORD)(value * 4);
		data.Xpos = tempValue;
		xPositon = value;
	}

	double OTCXYZHBMsgPS::Y_POSITION::get()
	{
		return yPosition;
	}

	void OTCXYZHBMsgPS::Y_POSITION::set(double value)
	{
		WORD tempValue = (WORD)(value * 4);
		data.Ypos = tempValue;
		yPosition = value;
	}

	double OTCXYZHBMsgPS::Z_POSITION::get()
	{
		return zPosition;
	}

	void OTCXYZHBMsgPS::Z_POSITION::set(double value)
	{
		WORD tempValue = (WORD)(value * 4);
		data.Zpos = tempValue;
		zPosition = value;
	}

	bool OTCXYZHBMsgPS::ZMIN_SET::get()
	{
		return ZMinSet;
	}

	void OTCXYZHBMsgPS::ZMIN_SET::set(bool value)
	{
		if (value == true)
		{
			data.AxisCalStatus |= 0x01;
			ZMinSet = true;
		}
		else
		{
			data.AxisCalStatus &= 0xFE;
			ZMinSet = false;
		}
	}

	bool OTCXYZHBMsgPS::XMIN_SET::get()
	{
		return XMinSet;
	}

	void OTCXYZHBMsgPS::XMIN_SET::set(bool value)
	{
		if (value == true)
		{
			data.AxisCalStatus |= 0x02;
			XMinSet = true;
		}
		else
		{
			data.AxisCalStatus &= 0XFD;
			XMinSet = false;
		}
	}

	bool OTCXYZHBMsgPS::YMIN_SET::get()
	{
		return YMinSet;
	}

	void OTCXYZHBMsgPS::YMIN_SET::set(bool value)
	{
		if (value == true)
		{
			data.AxisCalStatus |= 0x04;
			YMinSet = true;
		}
		else
		{
			data.AxisCalStatus &= 0xFB;
			YMinSet = false;
		}
	}

	bool OTCXYZHBMsgPS::ZMAX_SET::get()
	{
		return ZMaxSet;
	}

	void OTCXYZHBMsgPS::ZMAX_SET::set(bool value)
	{
		if (value == true)
		{
			data.AxisCalStatus |= 0x08;
			ZMaxSet = true;
		}
		else
		{
			data.AxisCalStatus &= 0xF7;
			ZMaxSet = false;
		}
	}

	bool OTCXYZHBMsgPS::XMAX_SET::get()
	{
		return XMaxSet;
	}

	void OTCXYZHBMsgPS::XMAX_SET::set(bool value)
	{
		if (value == true)
		{
			data.AxisCalStatus |= 0x10;
			XMaxSet = true;
		}
		else
		{
			data.AxisCalStatus &= 0xEF;
			XMaxSet = false;
		}
	}

	bool OTCXYZHBMsgPS::YMAX_SET::get()
	{
		return YMaxSet;
	}

	void OTCXYZHBMsgPS::YMAX_SET::set(bool value)
	{
		if (value == true)
		{
			data.AxisCalStatus |= 0x20;
			YMaxSet = true;
		}
		else
		{
			data.AxisCalStatus &= 0xDF;
			YMaxSet = false;
		}
	}

	bool OTCXYZHBMsgPS::RMCAL_DONE::get()
	{
		return RmCalDone;
	}

	void OTCXYZHBMsgPS::RMCAL_DONE::set(bool value)
	{
		if (value == true)
		{
			data.AxisCalStatus |= 0x40;
			RmCalDone = true;
		}
		else
		{
			data.AxisCalStatus &= 0xBF;
			RmCalDone = false;
		}
	}

	bool OTCXYZHBMsgPS::PINCAL_DONE::get()
	{
		return PinCalDone;
	}

	void OTCXYZHBMsgPS::PINCAL_DONE::set(bool value)
	{
		if (value == true)
		{
			data.AxisCalStatus |= 0x80;
			PinCalDone = true;
		}
		else
		{
			data.AxisCalStatus &= 0x7F;
			PinCalDone = false;
		}
	}

	OTCXYZHBUpdateReason OTCXYZHBMsgPS::UPDATE_REASON::get()
	{
		return updateReason;
	}

	void OTCXYZHBMsgPS::UPDATE_REASON::set(OTCXYZHBUpdateReason value)
	{
		if (value.hbTimeOut == true)
		{
			data.UpdataReason |= 0x01;
			updateReason.hbTimeOut = true;
		}
		else
		{
			data.UpdataReason &= 0xFE;
			updateReason.hbTimeOut = false;
		}
		if (value.scbRequest == true)
		{
			data.UpdataReason |= 0x02;
			updateReason.scbRequest = true;
		}
		else
		{
			data.UpdataReason &= 0xFD;
			updateReason.scbRequest = false;
		}
		if (value.axisMove == true)
		{
			data.UpdataReason |= 0x04;
			updateReason.axisMove = true;
		}
		else
		{
			data.UpdataReason &= 0xFB;
			updateReason.axisMove = false;
		}
		if (value.CalStaChange == true)
		{
			data.UpdataReason |= 0x08;
			updateReason.CalStaChange = true;
		}
		else
		{
			data.UpdataReason &= 0xF7;
			updateReason.CalStaChange = false;
		}
	}

	AxisCtlProtoStuct OTCXYZHBMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 8;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.XposM;
		frame.frameData[1] = data.XPosL;
		frame.frameData[2] = data.YPosM;
		frame.frameData[3] = data.YPosL;
		frame.frameData[4] = data.ZPosM;
		frame.frameData[5] = data.ZPosL;
		frame.frameData[6] = data.AxisCalStatus;
		frame.frameData[7] = data.UpdataReason;

		return frame;
	}



	/**************************************************/
	//   5. OTC XYZ LLI Status Message (0x305)        //
	/**************************************************/
	// OTC XYZ LLI Status Message (0x305) message
	OTCLLIStatusMsgPS::OTCLLIStatusMsgPS()
	{
		data.id = 0x305;
		xyTravStart = xyTravEnd = zTravStart = zTravEnd = 0;
	}

	double OTCLLIStatusMsgPS::XYTRAV_END::get()
	{
		return xyTravEnd;
	}

	void OTCLLIStatusMsgPS::XYTRAV_END::set(double value)
	{
		WORD tempValue = (WORD)(value * 4);
		data.XYTravEnd = tempValue;
		xyTravEnd = value;
	}

	double OTCLLIStatusMsgPS::XYTRAV_START::get()
	{
		return xyTravStart;
	}

	void OTCLLIStatusMsgPS::XYTRAV_START::set(double value)
	{
		WORD tempValue = (WORD)(value * 4);
		data.XYTravStart = tempValue;
		xyTravStart = value;
	}

	double OTCLLIStatusMsgPS::ZTRAV_END::get()
	{
		return zTravEnd;
	}

	void OTCLLIStatusMsgPS::ZTRAV_END::set(double value)
	{
		WORD tempValue = (WORD)(value * 4);
		data.ZTravEnd = tempValue;
		zTravEnd = value;
	}

	double OTCLLIStatusMsgPS::ZTRAV_START::get()
	{
		return zTravStart;
	}

	void OTCLLIStatusMsgPS::ZTRAV_START::set(double value)
	{
		WORD tempValue = (WORD)(value * 4);
		data.ZTravStart = tempValue;
		zTravStart = value;
	}

	AxisCtlProtoStuct OTCLLIStatusMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 8;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.XYTravEndM;
		frame.frameData[1] = data.XYTravEndL;
		frame.frameData[2] = data.XYTravStartM;
		frame.frameData[3] = data.XYTravStartL;
		frame.frameData[4] = data.ZTravEndM;
		frame.frameData[5] = data.ZTravEndL;
		frame.frameData[6] = data.ZTravStartM;
		frame.frameData[7] = data.ZTravStartL;

		return frame;
	}


	/*************************  OTC AB Messages  *************************/
	/********************************************************************/
	//   1. OTC AB Status Message (0x210) /
	/********************************************************************/
	ABStatusMsgPS::ABStatusMsgPS()
	{
		data.id = 0x210;
		aStatus.brakeEn = aStatus.hmSensor = aStatus.inDetent = aStatus.mtRun = false;
		bStatus.dtEn = bStatus.dtSoEn = bStatus.mtRun = false;
		opMode = ControlOpMode::NoInit;
		updateReason.aStatusChange = updateReason.bStatusChange = updateReason.opModeChange = updateReason.scbRequest = false;
	}

	AStatus ABStatusMsgPS::A_STA::get()
	{
		return aStatus;
	}

	void ABStatusMsgPS::A_STA::set(AStatus value)
	{
		if (value.mtRun == true)
		{
			data.AStatus |= 0x01;
			aStatus.mtRun = true;
		}
		else
		{
			data.AStatus &= 0xFE;
			aStatus.mtRun = false;
		}

		if (value.brakeEn == true)
		{
			data.AStatus |= 0x02;
			aStatus.brakeEn = true;
		}
		else
		{
			data.AStatus &= 0xFD;
			aStatus.brakeEn = false;
		}
		if (value.hmSensor == true)
		{
			data.AStatus |= 0x04;
			aStatus.hmSensor = true;
		}
		else
		{
			data.AStatus &= 0xFB;
			aStatus.hmSensor = false;
		}
		if (value.inDetent == true)
		{
			data.AStatus |= 0x08;
			aStatus.inDetent = true;
		}
		else
		{
			data.AStatus &= 0xF7;
			aStatus.inDetent = false;
		}
	}

	BStatus ABStatusMsgPS::B_STA::get()
	{
		return bStatus;
	}

	void ABStatusMsgPS::B_STA::set(BStatus value)
	{
		if (value.mtRun == true)
		{
			data.BStatus |= 0x01;
			bStatus.mtRun = true;
		}
		else
		{
			data.BStatus &= 0xFE;
			bStatus.mtRun = false;
		}
		if (value.dtSoEn == true)
		{
			data.BStatus |= 0x02;
			bStatus.dtSoEn = true;
		}
		else
		{
			data.BStatus &= 0xFD;
			bStatus.dtSoEn = false;
		}
		if (value.dtEn == true)
		{
			data.BStatus |= 0x04;
			bStatus.dtEn = true;
		}
		else
		{
			data.BStatus &= 0xFB;
			bStatus.dtEn = false;
		}
	}

	ControlOpMode ABStatusMsgPS::OP_MODE::get()
	{
		return opMode;
	}

	void ABStatusMsgPS::OP_MODE::set(ControlOpMode value)
	{
		switch (value)
		{
		case ControlOpMode::NoInit:
			data.OpMode = 0x00;
			opMode = value;
			break;
		case ControlOpMode::Init:
			data.OpMode = 0x01;
			opMode = value;
			break;
		case ControlOpMode::NMtPara:
			data.OpMode = 0x02;
			opMode = value;
			break;
		case ControlOpMode::MLock:
			data.OpMode = 0x03;
			opMode = value;
			break;
		case ControlOpMode::ESTOP:
			data.OpMode = 0x04;
			opMode = value;
			break;
		case ControlOpMode::NRmCal:
			data.OpMode = 0x05;
			opMode = value;
			break;
		case ControlOpMode::SoftDTMod:
			data.OpMode = 0x06;
			opMode = value;
			break;
		case ControlOpMode::MMReq:
			data.OpMode = 0x07;
			opMode = value;
			break;
		case ControlOpMode::AMTP:
			data.OpMode = 0x08;
			opMode = value;
			break;
		case ControlOpMode::TkWSD:
			data.OpMode = 0x0A;
			opMode = value;
			break;
		case ControlOpMode::TkTBL:
			data.OpMode = 0x0B;
			opMode = value;
			break;
		case ControlOpMode::MtNoInit:
			data.OpMode = 0x0C;
			opMode = value;
			break;
		case ControlOpMode::LT:
			data.OpMode = 0x0D;
			opMode = value;
			break;
		case ControlOpMode::Idle:
			data.OpMode = 0x0F;
			opMode = value;
			break;
		default:
			data.OpMode = 0x00;
			opMode = ControlOpMode::NoInit;
			break;
		}
	}

	ABStaUpdateReason ABStatusMsgPS::UPDATE_REASON::get()
	{
		return updateReason;
	}

	void ABStatusMsgPS::UPDATE_REASON::set(ABStaUpdateReason value)
	{
		if (value.aStatusChange == true)
		{
			data.UpdataReason |= 0x01;
			updateReason.aStatusChange = true;
		}
		else
		{
			data.UpdataReason &= 0xFE;
			updateReason.bStatusChange = false;
		}
		if (value.bStatusChange == true)
		{
			data.UpdataReason |= 0x02;
			updateReason.bStatusChange = true;
		}
		else
		{
			data.UpdataReason &= 0xFD;
			updateReason.bStatusChange = false;
		}
		if (value.opModeChange == true)
		{
			data.UpdataReason |= 0x04;
			updateReason.opModeChange = true;
		}
		else
		{
			data.UpdataReason &= 0xFB;
			updateReason.opModeChange = false;
		}
		if (value.scbRequest == true)
		{
			data.UpdataReason |= 0x08;
			updateReason.scbRequest = true;
		}
		else
		{
			data.UpdataReason &= 0xF7;
			updateReason.scbRequest = false;
		}
	}

	AxisCtlProtoStuct ABStatusMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 4;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.AStatus;
		frame.frameData[1] = data.BStatus;
		frame.frameData[2] = data.OpMode;
		frame.frameData[3] = data.UpdataReason;

		return frame;
 	}


	/********************************************************************/
	//   2. OTC AB HeartBeat Message (0x310) /
	/********************************************************************/
	OTCABHBMsgPS::OTCABHBMsgPS()
	{
		data.id = 0x310;
		data.APos = 0x8000;
		data.BPos = 0x8000;
		aPosition = bPosition = 0;
		calStatus.AMinSet = calStatus.BMinSet = calStatus.AMaxSet = calStatus.BMaxSet= calStatus.PinCalSet = false;
		updateReason.axisMove = updateReason.CalStaChange = updateReason.hbTimeOut = updateReason.scbRequest = false;
	}

	double OTCABHBMsgPS::A_POSITION::get()
	{
		return aPosition;
	}

	void OTCABHBMsgPS::A_POSITION::set(double value)
	{
		data.APos = (WORD)(value * 10);
		aPosition = value;
	}

	double OTCABHBMsgPS::B_POSITION::get()
	{
		return bPosition;
	}

	void OTCABHBMsgPS::B_POSITION::set(double value)
	{
		data.BPos = (WORD)(value * 10);
		bPosition = value;
	}

	OTCABCalStatus OTCABHBMsgPS::CAL_STA::get()
	{
		return calStatus;
	}

	void OTCABHBMsgPS::CAL_STA::set(OTCABCalStatus value)
	{
		if (value.AMinSet == true)
		{
			data.AxisCalStatus |= 0x01;
			calStatus.AMinSet = true;
		}
		else
		{
			data.AxisCalStatus &= 0xFE;
			calStatus.AMinSet = false;
		}

		if (value.BMinSet == true)
		{
			data.AxisCalStatus |= 0x02;
			calStatus.BMinSet = true;
		}
		else
		{
			data.AxisCalStatus &= 0xFD;
			calStatus.BMinSet = false;
		}
		if (value.AMaxSet == true)
		{
			data.AxisCalStatus |= 0x04;
			calStatus.AMaxSet = true;
		}
		else
		{
			data.AxisCalStatus &= 0xFB;
			calStatus.AMaxSet = false;
		}
		if (value.BMaxSet == true)
		{
			data.AxisCalStatus |= 0x08;
			calStatus.BMaxSet = true;
		}
		else
		{
			data.AxisCalStatus &= 0xF7;
			calStatus.BMaxSet = false;
		}
		if (value.PinCalSet == true)
		{
			data.AxisCalStatus |= 0x80;
			calStatus.PinCalSet = true;
		}
		else
		{
			data.AxisCalStatus &= 0x7F;
			calStatus.PinCalSet = false;
		}
	}

	OTCABHBUpdateReason OTCABHBMsgPS::UPDATE_REASON::get()
	{
		return updateReason;
	}

	
	void OTCABHBMsgPS::UPDATE_REASON::set(OTCABHBUpdateReason value)
	{
		if (value.hbTimeOut == true)
		{
			data.UpdataReason |= 0x01;
			updateReason.hbTimeOut = true;
		}
		else
		{
			data.UpdataReason &= 0xFE;
			updateReason.hbTimeOut = false;
		}

		if (value.scbRequest == true)
		{
			data.UpdataReason |= 0x02;
			updateReason.scbRequest = true;
		}
		else
		{
			data.UpdataReason &= 0xFD;
			updateReason.scbRequest = false;
		}
		if (value.axisMove == true)
		{
			data.UpdataReason |= 0x04;
			updateReason.axisMove = true;
		}
		else
		{
			data.UpdataReason &= 0xFB;
			updateReason.axisMove = false;
		}
		if (value.CalStaChange == true)
		{
			data.UpdataReason |= 0x08;
			updateReason.CalStaChange = true;
		}
		else
		{
			data.UpdataReason &= 0xF7;
			updateReason.CalStaChange = false;
		}
	}

	AxisCtlProtoStuct OTCABHBMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 6;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.APosM;
		frame.frameData[1] = data.APosL;
		frame.frameData[2] = data.BPosM;
		frame.frameData[3] = data.BPosL;
		frame.frameData[4] = data.AxisCalStatus;
		frame.frameData[5] = data.UpdataReason;

		return frame;
	}

	/*************************  TBL Messages  *************************/
	/********************************************************************/
	//   1. TBL Sync Move Start (0x120) /
	/********************************************************************/
	TBLSyncMoveStartPS::TBLSyncMoveStartPS()
	{
		data.id = 0x120;
	}

	TBLSyncMoveStartPS::TBLSyncMoveStartPS(AxisCtlProtoStuct frame)
	{
		data.id = frame.id;
		data.AtSpeedDelayM = frame.frameData[0];
		data.AtSpeedDelayL = frame.frameData[1];

		atSpeedDelay = data.AtSpeedDelay;
	}

	UInt16 TBLSyncMoveStartPS::AT_SPEED_DELAY::get()
	{
		return atSpeedDelay;
	}

	void TBLSyncMoveStartPS::AT_SPEED_DELAY::set(UInt16 value)
	{
		data.AtSpeedDelay = value;
		atSpeedDelay = value;
	}

	AxisCtlProtoStuct TBLSyncMoveStartPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 2;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.AtSpeedDelayM;
		frame.frameData[1] = data.AtSpeedDelayL;
		return frame;
	}
	/********************************************************************/
	//   2. TBL Status Message (0x230) /
	/********************************************************************/
	
	//ctor
	TBLStatusMsgPS::TBLStatusMsgPS()
	{
		data.id = 0x230;
	}

	TBLSwStatus TBLStatusMsgPS::SW_STATUS::get()
	{
		return swStatus;
	}

	void TBLStatusMsgPS::SW_STATUS::set(TBLSwStatus value)
	{
		if (value.autoCenterSW)
		{
			data.SwStatus |= 0x01;
		}
		else
		{
			data.SwStatus &= 0xFE;
		}
		if (value.autoTrackSW)
		{
			data.SwStatus |= 0x02;
		}
		else
		{
			data.SwStatus &= 0xFD;
		}
		if (value.estopSW)
		{
			data.SwStatus |= 0x04;
		}
		else
		{
			data.SwStatus &= 0xFB;
		}
		if (value.zUpPedal)
		{
			data.SwStatus |= 0x08;
		}
		else
		{
			data.SwStatus &= 0xF7;
		}
		if (value.zDownPedal)
		{
			data.SwStatus |= 0x10;
		}
		else
		{
			data.SwStatus &= 0xEF;
		}
		if (value.floatPedal)
		{
			data.SwStatus |= 0x20;
		}
		else
		{
			data.SwStatus &= 0xDF;
		}
		swStatus = value;
	}

	TBLMtControls TBLStatusMsgPS::MTCONTROLS::get()
	{
		return mtControls;
	}

	void TBLStatusMsgPS::MTCONTROLS::set(TBLMtControls value)
	{
		if (value.longMotorRun)
		{
			data.MtControl |= 0x01;
		}
		else
		{
			data.MtControl &= 0xFE;
		}
		if (value.zMotorRun)
		{
			data.MtControl |= 0x04;
		}
		else
		{
			data.MtControl &= 0xFB;
		}
		if (value.floatSolenAssert)
		{
			data.MtControl |= 0x08;
		}
		else
		{
			data.MtControl &= 0xF7;
		}

		mtControls = value;
	}

	TBLMtSensors TBLStatusMsgPS::MTSENSORS::get()
	{
		return mtSensors;
	}

	void TBLStatusMsgPS::MTSENSORS::set(TBLMtSensors value)
	{
		if (value.longBuckySensor)
		{
			data.MtSensor |= 0x01;
		}
		else
		{
			data.MtSensor &= 0xFE;
		}
		if (value.gridPresent)
		{
			data.MtSensor |= 0x02;
		}
		else
		{
			data.MtSensor &= 0xFD;
		}
		if (value.dtIntable)
		{
			data.MtSensor |= 0x04;
		}
		else
		{
			data.MtSensor &= 0xFB;
		}
		if (value.dtExtend)
		{
			data.MtSensor |= 0x08;
		}
		else
		{
			data.MtSensor &= 0xF7;
		}
		if (value.buckyInPortrait)
		{
			data.MtSensor |= 0x10;
		}
		else
		{
			data.MtSensor &= 0xEF;
		}
		if (value.buckyInLand)
		{
			data.MtSensor |= 0x20;
		}
		else
		{
			data.MtSensor &= 0xDF;
		}
		if (value.floatCollison)
		{
			data.MtSensor |= 0x40;
		}
		else
		{
			data.MtSensor &= 0xBF;
		}

		mtSensors = value;
	}

	Byte TBLStatusMsgPS::BUCKY_DT::get()
	{
		return buckyDetent;
	}

	void TBLStatusMsgPS::BUCKY_DT::set(Byte value)
	{
		data.BuckyZdt &= 0xF0;
		data.BuckyZdt += value;
		buckyDetent = value;
	}

	Byte TBLStatusMsgPS::Z_DT::get()
	{
		return zDetent;
	}

	void TBLStatusMsgPS::Z_DT::set(Byte value)
	{
		data.BuckyZdt &= 0x0F;
		data.BuckyZdt += (value << 4);
		zDetent = value;
	}

	ControlOpMode TBLStatusMsgPS::OP_MODE::get()
	{
		return operateMode;
	}

	void TBLStatusMsgPS::OP_MODE::set(ControlOpMode value)
	{
		switch (value)
		{
		case ControlOpMode::NoInit:
			data.OpMode = 0x00;
			break;
		case ControlOpMode::Init:
			data.OpMode = 0x01;
			break;
		case ControlOpMode::NMtPara:
			data.OpMode = 0x02;
			break;
		case ControlOpMode::MLock:
			data.OpMode = 0x03;
			break;
		case ControlOpMode::ESTOP:
			data.OpMode = 0x04;
			break;
		case ControlOpMode::NRmCal:
			data.OpMode = 0x05;
			break;
		case ControlOpMode::SoftDTMod:
			data.OpMode = 0x06;
			break;
		case ControlOpMode::MMReq:
			data.OpMode = 0x07;
			break;
		case ControlOpMode::AMTP:
			data.OpMode = 0x08;
			break;
		case ControlOpMode::TKOTC:
			data.OpMode = 0x09;
			break;
		case ControlOpMode::MtNoInit:
			data.OpMode = 0x0C;
			break;
		case ControlOpMode::LT:
			data.OpMode = 0x0D;
			break;
		case ControlOpMode::Idle:
			data.OpMode = 0x0F;
			break;
		default:
			break;
		}

		operateMode = value;
	}


	TBLStatusUpdateReason TBLStatusMsgPS::UPDATE_REASON::get()
	{
		return updateReason;
	}

	void TBLStatusMsgPS::UPDATE_REASON::set(TBLStatusUpdateReason value)
	{
		if (value.swStatusChange)
		{
			data.UpdateReason |= 0x01;
		}
		else
		{
			data.UpdateReason &= 0xFE;
		}
		if (value.xAxisStatusChange)
		{
			data.UpdateReason |= 0x02;
		}
		else
		{
			data.UpdateReason &= 0xFD;
		}
		if (value.mtSensorChange)
		{
			data.UpdateReason |= 0x04;
		}
		else
		{
			data.UpdateReason &= 0xFB;
		}
		if (value.softDtChange)
		{
			data.UpdateReason |= 0x08;
		}
		else
		{
			data.UpdateReason &= 0xF7;
		}
		if (value.opModeChange)
		{
			data.UpdateReason |= 0x10;
		}
		else
		{
			data.UpdateReason &= 0xEF;
		}
		if (value.scbRequest)
		{
			data.UpdateReason |= 0x20;
		}
		else
		{
			data.UpdateReason &= 0xDF;
		}
		if (value.detectorIdCHANGE)
		{
			data.UpdateReason |= 0x40;
		}
		else
		{
			data.UpdateReason &= 0xBF;
		}
		updateReason = value;
	}


	TBLKeyPadStatus TBLStatusMsgPS::KEYPAD_STATUS::get()
	{
		return keypadStatus;
	}

	void TBLStatusMsgPS::KEYPAD_STATUS::set(TBLKeyPadStatus value)
	{
		if (value.relTransSW)
		{
			data.KeyPadStatus |= 0x01;
		}
		else
		{
			data.KeyPadStatus &= 0xFE;
		}
		if (value.relLongSW)
		{
			data.KeyPadStatus |= 0x02;
		}
		else
		{
			data.KeyPadStatus &= 0xFD;
		}
		if (value.relBothSW)
		{
			data.KeyPadStatus |= 0x04;
		}
		else
		{
			data.KeyPadStatus &= 0xFB;
		}
		if (value.tblDownSW)
		{
			data.KeyPadStatus |= 0x08;
		}
		else
		{
			data.KeyPadStatus &= 0xF7;
		}
		if (value.tblUpSW)
		{
			data.KeyPadStatus |= 0x10;
		}
		else
		{
			data.KeyPadStatus &= 0xEF;
		}

		keypadStatus = value;
	}

	TBLIndicatorStatus TBLStatusMsgPS::INDICATOR_STATUS::get()
	{
		return indicatorStatus;
	}

	void TBLStatusMsgPS::INDICATOR_STATUS::set(TBLIndicatorStatus value)
	{
		if (value.gridPresent)
		{
			data.IndDetctorId |= 0x04;
		}
		else
		{
			data.IndDetctorId &= 0xFB;
		}
		if (value.autoTrack)
		{
			data.IndDetctorId |= 0x08;
		}
		else
		{
			data.IndDetctorId &= 0xF7;
		}

		indicatorStatus = value;
	}

	Byte TBLStatusMsgPS::DETECTOR_ID::get()
	{
		return detectorID;
	}

	void TBLStatusMsgPS::DETECTOR_ID::set(Byte value)
	{
		data.IndDetctorId &= 0x0F;
		data.IndDetctorId += (value << 4);
		detectorID = value;
	}

	AxisCtlProtoStuct TBLStatusMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 8;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.SwStatus;
		frame.frameData[1] = data.MtControl;
		frame.frameData[2] = data.MtSensor;
		frame.frameData[3] = data.BuckyZdt;
		frame.frameData[4] = data.OpMode;
		frame.frameData[5] = data.UpdateReason;
		frame.frameData[6] = data.KeyPadStatus;
		frame.frameData[7] = data.IndDetctorId;

		return frame;
	}

	/*************************************/
	//  3.TBL Heartbeat Message (0x330)    /
	/*************************************/
	TBLHBMsgPS::TBLHBMsgPS()
	{
		data.id = 0x330;
	}

	double TBLHBMsgPS::X_POS::get()
	{
		return xPosition;
	}

	void TBLHBMsgPS::X_POS::set(double value)
	{
	    data.XPos = (WORD)(value * 4);
		xPosition = value;
	}

	double TBLHBMsgPS::Y_POS::get()
	{
		return yPosition;
	}

	void TBLHBMsgPS::Y_POS::set(double value)
	{
		data.YPos = (WORD)(value * 4);
		yPosition = value;
	}

	double TBLHBMsgPS::Z_POS::get()
	{
		return zPosition;
	}

	void TBLHBMsgPS::Z_POS::set(double value)
	{
		data.ZPos = (WORD)(value * 4);
		zPosition = value;
	}

	TBLCalStatus TBLHBMsgPS::CALSTATUS::get()
	{
		return calStatus;
	}

	void TBLHBMsgPS::CALSTATUS::set(TBLCalStatus value)
	{
		if (value.zMinSet)
		{
			data.AxisCalStatus |= 0x01;
		}
		else
		{
			data.AxisCalStatus &= 0xFE;
		}
		if (value.longMinSet)
		{
			data.AxisCalStatus |= 0x02;
		}
		else
		{
			data.AxisCalStatus &= 0xFD;
		}
		if (value.zMaxSet)
		{
			data.AxisCalStatus |= 0x04;
		}
		else
		{
			data.AxisCalStatus &= 0xFB;
		}
		if (value.longMaxSet)
		{
			data.AxisCalStatus |= 0x08;
		}
		else
		{
			data.AxisCalStatus &= 0xF7;
		}
		if (value.tblLocEst)
		{
			data.AxisCalStatus |= 0x20;
		}
		else
		{
			data.AxisCalStatus &= 0xDF;
		}
		if (value.rmCalDone)
		{
			data.AxisCalStatus |= 0x40;
		}
		else
		{
			data.AxisCalStatus &= 0xBF;
		}
		if (value.pinCalDone)
		{
			data.AxisCalStatus |= 0x80;
		}
		else
		{
			data.AxisCalStatus &= 0x7F;
		}

		calStatus = value;
	}

	TBLHBUpdateReason TBLHBMsgPS::UPDATEREASON::get()
	{
		return updateReason;
	}

	void TBLHBMsgPS::UPDATEREASON::set(TBLHBUpdateReason value)
	{
		if (value.hbTimeOut)
		{
			data.UpdatereasonOr |= 0x01;
		}
		else
		{
			data.UpdatereasonOr &= 0xFE;
		}
		if (value.scbRequest)
		{
			data.UpdatereasonOr |= 0x02;
		}
		else
		{
			data.UpdatereasonOr &= 0xFD;
		}
		if (value.movementOccur)
		{
			data.UpdatereasonOr |= 0x04;
		}
		else
		{
			data.UpdatereasonOr &= 0xFB;
		}
		if (value.calStatusChange)
		{
			data.UpdatereasonOr |= 0x08;
		}
		else
		{
			data.UpdatereasonOr &= 0xF7;
		}

		updateReason = value;
	}

	TBLRoomLayout TBLHBMsgPS::LAYOUT::get()
	{
		return layout;
	}

	void TBLHBMsgPS::LAYOUT::set(TBLRoomLayout value)
	{
		data.UpdatereasonOr &= 0x0F;
		switch (value)
		{
		case TBLRoomLayout::NotSet:
			data.UpdatereasonOr |= 0x00;
			break;
		case TBLRoomLayout::Layout1:
			data.UpdatereasonOr |= 0x10;
			break;
		case TBLRoomLayout::Layout2:
			data.UpdatereasonOr |= 0x20;
			break;
		case TBLRoomLayout::Layout3:
			data.UpdatereasonOr |= 0x30;
			break;
		case TBLRoomLayout::Layout4:
			data.UpdatereasonOr |= 0x40;
			break;
		default:
			break;
		}
		layout = value;
	}

	AxisCtlProtoStuct TBLHBMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 8;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.XPosM;
		frame.frameData[1] = data.XPosL;
		frame.frameData[2] = data.YPosM;
		frame.frameData[3] = data.YPosL;
		frame.frameData[4] = data.ZPosM;
		frame.frameData[5] = data.ZPosL;
		frame.frameData[6] = data.AxisCalStatus;
		frame.frameData[7] = data.UpdatereasonOr;
		return frame;
	}


	/**************************************************/
	//   4. TBL LLI Status Message (0x335)            //
	/**************************************************/

	TBLLLIStatusMsgPS::TBLLLIStatusMsgPS()
	{
		data.id = 0x335;
		data.BuckyLLIPos = 0x80000000;
		data.BuckyTravEnd = data.BuckyTravStart = 0x8000;
	}

	Int32 TBLLLIStatusMsgPS::BUCKY_LLIPOS::get()
	{
		return buckyLLIPos;
	}

	void TBLLLIStatusMsgPS::BUCKY_LLIPOS::set(Int32 value)
	{
		data.BuckyLLIPos = value;
		buckyLLIPos = value;
	}

	double TBLLLIStatusMsgPS::BUCKYTRAV_END::get()
	{
		return buckyTravEnd;
	}

	void TBLLLIStatusMsgPS::BUCKYTRAV_END::set(double value)
	{
		data.BuckyTravEnd = (WORD)(value * 4);
		buckyTravEnd = value;
	}

	double TBLLLIStatusMsgPS::BUCKYTRAV_START::get()
	{
		return buckyTravStart;
	}

	void TBLLLIStatusMsgPS::BUCKYTRAV_START::set(double value)
	{
		data.BuckyTravStart = (WORD)(value * 4);
		buckyTravStart = value;
	}

	AxisCtlProtoStuct TBLLLIStatusMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 8;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.BuckyLLIPosByte1;
		frame.frameData[1] = data.BuckyLLIPosByte2;
		frame.frameData[2] = data.BuckyLLIPosByte3;
		frame.frameData[3] = data.BuckyLLIPosByte4;
		frame.frameData[4] = data.BuckyTravEndM;
		frame.frameData[5] = data.BuckyTravEndL;
		frame.frameData[6] = data.BuckyTravStartM;
		frame.frameData[7] = data.BuckyTravStartL;

		return frame;
	}


	/*************************  WSD Messages  *************************/

	/********************************************************************/
	//   1. WSD Status Message (0x220) /
	/********************************************************************/
	WSDStatusMsgPS::WSDStatusMsgPS()
	{
		data.id = 0x220;
	}

	WSDSWStatus WSDStatusMsgPS::SW_STATUS::get()
	{
		return swStatus;
	}

	void WSDStatusMsgPS::SW_STATUS::set(WSDSWStatus value)
	{
		if (value.zPressed == true)
		{
			data.SWIndStatus |= 0x01;
			swStatus.zPressed = true;
		}
		else
		{
			data.SWIndStatus &= 0xFE;
			swStatus.zPressed = false;
		}

		if (value.xPressed == true)
		{
			data.SWIndStatus |= 0x02;
			swStatus.xPressed = true;
		}
		else
		{
			data.SWIndStatus &= 0xFD;
			swStatus.xPressed = false;
		}
		if (value.estopPressed == true)
		{
			data.SWIndStatus |= 0x10;
			swStatus.estopPressed = true;
		}
		else
		{
			data.SWIndStatus &= 0xEF;
			swStatus.estopPressed = false;
		}
	}


	WSDMotionControls WSDStatusMsgPS::MT_CONTROL::get()
	{
		return motionControls;
	}

	void WSDStatusMsgPS::MT_CONTROL::set(WSDMotionControls value)
	{
		if (value.zMotorRun == true)
		{
			data.MtControl |= 0x01;
			motionControls.zMotorRun = true;
		}
		else
		{
			data.MtControl &= 0xFE;
			motionControls.zMotorRun = false;
		}

		if (value.zBrakeEn == true)
		{
			data.MtControl |= 0x04;
			motionControls.zBrakeEn = true;
		}
		else
		{
			data.MtControl &= 0xFB;
			motionControls.zBrakeEn = false;
		}
		if (value.xMotorRun == true)
		{
			data.MtControl |= 0x08;
			motionControls.xMotorRun = true;
		}
		else
		{
			data.MtControl &= 0xF7;
			motionControls.xMotorRun = false;
		}
		if (value.tiltMotorRun == true)
		{
			data.MtControl |= 0x40;
			motionControls.tiltMotorRun = true;
		}
		else
		{
			data.MtControl &= 0xBF;
			motionControls.tiltMotorRun = false;
		}
	}

	DetGridSensor WSDStatusMsgPS::DT_SENSOR::get()
	{
		return detGridSensors;
	}

	void WSDStatusMsgPS::DT_SENSOR::set(DetGridSensor value)
	{
		if (value.buckyInPort == true)
		{
			data.DetGrSensor |= 0x08;
			detGridSensors.buckyInPort = true;
		}
		else
		{
			data.DetGrSensor &= 0xF7;
			detGridSensors.buckyInPort = false;
		}
		if (value.buckyInLand == true)
		{
			data.DetGrSensor |= 0x10;
			detGridSensors.buckyInLand = true;
		}
		else
		{
			data.DetGrSensor &= 0xEF;
			detGridSensors.buckyInLand = false;
		}
		if (value.detInPort == true)
		{
			data.DetGrSensor |= 0x20;
			detGridSensors.detInPort = true;
		}
		else
		{
			data.DetGrSensor &= 0xDF;
			detGridSensors.detInPort = false;
		}
		if (value.detInLand == true)
		{
			data.DetGrSensor |= 0x40;
			detGridSensors.detInLand = true;
		}
		else
		{
			data.DetGrSensor &= 0xBF;
			detGridSensors.detInLand = false;
		}
	}

	GridType WSDStatusMsgPS::GRID_TYPE::get()
	{
		return gridType;
	}

	void WSDStatusMsgPS::GRID_TYPE::set(GridType value)
	{
		switch (value)
		{
		case SCBProtocalWrapper::GridType::Type1:
			data.DetGrSensor &= 0xF8;
			data.DetGrSensor += 0x01;
			break;
		case SCBProtocalWrapper::GridType::Type2:
			data.DetGrSensor &= 0xF8;
			data.DetGrSensor += 0x02;
			break;
		case SCBProtocalWrapper::GridType::Type3:
			data.DetGrSensor &= 0xF8;
			data.DetGrSensor += 0x03;
			break;
		case SCBProtocalWrapper::GridType::Type4:
			data.DetGrSensor &= 0xF8;
			data.DetGrSensor += 0x04;
			break;
		case SCBProtocalWrapper::GridType::Type5:
			data.DetGrSensor &= 0xF8;
			data.DetGrSensor += 0x05;
			break;
		case SCBProtocalWrapper::GridType::Type6:
			data.DetGrSensor &= 0xF8;
			data.DetGrSensor += 0x06;
			break;
		case SCBProtocalWrapper::GridType::Type7:
			data.DetGrSensor &= 0xF8;
			data.DetGrSensor += 0x07;
			break;
		default:
			data.DetGrSensor &= 0xF8;
			break;
		}
		gridType = value;
	}

	Sensors WSDStatusMsgPS::SENSORS::get()
	{
		return sensors;
	}

	void WSDStatusMsgPS::SENSORS::set(Sensors value)
	{
		if (value.isTrayIn == true)
		{
			data.Sensors |= 0x01;
			sensors.isTrayIn = true;
		}
		else
		{
			data.Sensors &= 0xFE;
			sensors.isTrayIn = false;
		}
		if (value.buckyRetract == true)
		{
			data.Sensors |= 0x08;
			sensors.buckyRetract = true;
		}
		else
		{
			data.Sensors &= 0xF7;
			sensors.buckyRetract = false;
		}
		if (value.buckyExtend == true)
		{
			data.Sensors |= 0x10;
			sensors.buckyExtend = true;
		}
		else
		{
			data.Sensors &= 0xEF;
			sensors.buckyExtend = false;
		}
	}

	ControlOpMode WSDStatusMsgPS::OP_MODE::get()
	{
		return opMode;
	}

	void WSDStatusMsgPS::OP_MODE::set(ControlOpMode value)
	{
		switch (value)
		{
		case ControlOpMode::NoInit:
			data.OpMode = 0x00;
			opMode = value;
			break;
		case ControlOpMode::Init:
			data.OpMode = 0x01;
			opMode = value;
			break;
		case ControlOpMode::NMtPara:
			data.OpMode = 0x02;
			opMode = value;
			break;
		case ControlOpMode::MLock:
			data.OpMode = 0x03;
			opMode = value;
			break;
		case ControlOpMode::ESTOP:
			data.OpMode = 0x04;
			opMode = value;
			break;
		case ControlOpMode::NRmCal:
			data.OpMode = 0x05;
			opMode = value;
			break;
		case ControlOpMode::SoftDTMod:
			data.OpMode = 0x06;
			opMode = value;
			break;
		case ControlOpMode::MMReq:
			data.OpMode = 0x07;
			opMode = value;
			break;
		case ControlOpMode::AMTP:
			data.OpMode = 0x08;
			opMode = value;
			break;
		case ControlOpMode::TKOTC :
			data.OpMode = 0x09;
			opMode = value;
			break;
		case ControlOpMode::MtNoInit:
			data.OpMode = 0x0C;
			opMode = value;
			break;
		case ControlOpMode::LT:
			data.OpMode = 0x0D;
			opMode = value;
			break;
		case ControlOpMode::Idle:
			data.OpMode = 0x0F;
			opMode = value;
			break;
		default:
			break;
		}
	}

	UINT8 WSDStatusMsgPS::Z_DT::get()
	{
		return zSoftDetent;
	}

	void WSDStatusMsgPS::Z_DT::set(UINT8 value)
	{
		data.ZXDetent &= 0xF0;
		data.ZXDetent += value;
		zSoftDetent = value;
	}

	UINT8 WSDStatusMsgPS::X_DT::get()
	{
		return xSoftDetent;
	}

	void WSDStatusMsgPS::X_DT::set(UINT8 value)
	{
		data.ZXDetent &= 0x0F;
		data.ZXDetent += value << 4;
		xSoftDetent = value;
	}

	UINT8 WSDStatusMsgPS::TILT_DT::get()
	{
		return tiltDetent;
	}

	void WSDStatusMsgPS::TILT_DT::set(UINT8 value)
	{
		data.TlDtDetectId &= 0xF0;
		data.TlDtDetectId += value;
		tiltDetent = value;
	}

	UINT8 WSDStatusMsgPS::DETECTOR_ID::get()
	{
		return detectorID;
	}

	void WSDStatusMsgPS::DETECTOR_ID::set(UINT8 value)
	{
		data.TlDtDetectId &= 0x0F;
		data.TlDtDetectId += value << 4;
		detectorID = value;
	}

	WSDStatusMsgUpdateReason WSDStatusMsgPS::UPDATE_REASON::get()
	{
		return updateReason;
	}

	void WSDStatusMsgPS::UPDATE_REASON::set(WSDStatusMsgUpdateReason value)
	{
		if (value.swIndStateChange == true)
		{
			data.UpdateReason |= 0x01;
			updateReason.swIndStateChange = true;
		}
		else
		{
			data.UpdateReason &= 0xFE;
			updateReason.swIndStateChange = false;
		}

		if (value.motionCtlChange == true)
		{
			data.UpdateReason |= 0x02;
			updateReason.motionCtlChange = true;
		}
		else
		{
			data.UpdateReason &= 0xFD;
			updateReason.motionCtlChange = false;
		}
		if (value.buckyGridChange == true)
		{
			data.UpdateReason |= 0x04;
			updateReason.buckyGridChange = true;
		}
		else
		{
			data.UpdateReason &= 0xFB;
			updateReason.buckyGridChange = false;
		}
		if (value.sensorChange == true)
		{
			data.UpdateReason |= 0x08;
			updateReason.sensorChange = true;
		}
		else
		{
			data.UpdateReason &= 0xF7;
			updateReason.sensorChange = false;
		}
		if (value.opModeChange == true)
		{
			data.UpdateReason |= 0x10;
			updateReason.opModeChange = true;
		}
		else
		{
			data.UpdateReason &= 0xEF;
			updateReason.opModeChange = false;
		}
		if (value.detIdChange == true)
		{
			data.UpdateReason |= 0x20;
			updateReason.detIdChange = true;
		}
		else
		{
			data.UpdateReason &= 0xDF;
			updateReason.detIdChange = false;
		}
		if (value.detentChange == true)
		{
			data.UpdateReason |= 0x40;
			updateReason.detentChange = true;
		}
		else
		{
			data.UpdateReason &= 0xBF;
			updateReason.detentChange = false;
		}
		if (value.scbRequest == true)
		{
			data.UpdateReason |= 0x80;
			updateReason.scbRequest = true;
		}
		else
		{
			data.UpdateReason &= 0x7F;
			updateReason.scbRequest = false;
		}
	}

	AxisCtlProtoStuct WSDStatusMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 8;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.SWIndStatus;
		frame.frameData[1] = data.MtControl;
		frame.frameData[2] = data.DetGrSensor;
		frame.frameData[3] = data.Sensors;
		frame.frameData[4] = data.OpMode;
		frame.frameData[5] = data.ZXDetent;
		frame.frameData[6] = data.TlDtDetectId;
		frame.frameData[7] = data.UpdateReason;

		return frame;
	}



	/********************************************************************/
	//   2. WSD Keypad Status Message (0x250) /
	/********************************************************************/

	WSDKeyStatusMsgPS::WSDKeyStatusMsgPS()
	{
		data.id = 0x250;
	}

	SimpleMotionSW WSDKeyStatusMsgPS::SIM_MT_SW::get()
	{
		return simMotionSW;
	}

	void WSDKeyStatusMsgPS::SIM_MT_SW::set(SimpleMotionSW value)
	{
		if (value.hsUpSW == true)
		{
			data.SimMtSW |= 0x01;
			simMotionSW.hsUpSW = true;
		}
		else
		{
			data.SimMtSW &= 0xFE;
			simMotionSW.hsUpSW = false;
		}
		if (value.lsUpSW == true)
		{
			data.SimMtSW |= 0x02;
			simMotionSW.lsUpSW = true;
		}
		else
		{
			data.SimMtSW &= 0xFD;
			simMotionSW.lsUpSW = false;
		}
		if (value.hsDownSW == true)
		{
			data.SimMtSW |= 0x04;
			simMotionSW.hsDownSW = true;
		}
		else
		{
			data.SimMtSW &= 0xFB;
			simMotionSW.hsDownSW = false;
		}
		if (value.lsDownSW == true)
		{
			data.SimMtSW |= 0x08;
			simMotionSW.lsDownSW = true;
		}
		else
		{
			data.SimMtSW &= 0xF7;
			simMotionSW.lsDownSW = false;
		}
		if (value.hsLeftSW == true)
		{
			data.SimMtSW |= 0x10;
			simMotionSW.hsLeftSW = true;
		}
		else
		{
			data.SimMtSW &= 0xEF;
			simMotionSW.hsLeftSW = false;
		}
		if (value.lsLeftSW == true)
		{
			data.SimMtSW |= 0x20;
			simMotionSW.lsLeftSW = true;
		}
		else
		{
			data.SimMtSW &= 0xDF;
			simMotionSW.lsLeftSW = false;
		}
		if (value.hsRightSW == true)
		{
			data.SimMtSW |= 0x40;
			simMotionSW.hsRightSW = true;
		}
		else
		{
			data.SimMtSW &= 0xBF;
			simMotionSW.hsRightSW = false;
		}
		if (value.lsRightSW == true)
		{
			data.SimMtSW |= 0x80;
			simMotionSW.lsRightSW = true;
		}
		else
		{
			data.SimMtSW &= 0x7F;
			simMotionSW.lsRightSW = false;
		}
	}

	TiltSW WSDKeyStatusMsgPS::TILT_SW::get()
	{
		return tiltSW;
	}

	void WSDKeyStatusMsgPS::TILT_SW::set(TiltSW value)
	{
		if (value.tiltUpSW == true)
		{
			data.TiltSW |= 0x01;
			tiltSW.tiltUpSW = true;
		}
		else
		{
			data.TiltSW &= 0xFE;
			tiltSW.tiltUpSW = false;
		}
		if (value.tiltDownSW == true)
		{
			data.TiltSW |= 0x02;
			tiltSW.tiltDownSW = true;
		}
		else
		{
			data.TiltSW &= 0xFD;
			tiltSW.tiltDownSW = false;
		}
	}

	ComplexMotionSW WSDKeyStatusMsgPS::COM_MT_SW::get()
	{
		return comMotionSW;
	}

	void WSDKeyStatusMsgPS::COM_MT_SW::set(ComplexMotionSW value)
	{
		if (value.autoCenterSW == true)
		{
			data.ComMtSW |= 0x01;
			comMotionSW.autoCenterSW = true;
		}
		else
		{
			data.ComMtSW &= 0xFE;
			comMotionSW.autoCenterSW = false;
		}
		if (value.autoTrackSW == true)
		{
			data.ComMtSW |= 0x02;
			comMotionSW.autoTrackSW = true;
		}
		else
		{
			data.ComMtSW &= 0xFD;
			comMotionSW.autoTrackSW = false;
		}
		if (value.autoPosSW == true)
		{
			data.ComMtSW |= 0x04;
			comMotionSW.autoPosSW = true;
		}
		else
		{
			data.ComMtSW &= 0xFB;
			comMotionSW.autoPosSW = false;
		}
		if (value.autoCenter2SW == true)
		{
			data.ComMtSW |= 0x08;
			comMotionSW.autoCenter2SW = true;
		}
		else
		{
			data.ComMtSW &= 0xF7;
			comMotionSW.autoCenter2SW = false;
		}
	}

	KeyIndicator WSDKeyStatusMsgPS::KEY_INDICATOR::get()
	{
		return keyIndicators;
	}

	void WSDKeyStatusMsgPS::KEY_INDICATOR::set(KeyIndicator value)
	{
		if (value.autoTrackInd == true)
		{
			data.KeyIndt |= 0x01;
			keyIndicators.autoTrackInd = true;
		}
		else
		{
			data.KeyIndt &= 0xFE;
			keyIndicators.autoTrackInd = false;
		}
		if (value.autoPosInd == true)
		{
			data.KeyIndt |= 0x02;
			keyIndicators.autoPosInd = true;
		}
		else
		{
			data.KeyIndt &= 0xFD;
			keyIndicators.autoPosInd = false;
		}
		if (value.autoCenterInd == true)
		{
			data.KeyIndt |= 0x04;
			keyIndicators.autoCenterInd = true;
		}
		else
		{
			data.KeyIndt &= 0xFB;
			keyIndicators.autoCenterInd = false;
		}
	}

	ColPresetSW WSDKeyStatusMsgPS::COL_PRESET_SW::get()
	{
		return colPresetSW;
	}

	void WSDKeyStatusMsgPS::COL_PRESET_SW::set(ColPresetSW value)
	{
		if (value.Pre1SW == true)
		{
			data.ColPsetSW |= 0x01;
			colPresetSW.Pre1SW = true;
		}
		else
		{
			data.ColPsetSW &= 0xFE;
			colPresetSW.Pre1SW = false;
		}
		if (value.Pre2SW == true)
		{
			data.ColPsetSW |= 0x02;
			colPresetSW.Pre2SW = true;
		}
		else
		{
			data.ColPsetSW &= 0xFD;
			colPresetSW.Pre2SW = false;
		}
		if (value.Pre3SW == true)
		{
			data.ColPsetSW |= 0x04;
			colPresetSW.Pre3SW = true;
		}
		else
		{
			data.ColPsetSW &= 0xFB;
			colPresetSW.Pre3SW = false;
		}
	}

	ColBladePosSW WSDKeyStatusMsgPS::COL_BLADE_SW::get()
	{
		return colBPSW;
	}

	void WSDKeyStatusMsgPS::COL_BLADE_SW::set(ColBladePosSW value)
	{
		if (value.openHBSW == true)
		{
			data.ColBPSW |= 0x01;
			colBPSW.openHBSW = true;
		}
		else
		{
			data.ColBPSW &= 0xFE;
			colBPSW.openHBSW = false;
		}
		if (value.closeHBSW == true)
		{
			data.ColBPSW |= 0x02;
			colBPSW.closeHBSW = true;
		}
		else
		{
			data.ColBPSW &= 0xFD;
			colBPSW.closeHBSW = false;
		}
		if (value.openVBSW = true)
		{
			data.ColBPSW |= 0x04;
			colBPSW.openVBSW = true;
		}
		else
		{
			data.ColBPSW &= 0xFB;
			colBPSW.openVBSW = false;
		}
		if (value.closeVBSW == true)
		{
			data.ColBPSW |= 0x08;
			colBPSW.closeVBSW = true;
		}
		else
		{
			data.ColBPSW &= 0xF7;
			colBPSW.closeVBSW = false;
		}
		if (value.openHBTapped == true)
		{
			data.ColBPSW |= 0x10;
			colBPSW.openHBTapped = true;
		}
		else
		{
			data.ColBPSW &= 0xEF;
			colBPSW.openHBTapped = false;
		}
		if (value.closeHBTapped == true)
		{
			data.ColBPSW |= 0x20;
			colBPSW.closeHBTapped = true;
		}
		else
		{
			data.ColBPSW &= 0xDF;
			colBPSW.closeHBTapped = false;
		}
		if (value.openVBTapped == true)
		{
			data.ColBPSW |= 0x40;
			colBPSW.openVBTapped = true;
		}
		else
		{
			data.ColBPSW &= 0xBF;
			colBPSW.openVBTapped = false;
		}
		if (value.closeVBTapped == true)
		{
			data.ColBPSW |= 0x80;
			colBPSW.closeVBTapped = true;
		}
		else
		{
			data.ColBPSW &= 0x7F;
			colBPSW.closeVBTapped = false;
		}
	}

	bool WSDKeyStatusMsgPS::LAMP_SW::get()
	{
		return colLampSW;
	}

	void WSDKeyStatusMsgPS::LAMP_SW::set(bool value)
	{
		if (value == true)
		{
			data.ColLampSW |= 0x01;
			colLampSW = true;
		}
		else
		{
			data.ColLampSW &= 0xFE;
			colLampSW = false;
		}
	}

	WSDKEYStatusMsgUpdateReason WSDKeyStatusMsgPS::UPDATE_REASON::get()
	{
		return updateReason;
	}

	void WSDKeyStatusMsgPS::UPDATE_REASON::set(WSDKEYStatusMsgUpdateReason value)
	{
		if (value.simMtSWChange == true)
		{
			data.UpdateReason |= 0x01;
			updateReason.simMtSWChange = true;
		}
		else
		{
			data.UpdateReason &= 0xFE;
			updateReason.simMtSWChange = false;
		}
		if (value.tiltSWChange == true)
		{
			data.UpdateReason |= 0x02;
			updateReason.tiltSWChange = true;
		}
		else
		{
			data.UpdateReason &= 0xFD;
			updateReason.tiltSWChange = false;
		}
		if (value.comMtSWChange == true)
		{
			data.UpdateReason |= 0x04;
			updateReason.comMtSWChange = true;
		}
		else
		{
			data.UpdateReason &= 0xFB;
			updateReason.comMtSWChange = false;
		}
		if (value.colPSChange == true)
		{
			data.UpdateReason |= 0x08;
			updateReason.colPSChange = true;
		}
		else
		{
			data.UpdateReason &= 0xF7;
			updateReason.colPSChange = false;
		}
		if (value.colBSChange == true)
		{
			data.UpdateReason |= 0x10;
			updateReason.colBSChange = true;
		}
		else
		{
			data.UpdateReason &= 0xEF;
			updateReason.colBSChange = false;
		}
		if (value.colLampSWChange == true)
		{
			data.UpdateReason |= 0x20;
			updateReason.colLampSWChange = true;
		}
		else
		{
			data.UpdateReason &= 0xDF;
			updateReason.colLampSWChange = false;
		}
		if (value.scbRequest == true)
		{
			data.UpdateReason |= 0x40;
			updateReason.scbRequest = true;
		}
		else
		{
			data.UpdateReason &= 0xBF;
			updateReason.scbRequest = false;
		}
	}

	AxisCtlProtoStuct WSDKeyStatusMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 8;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.SimMtSW;
		frame.frameData[1] = data.TiltSW;
		frame.frameData[2] = data.ComMtSW;
		frame.frameData[3] = data.KeyIndt;
		frame.frameData[4] = data.ColPsetSW;
		frame.frameData[5] = data.ColBPSW;
		frame.frameData[6] = data.ColLampSW;
		frame.frameData[7] = data.UpdateReason;

		return frame;
	}



	/*************************************/
	//   3. WSD Heartbeat Message (0x320) /
	/*************************************/

	WSDHBMsgPS::WSDHBMsgPS()
	{
		data.id = 0x320;
		xPos = yPos = zPos = 0;
	}

	double WSDHBMsgPS::X_POS::get()
	{
		return xPos;
	}

	void WSDHBMsgPS::X_POS::set(double value)
	{
		data.XPos = (WORD)(value * 4);
		xPos = value;
	}

	double WSDHBMsgPS::Y_POS::get()
	{
		return yPos;
	}

	void WSDHBMsgPS::Y_POS::set(double value)
	{
		data.YPos = (WORD)(value * 4);
		yPos = value;
	}

	double WSDHBMsgPS::Z_POS::get()
	{
		return zPos;
	}

	void WSDHBMsgPS::Z_POS::set(double value)
	{
		data.ZPos = (WORD)(value * 4);
		zPos = value;
	}

	WSDCalStatus WSDHBMsgPS::CAL_STATUS::get()
	{
		return calStatus;
	}

	void WSDHBMsgPS::CAL_STATUS::set(WSDCalStatus value)
	{
		if (value.ZMinSet == true)
		{
			data.AxisCalStatus |= 0x01;
			calStatus.ZMinSet = true;
		}
		else
		{
			data.AxisCalStatus &= 0xFE;
			calStatus.ZMinSet = false;
		}
		if (value.ZMaxSet == true)
		{
			data.AxisCalStatus |= 0x04;
			calStatus.ZMaxSet = true;
		}
		else
		{
			data.AxisCalStatus &= 0xFB;
			calStatus.ZMaxSet = false;
		}
		if (value.XMinSet == true)
		{
			data.AxisCalStatus |= 0x08;
			calStatus.XMinSet = true;
		}
		else
		{
			data.AxisCalStatus &= 0xF7;
			calStatus.XMinSet = false;
		}
		if (value.XMaxSet == true)
		{
			data.AxisCalStatus |= 0x10;
			calStatus.XMaxSet = true;
		}
		else
		{
			data.AxisCalStatus &= 0xEF;
			calStatus.XMaxSet = false;
		}
		if (value.HorRMCalDone == true)
		{
			data.AxisCalStatus |= 0x20;
			calStatus.HorRMCalDone = true;
		}
		else
		{
			data.AxisCalStatus &= 0xDF;
			calStatus.HorRMCalDone = false;
		}
		if (value.VertRMCalDone == true)
		{
			data.AxisCalStatus |= 0x40;
			calStatus.VertRMCalDone = true;
		}
		else
		{
			data.AxisCalStatus &= 0xBF;
			calStatus.VertRMCalDone = false;
		}
		if (value.PinCalDone == true)
		{
			data.AxisCalStatus |= 0x80;
			calStatus.PinCalDone = true;
		}
		else
		{
			data.AxisCalStatus &= 0x7F;
			calStatus.PinCalDone = false;
		}
	}

	WSDHBUpdateReason WSDHBMsgPS::UPDATE_REASON::get()
	{
		return updateReason;
	}

	void WSDHBMsgPS::UPDATE_REASON::set(WSDHBUpdateReason value)
	{
		if (value.hbTimeOut == true)
		{
			data.UpdatereasonOr |= 0x01;
			updateReason.hbTimeOut = true;
		}
		else
		{
			data.UpdatereasonOr &= 0xFE;
			updateReason.hbTimeOut = false;
		}
		if (value.scbRequest == true)
		{
			data.UpdatereasonOr |= 0x02;
			updateReason.scbRequest = true;
		}
		else
		{
			data.UpdatereasonOr &= 0xFD;
			updateReason.scbRequest = false;
		}
		if (value.movementOccur == true)
		{
			data.UpdatereasonOr |= 0x04;
			updateReason.movementOccur = true;
		}
		else
		{
			data.UpdatereasonOr &= 0xFB;
			updateReason.movementOccur = false;
		}
		if (value.calStatusChange == true)
		{
			data.UpdatereasonOr |= 0x08;
			updateReason.calStatusChange = true;
		}
		else
		{
			data.UpdatereasonOr &= 0xF7;
			updateReason.calStatusChange = false;
		}
	}

	WSDRoomLayout WSDHBMsgPS::LAYOUT::get()
	{
		return layout;
	}

	void WSDHBMsgPS::LAYOUT::set(WSDRoomLayout value)
	{
		switch (value)
		{
		case WSDRoomLayout::Layout1:
			data.UpdatereasonOr &= 0x0F;
			data.UpdatereasonOr += 0x10;
			layout = value;
			break;
		case WSDRoomLayout::Layout2:
			data.UpdatereasonOr &= 0x0F;
			data.UpdatereasonOr += 0x20;
			layout = value;
			break;
		case WSDRoomLayout::Layout3:
			data.UpdatereasonOr &= 0x0F;
			data.UpdatereasonOr += 0x30;
			layout = value;
			break;
		case WSDRoomLayout::Layout4:
			data.UpdatereasonOr &= 0x0F;
			data.UpdatereasonOr += 0x40;
			layout = value;
			break;
		default:
			data.UpdatereasonOr &= 0x0F;
			layout = value;
			break;
		}
	}

	AxisCtlProtoStuct WSDHBMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 8;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.XPosM;
		frame.frameData[1] = data.XPosL;
		frame.frameData[2] = data.YPosM;
		frame.frameData[3] = data.YPosL;
		frame.frameData[4] = data.ZPosM;
		frame.frameData[5] = data.ZPosL;
		frame.frameData[6] = data.AxisCalStatus;
		frame.frameData[7] = data.UpdatereasonOr;

		return frame;
	}



	/*******************************************************/
	//   4. WSD Tilt and Angulation Postion Message (0x321) /
	/*******************************************************/

	WSDTiltPosMsgPS::WSDTiltPosMsgPS()
	{
		data.id = 0x321;
		tiltPos = angPos = 0;
	}

	double WSDTiltPosMsgPS::TILT_POS::get()
	{
		return tiltPos;
	}

	void WSDTiltPosMsgPS::TILT_POS::set(double value)
	{
		data.TiltPos = (WORD)(value * 10);
		tiltPos = value;
	}

	double WSDTiltPosMsgPS::ANG_POS::get()
	{
		return angPos;
	}

	void WSDTiltPosMsgPS::ANG_POS::set(double value)
	{
		data.AngPos = (WORD)(value * 10);
		angPos = value;
	}

	WSDTiltCalStatus WSDTiltPosMsgPS::CAL_STATUS::get()
	{
		return calStatus;
	}

	void WSDTiltPosMsgPS::CAL_STATUS::set(WSDTiltCalStatus value)
	{
		if (value.TiltMinSet == true)
		{
			data.AxisCalStatus |= 0x01;
			calStatus.TiltMinSet = true;
		}
		else
		{
			data.AxisCalStatus &= 0xFE;
			calStatus.TiltMinSet = false;
		}
		if (value.TiltVertSet == true)
		{
			data.AxisCalStatus |= 0x02;
			calStatus.TiltVertSet = true;
		}
		else
		{
			data.AxisCalStatus &= 0xFD;
			calStatus.TiltVertSet = false;
		}
		if (value.TiltHorSet == true)
		{
			data.AxisCalStatus |= 0x04;
			calStatus.TiltHorSet = true;
		}
		else
		{
			data.AxisCalStatus &= 0xFB;
			calStatus.TiltHorSet = false;
		}
	}

	WSDTiltPosUpdateReason WSDTiltPosMsgPS::UPDATE_REASON::get()
	{
		return updateReason;
	}

	void WSDTiltPosMsgPS::UPDATE_REASON::set(WSDTiltPosUpdateReason value)
	{
		if (value.hbTimeOut == true)
		{
			data.UpdateReason |= 0x01;
			updateReason.hbTimeOut = true;
		}
		else
		{
			data.UpdateReason &= 0xFE;
			updateReason.hbTimeOut = false;
		}
		if (value.scbRequest == true)
		{
			data.UpdateReason |= 0x02;
			updateReason.scbRequest = true;
		}
		else
		{
			data.UpdateReason &= 0xFD;
			updateReason.scbRequest = false;
		}
		if (value.movementOccur == true)
		{
			data.UpdateReason |= 0x04;
			updateReason.movementOccur = true;
		}
		else
		{
			data.UpdateReason &= 0xFB;
			updateReason.movementOccur = false;
		}
		if (value.calStaChange == true)
		{
			data.UpdateReason |= 0x08;
			updateReason.calStaChange = true;
		}
		else
		{
			data.UpdateReason &= 0xF7;
			updateReason.calStaChange = false;
		}
	}

	AxisCtlProtoStuct WSDTiltPosMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 6;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.TiltPosM;
		frame.frameData[1] = data.TiltPosL;
		frame.frameData[2] = data.AngPosM;
		frame.frameData[3] = data.AngPosL;
		frame.frameData[4] = data.AxisCalStatus;
		frame.frameData[5] = data.UpdateReason;

		return frame;

	}


	/**************************************************/
	//   5. WSD LLI Status Message (0x325)            //
	/**************************************************/
	WSDLLIStatusMsgPS::WSDLLIStatusMsgPS()
	{
		data.id = 0x325;
		data.ZLLIPos = 0x80000000;
		data.ZTravEnd = data.ZTravStart = 0x8000;
	}

	Int32 WSDLLIStatusMsgPS::Z_LLIPOS::get()
	{
		return zLLIPos;
	}

	void WSDLLIStatusMsgPS::Z_LLIPOS::set(Int32 value)
	{
		data.ZLLIPos = value;
		zLLIPos = value;
	}

	double WSDLLIStatusMsgPS::ZTRAV_END::get()
	{
		return zTravEnd;
	}

	void WSDLLIStatusMsgPS::ZTRAV_END::set(double value)
	{
		WORD temp = (WORD)(value * 4);
		data.ZTravEnd = temp;
		zTravEnd = value;
	}

	double WSDLLIStatusMsgPS::ZTRAV_START::get()
	{
		return zTravStart;
	}

	void WSDLLIStatusMsgPS::ZTRAV_START::set(double value)
	{
		data.ZTravStart = (WORD)(value * 4);
		zTravStart = value;
	}

	AxisCtlProtoStuct WSDLLIStatusMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 8;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.ZLLIPosByte1;
		frame.frameData[1] = data.ZLLIPosByte2;
		frame.frameData[2] = data.ZLLIPosByte3;
		frame.frameData[3] = data.ZLLIPosByte4;
		frame.frameData[4] = data.ZTravEndM;
		frame.frameData[5] = data.ZTravEndL;
		frame.frameData[6] = data.ZTravStartM;
		frame.frameData[7] = data.ZTravStartL;

		return frame;
	}


	/*************************  Collimator to SCB  Messages  *************************/

	/***********************************************/
	//     1. ASR1 Board Model Message (0x700)      /
	/***********************************************/
	ASR1ModelMsgPS::ASR1ModelMsgPS()
	{
		data.id = 0x700;
	}

	String ^ ASR1ModelMsgPS::MODEL::get()
	{
		return model;
	}

	void ASR1ModelMsgPS::MODEL::set(String ^ value)
	{
		model = value;
		array<wchar_t> ^ tempStrArray;
		tempStrArray = value->ToCharArray();
		data.Byte0 = tempStrArray[0];
		data.Byte1 = tempStrArray[1];
		data.Byte2 = tempStrArray[2];
		data.Byte3 = tempStrArray[3];
		data.Byte4 = tempStrArray[4];
		data.Byte5 = tempStrArray[5];
		data.Byte6 = tempStrArray[6];
		data.Byte7 = tempStrArray[7];
	}

	AxisCtlProtoStuct ASR1ModelMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 8;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.Byte0;
		frame.frameData[1] = data.Byte1;
		frame.frameData[2] = data.Byte2;
		frame.frameData[3] = data.Byte3;
		frame.frameData[4] = data.Byte4;
		frame.frameData[5] = data.Byte5;
		frame.frameData[6] = data.Byte6;
		frame.frameData[7] = data.Byte7;

		return frame;
	}


	/***********************************************/
	//     2. ASR2 Board Model Message (0x7A6)      /
	/***********************************************/
	ASR2ModelMsgPS::ASR2ModelMsgPS()
	{
		data.id = 0x7A6;
	}

	String ^ ASR2ModelMsgPS::MODEL::get()
	{
		return model;
	}

	void ASR2ModelMsgPS::MODEL::set(String ^ value)
	{
		model = value;
		array<wchar_t> ^ tempStrArray;
		tempStrArray = value->ToCharArray();
		data.Byte0 = tempStrArray[0];
		data.Byte1 = tempStrArray[1];
		data.Byte2 = tempStrArray[2];
		data.Byte3 = tempStrArray[3];
		data.Byte4 = tempStrArray[4];
		data.Byte5 = tempStrArray[5];
		data.Byte6 = tempStrArray[6];
		data.Byte7 = tempStrArray[7];
	}

	AxisCtlProtoStuct ASR2ModelMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 8;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.Byte0;
		frame.frameData[1] = data.Byte1;
		frame.frameData[2] = data.Byte2;
		frame.frameData[3] = data.Byte3;
		frame.frameData[4] = data.Byte4;
		frame.frameData[5] = data.Byte5;
		frame.frameData[6] = data.Byte6;
		frame.frameData[7] = data.Byte7;

		return frame;
	}


	/***********************************************/
	//     3. ASR1 Board Serial Message (0x703)     /
	/***********************************************/
	ASR1SNMsgPS::ASR1SNMsgPS()
	{
		data.id = 0x703;
	}

	String ^ ASR1SNMsgPS::SN::get()
	{
		return sn;
	}

	void ASR1SNMsgPS::SN::set(String ^ value)
	{
		sn = value;
		array<wchar_t> ^ tempStrArray;
		tempStrArray = value->ToCharArray();
		data.Byte0 = tempStrArray[0];
		data.Byte1 = tempStrArray[1];
		data.Byte2 = tempStrArray[2];
		data.Byte3 = tempStrArray[3];
		data.Byte4 = tempStrArray[4];
		data.Byte5 = tempStrArray[5];
		data.Byte6 = tempStrArray[6];
		data.Byte7 = tempStrArray[7];
	}

	AxisCtlProtoStuct ASR1SNMsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 8;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.Byte0;
		frame.frameData[1] = data.Byte1;
		frame.frameData[2] = data.Byte2;
		frame.frameData[3] = data.Byte3;
		frame.frameData[4] = data.Byte4;
		frame.frameData[5] = data.Byte5;
		frame.frameData[6] = data.Byte6;
		frame.frameData[7] = data.Byte7;

		return frame;
	}

	/***********************************************/
	//     4. ASR2 Board Status Message 1 (0x7F0)      /
	/***********************************************/
	ASR2Status1MsgPS::ASR2Status1MsgPS()
	{
		data.id = 0x7F0;
		mode = CollimatorMode::Auto;
		vsid = 0;
		crossSize = longSize = 0;
	}

	CollimatorMode ASR2Status1MsgPS::Mode::get()
	{
		return mode;
	}

	void ASR2Status1MsgPS::Mode::set(CollimatorMode value)
	{
		mode = value;
		if (value == CollimatorMode::Manual)
		{
			data.Mode_Lamp |= 0x01;
		}
		else
		{
			data.Mode_Lamp &= 0xFE;
		}
	}

	bool ASR2Status1MsgPS::LAMP_ON::get()
	{
		return lampOn;
	}

	void ASR2Status1MsgPS::LAMP_ON::set(bool value)
	{
		lampOn = value;
		if (value)
		{
			data.Mode_Lamp |= 0x40;
		}
		else
		{
			data.Mode_Lamp &= 0xBF;
		}
	}

	int ASR2Status1MsgPS::V_SID::get()
	{
		return vsid;
	}

	void ASR2Status1MsgPS::V_SID::set(int value)
	{
		vsid = value;
		data.VSID = value;
	}

	int ASR2Status1MsgPS::CROSS_SIZE::get()
	{
		return crossSize;
	}

	void ASR2Status1MsgPS::CROSS_SIZE::set(int value)
	{
		crossSize = value;
		data.VCross = value;
	}

	int ASR2Status1MsgPS::LONG_SIZE::get()
	{
		return longSize;
	}

	void ASR2Status1MsgPS::LONG_SIZE::set(int value)
	{
		longSize = value;
		data.VLong = value;
	}

	AxisCtlProtoStuct ASR2Status1MsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 8;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.Mode_Lamp;
		frame.frameData[1] = data.Byte1;
		frame.frameData[2] = data.Byte2;
		frame.frameData[3] = data.VSID;
		frame.frameData[4] = data.VCrossM;
		frame.frameData[5] = data.VCrossL;
		frame.frameData[6] = data.VLongM;
		frame.frameData[7] = data.VLongL;

		return frame;
	}

	/***********************************************/
	//     5. ASR2 Board Status Message 2 (0x7F1)      /
	/***********************************************/

	ASR2Status2MsgPS::ASR2Status2MsgPS()
	{
		data.id = 0x7F1;
	}

	Byte ASR2Status2MsgPS::FILTER_TYPE::get()
	{
		return filterType;
	}

	void ASR2Status2MsgPS::FILTER_TYPE::set(Byte value)
	{
		filterType = value;
		data.Filter = value;
	}

	int ASR2Status2MsgPS::L_SID::get()
	{
		return lsid;
	}

	void ASR2Status2MsgPS::L_SID::set(int value)
	{
		lsid = value;
		data.LSid = (WORD)value;
	}

	AxisCtlProtoStuct ASR2Status2MsgPS::GetFrame()
	{
		AxisCtlProtoStuct frame;
		frame.id = data.id;
		frame.size = 8;
		frame.frameData = gcnew array<Byte>(frame.size);
		frame.frameData[0] = data.Byte0;
		frame.frameData[1] = data.Byte1;
		frame.frameData[2] = data.Byte2;
		frame.frameData[3] = data.Filter;
		frame.frameData[4] = data.LSidM;
		frame.frameData[5] = data.LSidL;
		frame.frameData[6] = data.Byte6;
		frame.frameData[7] = data.Byte7;
		return frame;
	}


	/*************************  SCB to Collimator  Messages  *************************/
	/***********************************************/
	//     1. ASR2 Board Control Message (0x7A0)      /
	/***********************************************/

	ASR2CtlMsgPS::ASR2CtlMsgPS(AxisCtlProtoStuct frame)
	{
		data.id = frame.id;
		data.Byte0 = frame.frameData[0];
		data.Byte1 = frame.frameData[1];
		data.Byte2 = frame.frameData[2];
		data.VSid = frame.frameData[3];
		data.CrossSizeM = frame.frameData[4];
		data.CrossSizeL = frame.frameData[5];
		data.LongSizeM = frame.frameData[6];
		data.LongSizeL = frame.frameData[7];

		if ((data.Byte0 & 0x80) != 0)
		{
			mode = CollimatorMode::Manual;
		}

		if ((data.Byte0 & 0x40) != 0)
		{
			mode = CollimatorMode::Auto;
		}

		if ((data.Byte1 & 0x01) != 0)
		{
			lampOn = true;
		}

		vSid = data.VSid;

		crossSize = data.CrossSize;
		longSize = data.LongSize;
	}

	CollimatorMode ASR2CtlMsgPS::MODE::get()
	{
		return mode;
	}

	bool ASR2CtlMsgPS::LAMP_ON::get()
	{
		return lampOn;
	}

	int ASR2CtlMsgPS::V_SID::get()
	{
		return vSid;
	}

	int ASR2CtlMsgPS::CROSS_SIZE::get()
	{
		return crossSize;
	}

	int ASR2CtlMsgPS::LONG_SIZE::get()
	{
		return longSize;
	}


	/***********************************************/
	//     2. ASR2 Board Control Message 2 (0x702)    /
	/***********************************************/

	///   2. ASR2 Board Control Message 2 (0x702) 
	ASR2Ctl2MsgPS::ASR2Ctl2MsgPS(AxisCtlProtoStuct frame)
	{
		data.id = frame.id;
		for (int i = 0; i < frame.size; i++)
		{
			*(&data.CMD + i) = frame.frameData[i];
		}
		if (data.CMD == 0x01)
		{
			isLampCycle = true;
			Byte tempByte = data.Byte1;
			data.Byte1 = data.Byte2;
			data.Byte2 = tempByte;
			lampTime = data.LampTime * 0.01;
			rampTime = data.RampTime * 0.1;
		}
		else
		{
			isLampCycle = false;
			lampTime = 0;
			rampTime = 0;
		}
		if (data.CMD == 0x02)
		{
			laserOn = true;
		}
		else
		{
			laserOn = false;
		}
		if (data.CMD == 0x03)
		{
			laserOff = true;
		}
		else
		{
			laserOff = false;
		}
		if (data.CMD == 0x04)
		{
			fanOn = true;
		}
		else
		{
			fanOn = false;
		}
		if (data.CMD == 0x05)
		{
			fanOff = true;
		}
		else
		{
			fanOff = false;
		}
		if (data.CMD == 0xFF)
		{
			lampAbort = true;
		}
		else
		{
			lampAbort = false;
		}
	}

	bool ASR2Ctl2MsgPS::IS_LAMPCYCLE::get()
	{
		return isLampCycle;
	}

	double ASR2Ctl2MsgPS::LAMP_TIME::get()
	{
		return lampTime;
	}

	double ASR2Ctl2MsgPS::RAMP_TIME::get()
	{
		return rampTime;
	}

	bool ASR2Ctl2MsgPS::LASER_ON::get()
	{
		return laserOn;
	}

	bool ASR2Ctl2MsgPS::LASER_OFF::get()
	{
		return laserOff;
	}

	bool ASR2Ctl2MsgPS::FAN_ON::get()
	{
		return fanOn;
	}
	bool ASR2Ctl2MsgPS::FAN_OFF::get()
	{
		return fanOff;
	}

	bool ASR2Ctl2MsgPS::LAMP_ABORT::get()
	{
		return lampAbort;
	}
}