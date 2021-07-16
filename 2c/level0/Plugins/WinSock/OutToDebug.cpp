// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
#include "stdafx.h"
#include "OutToDebug.h"
#include "STDIO.H"
#define FILE_NAME "c:\\debug.txt"
//#define OutToDebug TODEBUG
#define BEGIN_OUT_DEBUG(x)\
{\
    FILE *sFile;\
    if(sFile=fopen(FILE_NAME,"a+"))\
    {

#define END_OUT_DEBUG(x)\
        fclose(sFile);\
    }}

void OutToDebug(LPCTSTR fmt,...)
BEGIN_OUT_DEBUG(0)
 va_list lst;
 va_start(lst,fmt);
 vfprintf(sFile,fmt,lst);
 va_end(lst);
END_OUT_DEBUG(0)


UINT GetProcessorTime(UINT nDiv)
{
        UINT TikCount;
        UINT TikCountLo;
        UINT TikCountHi;
        __asm                                   
        {                                               
                RDTSC
                MOV     TikCountLo,EAX          
                MOV     TikCountHi,EDX          
        }       
        TikCount=(TikCountHi<<(32-nDiv))+(TikCountLo>>nDiv);
        return TikCount;

}


