// Initializer.h: interface for the CInitializer class.
#ifndef INITIALIZER_H
#define INITIALIZER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef void(*PINITFUNC)();
class CInitializer  
{
public:
	CInitializer();
	static void InitDLLMain()			{Init(m_InitOnDLLMain);}
	static void InitOnCreateMainWnd()	{Init(m_InitOnCreateMainWnd);}
	static void InitOnCreateConfigWnd()	{Init(m_InitOnCreateConfigWnd);}
	static void Done()					{Init(m_Done);}
protected:
	static void Init(PINITFUNC* ppArrayOfFunc);
	static PINITFUNC m_InitOnDLLMain[];
	static PINITFUNC m_InitOnCreateMainWnd[];
	static PINITFUNC m_InitOnCreateConfigWnd[];
	static PINITFUNC m_Done[];
};

#endif
