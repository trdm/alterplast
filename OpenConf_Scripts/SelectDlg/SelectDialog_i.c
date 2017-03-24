/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Dec 21 20:21:30 2005
 */
/* Compiler settings for D:\Leha\OpenConf_Scripts\—истемные‘айлы\SelectDlg\SelectDialog.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_ISelection = {0xFD0634F7,0x4A92,0x488E,{0xA8,0x7B,0x85,0x03,0x5E,0xFF,0x67,0x2C}};


const IID LIBID_SELECTDIALOGLib = {0xD8E7C9B3,0xAB26,0x4114,{0x91,0xF8,0xAF,0xAA,0xAA,0xEC,0xC0,0x7A}};


const IID DIID__ISelectionEvents = {0xD6599353,0x3CE3,0x47AE,{0x83,0x73,0x07,0x5C,0x24,0xFD,0xAC,0xD3}};


const CLSID CLSID_Selection = {0x517A7574,0xB37D,0x4BA0,{0xA9,0xCC,0x58,0x6D,0x95,0x70,0x35,0x61}};


#ifdef __cplusplus
}
#endif

