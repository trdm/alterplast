/*
fix_basic.inl
(c) Александр Орефков
Файл для решения проблемы при необходимости статической линковки
с basic.dll в модулях, которые должны подгружаться при самом старте 1Ски.
	Суть проблемы:
basic.dll в своей процедуре DllMain при загрузке выполняет действия,
рассчитанные на то, что CWinApp в 1cv7.exe уже проиницализирован,
а при выгрузке - на то, что CWinApp в 1cv7.exe еще проиницализирован.
При статической линковке разрабатываемого модуля с basic.dll, когда
модуль пытается загрузится, DllMain в basic.dll возвращает 0,
тк CWinApp еще не проинициализирован, поэтому модуль не может загрузится.

	Решение - отложенная линковка, реализуемая линкером VC++, и данным кодом в
разрабатываемом модуле. Код логичнее всего подключать как

#include "fix_basic.inl"

в stdafx.cpp

	Возможные проблемы:
Таблица импорта из basic.dll инициализируется при первом вызове любой
функции из basic.dll. Если первым в программе будет обращение не к функции,
а к импортируемым данным, то в этом случае код инициализации таблицы импорта
не будет выполнен, а обращение к данным будет неверным.
	Функция-загрузчик жестко прошита на привязку к basic.dll. Если вам
необходимо осуществлять отложенную линковку других библиотек, не используйте
эту реализацию. Примените стандартную, либо напишите свою.
Основное отличие этой реализации - при первом вызове для данной длл
инициализируется вся таблица импорта, а не один адрес. Для того чтобы
корректно выполнялось обращение к импортируемым из длл данным.
Кроме того, вместо LoadLibrary применяется GetModuleHandle, для того,
чтобы не увеличивать счетчик загрузок длл, иначе она не будет выгружаться
вовремя, что приводит к падениям при выходе из программы. Таким образом,
не получится обращаться к методам basic.dll до того, как она будет
загружена самой 1С.
*/

#include <delayimp.h>

#pragma comment(linker, "/delayload:basic.dll")

// Функция инициализации таблицы импорта.
// Реализована в расчете на однопоточное приложение.
// Основана на оригинальном коде MS в VC98\Include\delayhlp.cpp
extern "C" FARPROC WINAPI __delayLoadHelper(PCImgDelayDescr pidd, FARPROC *ppfnIATEntry)
{
	HINSTANCE hInst = *pidd->phmod;
	if(!hInst)
	{
		// Получаем модуль
		hInst = GetModuleHandle(pidd->szName);
		if(!hInst)
		{
			MessageBox(NULL, "Попытка обращения к basic.dll до ее загрузки", 0, 0);
			return NULL;
		}
		*pidd->phmod = hInst;
		// Теперь надо заполнить всю таблицу импорта.
		// Если есть прибинденная таблица, просто скопируем ее,
		// а иначе пройдемся в цикле по всей таблице, вызывая GetProcAddress
		DWORD dwIdx = IndexFromPImgThunkData(PCImgThunkData(ppfnIATEntry), pidd->pIAT);
		DWORD dwImports = CountOfImports(pidd->pIAT);
		
		if(pidd->pBoundIAT && pidd->dwTimeStamp)
		{
			// bound imports exist...check the timestamp from the target image
			PIMAGE_NT_HEADERS pinh = PIMAGE_NT_HEADERS(PCHAR(hInst) + PIMAGE_DOS_HEADER(hInst)->e_lfanew);
			
			if(pinh->Signature == IMAGE_NT_SIGNATURE &&
				pinh->FileHeader.TimeDateStamp == pidd->dwTimeStamp &&
				DWORD(hInst) == pinh->OptionalHeader.ImageBase)
			{
				memcpy(pidd->pIAT, pidd->pBoundIAT, dwImports * sizeof IMAGE_THUNK_DATA);
				return FARPROC(pidd->pIAT[dwIdx].u1.Function);
			}
		}
		// Не удалось привязатся к прибинденной таблице.
		// Будем звать GetProcAddres
		PImgThunkData pWrite = pidd->pIAT;
		PCImgThunkData pRead = pidd->pINT;
		for(DWORD idx = 0; idx < dwImports; idx++)
		{
			pWrite->u1.Function = (PDWORD)GetProcAddress(hInst,
				(pRead->u1.Ordinal & IMAGE_ORDINAL_FLAG) ?
				(LPCSTR) pRead->u1.Ordinal :
				(LPCSTR) pRead->u1.AddressOfData->Name);
			pRead++, pWrite++;
		}
		return FARPROC(pidd->pIAT[dwIdx].u1.Function);
	}
	return NULL;
}
