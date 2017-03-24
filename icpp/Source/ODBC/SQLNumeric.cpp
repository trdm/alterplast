
#include "stdafx.h"
#include "SQLNumeric.h"

#define SQLNUM_LONG_UINT_LEN    (SQL_MAX_NUMERIC_LEN / 4)
#define SQLNUM_DYN_BUFFER_LEN   6 //максимальная длина буфера при максимальной precision 38

void CSQLNumeric::Load(const SQL_NUMERIC_STRUCT *pnumeric)
{
    ASSERT(pnumeric->scale <= 9);

    if (m_pBuffer != m_Buffer)
    {
        free(m_pBuffer);
        m_pBuffer = m_Buffer;
        m_nBufferLen = 4;
    };

    m_ScaleFactor = 1000000000;

    if (pnumeric->precision == 0)
    {
        m_nUsedLen = 0;
        m_nScaleLen = 0;
        m_Sign = 1;
        return;
    };

    if (pnumeric->sign)
        m_Sign = 1;
    else
        m_Sign = -1;

    UINT* pDData = m_Buffer;
    UINT Buf1[SQLNUM_LONG_UINT_LEN];
    UINT Buf2[SQLNUM_LONG_UINT_LEN];

    UINT* pSData;
    UINT* pDivRes;

    pDivRes = Buf1;
    pDivRes += SQLNUM_LONG_UINT_LEN - 1;

    UINT nNextIteration = SQLNUM_LONG_UINT_LEN;

    if (pnumeric->scale)
    {
        pSData = (UINT*) pnumeric->val;
        pSData += SQLNUM_LONG_UINT_LEN - 1;

        __asm
        {
            mov     esi, pnumeric
            mov     al, [esi]pnumeric.scale
            cbw
            cwde
            mov     ecx, eax
            mov     eax, 1
            mov     ebx, 10
l3:
            mul     ebx
            loop    l3
            mov     ebx, eax

            mov     esi, pSData
            mov     edi, pDivRes
            xor     edx, edx
            mov     ecx, nNextIteration
            mov     nNextIteration, 0
PartDivScale:
            mov     eax, [esi]
            div     ebx
            mov     [edi], eax
            test    eax, eax
            jz      l0
            mov     eax, nNextIteration
            test    eax, eax
            jnz     l0
            mov     nNextIteration, ecx
            mov     pDivRes, edi
l0:
            sub     esi, 4
            sub     edi, 4
            loop    PartDivScale

            mov     ecx, edx
            mov     eax, 1000000000
            xor     edx, edx
            div     ebx
            mul     ecx
            mov     edi, pDData
            mov     [edi], eax
            add     pDData, 4
        };
        m_nScaleLen = 1;

        pSData = pDivRes;
        pDivRes = Buf2;
        pDivRes += nNextIteration - 1;
    }
    else
    {
        memcpy(Buf2, pnumeric->val, SQL_MAX_NUMERIC_LEN);
        pSData = Buf2;
        pSData += SQLNUM_LONG_UINT_LEN - 1;
    }

    UINT* pBuffer_end = m_Buffer + 4;

    __asm
    {
        mov     ebx, 1000000000
        mov     ecx, nNextIteration
        test    ecx, ecx
        jnz     lIterate
        mov     edi, pDData
        mov     dword ptr [edi], 0
        add     pDData, 4
        jmp     End
lIterate:
        mov     nNextIteration, 0
        mov     esi, pSData
        mov     edi, pDivRes
        xor     edx, edx
PartDivCeil:
        mov     eax, [esi]
        div     ebx
        mov     [edi], eax
        test    eax, eax
        jz      l1
        mov     eax, nNextIteration
        test    eax, eax
        jnz     l1
        mov     nNextIteration, ecx
        mov     pSData, esi
        mov     pDivRes, edi
l1:
        sub     esi, 4
        sub     edi, 4
        loop    PartDivCeil
        mov     edi, pDData

        cmp     edi, pBuffer_end
        jne     l4
        //перестроимся на внешний буфер
        push    edx
        mov     eax, SQLNUM_DYN_BUFFER_LEN * 4
        push    eax
        call    dword ptr [malloc]
        add     esp, 4
        mov     edi, this
        mov     [edi].m_pBuffer, eax
        mov     [edi].m_nBufferLen, SQLNUM_DYN_BUFFER_LEN

        lea     edx, [edi].m_Buffer
        mov     edi, eax
        mov     ecx, 4
l5:
        mov     eax, [edx]
        mov     [edi], eax
        add     edx, 4
        add     edi, 4
        loop    l5
        
        mov     pDData, edi
        pop     edx
        //
l4:
        mov     [edi], edx
        add     pDData, 4
        mov     eax, pSData
        xchg    pDivRes, eax
        mov     pSData, eax
        mov     ecx, nNextIteration
        test    ecx, ecx
        jnz     lIterate
End:
    };
    m_nUsedLen = pDData - m_pBuffer;
}

void CSQLNumeric::Save(SQL_NUMERIC_STRUCT *pnumeric) const
{
    ASSERT(FALSE);
    //А оно Вам надо?
}
