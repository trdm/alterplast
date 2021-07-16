/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Jun 02 22:09:21 2004
 */
/* Compiler settings for C:\Visual C++\2C\Enterprise\PLUGINS\ServerCOM\ServerCOM.idl:
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

const IID IID_IConnect = {0x03FD46F1,0x4FEE,0x4B0B,{0x8B,0x40,0x1E,0x94,0x36,0x52,0xCA,0x08}};


const IID LIBID_SERVERCOMLib = {0xF1598F54,0xBF00,0x4950,{0xA1,0x1B,0xC4,0x7E,0xC4,0x9C,0x82,0x4F}};


const CLSID CLSID_Connect = {0x0497D803,0xC0F6,0x4E37,{0x92,0x47,0x6F,0xC0,0x08,0x0D,0x10,0x96}};


#ifdef __cplusplus
}
#endif

