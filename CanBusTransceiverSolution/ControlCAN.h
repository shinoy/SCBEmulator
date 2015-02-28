//ControlCAN.h@DSP Studio
//文件版本：v1.0.2.1

#if !defined(__CONTROL__CAN__H__)
#define __CONTROL__CAN__H__

//接口卡类型定义
#define VCI_USBCAN1		3
#define VCI_USBCAN2		4

//函数调用返回状态值
#define	STATUS_OK			1	//操作成功
#define STATUS_ERR			0	//操作失败
#define STATUS_ERR2			-1	//设备未打开

/*------------兼容ZLG的函数及数据类型---------------------------------*/

//USB-CAN总线适配器/CANalyst-II分析仪等，板卡信息的数据类型。
typedef  struct  _VCI_BOARD_INFO{
	USHORT	hw_Version;
	USHORT	fw_Version;
	USHORT	dr_Version;
	USHORT	in_Version;
	USHORT	irq_Num;
	BYTE	can_Num;
	CHAR	str_Serial_Num[20];
	CHAR	str_hw_Type[40];
	USHORT	Reserved[4];
} VCI_BOARD_INFO,*PVCI_BOARD_INFO;


//定义CAN信息帧的数据类型。
typedef  struct  _VCI_CAN_OBJ{
	UINT	ID;
	UINT	TimeStamp;	 			//时间标识
	BYTE	TimeFlag;				//是否使用时间标识
	BYTE	SendType;				//发送标志。保留，未用
	BYTE	RemoteFlag;				//是否是远程帧
	BYTE	ExternFlag;				//是否是扩展帧
	BYTE	DataLen;
	BYTE	Data[8];
	BYTE	Reserved[3];			//保留
}VCI_CAN_OBJ,*PVCI_CAN_OBJ;

//定义CAN控制器状态的数据类型。
typedef struct _VCI_CAN_STATUS{
	UCHAR	ErrInterrupt;
	UCHAR	regMode;
	UCHAR	regStatus;
	UCHAR	regALCapture;
	UCHAR	regECCapture; 
	UCHAR	regEWLimit;
	UCHAR	regRECounter; 
	UCHAR	regTECounter;
	DWORD	Reserved;
}VCI_CAN_STATUS,*PVCI_CAN_STATUS;

//定义错误信息的数据类型。
typedef struct _ERR_INFO{
	UINT	ErrCode;
	BYTE	Passive_ErrData[3];
	BYTE	ArLost_ErrData;
} VCI_ERR_INFO,*PVCI_ERR_INFO;

//定义初始化CAN的数据类型
typedef struct _INIT_CONFIG{
	DWORD	AccCode;
	DWORD	AccMask;
	DWORD	Reserved;
	UCHAR	Filter;			//滤波方式：0,1——接收所有帧。2——标准帧滤波，3——扩展帧滤波。
	UCHAR	Timing0;
	UCHAR	Timing1;
	UCHAR	Mode;			//工作模式：0——正常模式，1——只听模式,2——自测模式
}VCI_INIT_CONFIG,*PVCI_INIT_CONFIG;

DWORD __stdcall VCI_OpenDevice(DWORD DevType,DWORD DevIndex,DWORD Reserved);
DWORD __stdcall VCI_CloseDevice(DWORD DevType,DWORD DevIndex);
DWORD __stdcall VCI_InitCAN(DWORD DevType, DWORD DevIndex, DWORD CANIndex, PVCI_INIT_CONFIG pInitConfig);

DWORD __stdcall VCI_ReadBoardInfo(DWORD DevType,DWORD DevIndex,PVCI_BOARD_INFO pInfo);
DWORD __stdcall VCI_ReadErrInfo(DWORD DevType,DWORD DevIndex,DWORD CANIndex,PVCI_ERR_INFO pErrInfo);
DWORD __stdcall VCI_ReadCANStatus(DWORD DevType,DWORD DevIndex,DWORD CANIndex,PVCI_CAN_STATUS pCANStatus);

DWORD __stdcall VCI_GetReference(DWORD DevType,DWORD DevIndex,DWORD CANIndex,DWORD Reserved,PVOID pData);
DWORD __stdcall VCI_SetReference(DWORD DevType,DWORD DevIndex,DWORD CANIndex,DWORD RefType,PVOID pData);

DWORD __stdcall VCI_GetReceiveNum(DWORD DevType,DWORD DevIndex,DWORD CANIndex);
DWORD __stdcall VCI_ClearBuffer(DWORD DevType,DWORD DevIndex,DWORD CANIndex);

DWORD __stdcall VCI_StartCAN(DWORD DevType,DWORD DevIndex,DWORD CANIndex);
DWORD __stdcall VCI_ResetCAN(DWORD DevType,DWORD DevIndex,DWORD CANIndex);

DWORD __stdcall VCI_Receive(DWORD DevType,DWORD DevIndex,DWORD CANIndex,PVCI_CAN_OBJ pReceive,ULONG Len,INT WaitTime);
DWORD __stdcall VCI_Transmit(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_CAN_OBJ pSend,DWORD Length);

/*------------其他函数及数据结构描述---------------------------------*/

//USB-CAN总线适配器板卡信息的数据类型1，该类型为VCI_FindUsbDevice函数的返回参数。
typedef  struct  _VCI_BOARD_INFO1{
	USHORT	hw_Version;
	USHORT	fw_Version;
	USHORT	dr_Version;
	USHORT	in_Version;
	USHORT	irq_Num;
	BYTE	can_Num;
	BYTE	Reserved;
	CHAR	str_Serial_Num[8];
	CHAR	str_hw_Type[16];
	CHAR	str_Usb_Serial[4][4];
} VCI_BOARD_INFO1,*PVCI_BOARD_INFO1;

DWORD __stdcall VCI_GetReference2(DWORD DevType,DWORD DevIndex,DWORD CANIndex,DWORD Reserved,BYTE *pData);
DWORD __stdcall VCI_SetReference2(DWORD DevType,DWORD DevIndex,DWORD CANIndex,DWORD RefType,BYTE *pData);
DWORD __stdcall VCI_ResumeConfig(DWORD DevType,DWORD DevIndex,DWORD CANIndex);

DWORD __stdcall VCI_ConnectDevice(DWORD DevType,DWORD DevIndex);
DWORD __stdcall VCI_UsbDeviceReset(DWORD DevType,DWORD DevIndex,DWORD Reserved);
DWORD __stdcall VCI_FindUsbDevice(PVCI_BOARD_INFO1 pInfo);

#endif //#define __CONTROL__CAN__H__