// the zero-based byte could use this mask to set the board type
#define DMCBoardType 0x00
#define TMCBoardType 0x10
#define InvalidType  0x70

// Board Switch Number defination
// OTC Controller Type


#define OTCTYPE1 0x00  //Basic OTC Controller without Z motor option
#define OTCTYPE2 0x08  //Basic OTC Controller with Z motor option
#define OTCTYPE3 0x04  //Premium OTC XYZ Controller
#define OTCTYPE4 0x05  //Premium OTC AB Controller

// Basic WSD Controller Type
#define BSCWSDTYPE1 0x02 //Basic WSD Controller, no Tilt, No Bucky rotation
#define BSCWSDTYPE2 0x0A //Basic WSD Controller, Tilt, No Bucky rotation
#define BSCWSDTYPE3 0x12 //Basic WSD Controller, no Tilt, Bucky rotation
#define BSCWSDTYPE4 0x1A //Basic WSD Controller, Tilt, Bucky rotation

// BasiQ WSD Controller Type
#define BSQWSDTYPE1 0x42
#define BSQWSDTYPE2 0x4A
#define BSQWSDTYPE3 0x52
#define BSQWSDTYPE4 0x5A

// Full/Premium WSD Controller Type
#define FULWSDTYPE1 0x06
#define FULWSDTYPE2 0x0E

// Premium WSD Controller Type
#define PREWSDTYPE1 0x16
#define PREWSDTYPE2 0x1E
#define PREWSDTYPE3 0x36
#define PREWSDTYPE4 0x3E

// TBL Controller Type
#define TBLTYPE1 0x03
#define TBLTYPE2 0x43
#define TBLTYPE3 0x0F
#define TBLTYPE4 0x07
#define TBLTYPE5 0x17

// Reservered Type
#define RESVTYPE 0xFF


// the controller file transfer error code definition
#define FILETRANSFERCOMPLETED 0
#define FILETRANSFERACCEPTED 1
#define VERSIONNOTMATCH 101
#define MODECHANGEREJECT 102
#define FILEOPENERROR 103
#define BADFILEFORM 104
#define BADFILECHECKSUM 105
#define BADTRANSFERPATH 106
#define INVALIDTRANSFERTYPE 107
#define CANPACKETLENGTHERROR 108
#define INVALIDFORAPP 109
#define INVALIDFORBOOT 110
#define ZEROSIZEERROR 111
#define SIZETOOLARGE 112
#define TRANSFERINPROGRESS 113
#define TOOFEWBYTESFORCOMPLETE 114
#define TOOMANYBYTESFORCOMPLETE 115
#define CONFIGNOTMATCH 116
#define FLASHFAIL 117
#define NOTTRANSFERINPROGRESS 118
#define BLOCKTARGETINVALID 119
#define BLOCKSEQUNEXPECTED 120
#define BLOCKTOOMANYBYTES 121
#define BLOCKCHECKSUMFAIL 122
#define FILENOTSRED 123
#define EMBEDTYPENOTMATCH 124
#define EMBEDTARGETNOTMATCH 125
#define EMBEDSWCODENOTMATCH 126
#define SREDSIZENOTMATCH 127
#define FILEPARAERROR 128
