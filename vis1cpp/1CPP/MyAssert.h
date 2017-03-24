// MyAssert.h: interface for MyAssert
//
//	Автор: Артур Аюханов aka artbear, 2008
//
//	Позволяет работать с обычными assert, MFC-ASSERT, ATLASSERT и т.д. при использовании в разработке Visual С++ 6.0 и 1С
//	без этого файла ASSERT, VERIFY и т.д. из MFC и ATL не работают
//
//	ВНИМАНИЕ: 
//	1. для правильного использования нужно в самом начале StdAfx.h ставить #include "1CPP\\MyAssert.h"
//	даже до #include системных файлов
//	Иначе не будут работать Assert-ы в системных объектах
//
//	2. В настройках debug-сборки необходимо убрать/отключить переменную препроцессора NDEBUG
//		В настройках release-сборки ее нужно включить
//
//////////////////////////////////////////////////////////////////////

#include <afx.h>

#include <assert.h>

#ifdef NDEBUG
#else

	#undef ASSERT
	#undef VERIFY
	#undef ASSERT_VALID
	#undef DEBUG_ONLY

	#undef ATLASSERT

	#define ASSERT(exp) ( assert(exp) )

	#define VERIFY(f)          ASSERT(f)
	#define ASSERT_VALID(pOb)  ASSERT(pOb != NULL)
	#define DEBUG_ONLY(f)      (f)

	#define ATLASSERT(exp) ( assert(exp) )

#endif  /* NDEBUG */