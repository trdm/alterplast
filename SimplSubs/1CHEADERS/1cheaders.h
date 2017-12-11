// 1cheaders.h
/*
Данный файл предназначен для его включения в stdafx.h разнообразных проектов,
нуждающихся в доступе к 1С dll'кам.
*/
#ifndef _1C_HEADERS_H
#define _1C_HEADERS_H

#define IMPORT_1C __declspec(dllimport)

#include <afxcview.h> 
#include <comdef.h>
#include <afxctl.h>
#include <afxtempl.h>
#include <functional>
#include <set>
#include <vector>
#include <map>

// Добавим хедеры 1С классов
#include "type32.h"
#include "types.h"
#include "bkend.h"
#include "br32.h"
#include "frame.h"
#include "blang.h"
#include "seven.h"
#include "editr.h"
#include "basic.h"
#include "dbeng32.h"
#include "moxel.h"
#include "rgproc.h"
#include "txtedt.h"
#include "userdef.h"

using namespace std;

// Объявим несколько полезных глобальных объектов
extern CApp7*			pMainApp;
extern CBkEndUI*		pBkEndUI;
extern CMetaDataCont*	pMetaDataCont;
extern HINSTANCE		h1CResource;
extern SECMDIFrameWnd*	pMainFrame;

#endif