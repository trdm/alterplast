// This is part of the Professional User Interface Suite library.
// Copyright (C) 2001-2003 FOSS Software, Inc.
// All rights reserved.
//
// http://www.prof-uis.com
// http://www.fossware.com
// mailto:foss@fossware.com
//
// This source code can be used, modified and redistributed
// under the terms of the license agreement that is included
// in the Professional User Interface Suite package.
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#if (!defined __PROF_UIS_H)
#define __PROF_UIS_H

#if (!defined __EXT_MFC_DEF_H)
	#include "ExtMfcDef.h"
#endif // __EXT_MFC_DEF_H

#ifndef __AFXWIN_H__
	#include <AfxWin.h>         // MFC core and standard components
#endif

#ifndef __AFXEXT_H__
	#include <afxext.h>         // MFC extensions
#endif
//#if (!defined __AFXPRIV_H__)
//	#include <AfxPriv.h>
//#endif
//#if _MFC_VER < 0x700
//	#include <../src/AfxImpl.h>
//#else
//	#include <../src/mfc/AfxImpl.h>
//#endif


#ifndef _AFX_NO_OLE_SUPPORT
	#ifndef __AFXOLE_H__
		#include <AfxOle.h>     // MFC OLE classes
	#endif
	#ifndef __AFXODLGS_H__
		#include <AfxOdlgs.h>   // MFC OLE dialog classes
	#endif
	#ifndef __AFXDISP_H__
		#include <AfxDisp.h>    // MFC Automation classes
	#endif
#endif // _AFX_NO_OLE_SUPPORT

#ifndef __AFXTEMPL_H__
	#include <AfxTempl.h>
#endif

#ifndef __AFXMT_H__
	#include <AfxMT.h>
#endif

#ifndef __AFXCMN_H__
	#include <AfxCmn.h>
#endif

#ifndef __AFXDTCTL_H__
	#include <AfxDtCtl.h>
#endif

#ifndef __AFXCONV_H__
	#include <AfxConv.h>
#endif

#define __EXT_MFC_NO_RTF_TOOLTIPS
#define __EXT_MFC_NO_TAB_CTRL
#define __EXT_MFC_NO_TABMDI_CTRL
#define __EXT_MFC_NO_TAB_CONTROLBARS
#define __EXT_MFC_NO_TOOLBOX_CTRL
#define __EXT_MFC_NO_SHORTCUTLIST_CTRL
#define __EXT_MFC_NO_PAGECONTAINER_CTRL
#define __EXT_MFC_NO_SCROLLWND
#define __EXT_MFC_NO_IMAGEEDITWND
#define __EXT_MFC_NO_COLORPALETTE
#define __EXT_MFC_NO_CUSTOMIZE

#if (!defined __PROF_UIS_IMPL__)

	#if (!defined __EXTBTNONFLAT_H)
		#include "ExtBtnOnFlat.h"
	#endif

	#if (!defined __EXT_BUTTON_H)
		#include "ExtButton.h"
	#endif

	#if (!defined __EXT_COMBO_BOX_H)
		#include "ExtComboBox.h"
	#endif

	#if (!defined __EXT_EDIT_H)
		#include "ExtEdit.h"
	#endif

	#if (!defined __EXT_CMD_ICON_H)
		#include "ExtCmdIcon.h"
	#endif

	#if (!defined __ExtCmdManager_H)
		#include "ExtCmdManager.h"
	#endif

	#if (!defined __EXT_PAINT_MANAGER_H)
		#include "ExtPaintManager.h"
	#endif

	#if (!defined __EXT_CONTROLBAR_H)
		#include "ExtControlBar.h"
	#endif

	#if (!defined __EXT_TOOLCONTROLBAR_H)
		#include "ExtToolControlBar.h"
	#endif

	#if (!defined __EXT_MENUCONTROLBAR_H)
		#include "ExtMenuControlBar.h"
	#endif

	#if (!defined __EXT_STATUSCONTROLBAR_H)
		#include "ExtStatusControlBar.h"
	#endif

	#if (!defined __EXT_POPUP_MENU_WND_H)
		#include "ExtPopupMenuWnd.h"
	#endif

	#if (!defined __EXT_RESIZABLE_DIALOG_H)
		#include "ExtResizableDialog.h"
	#endif

	#if (!defined __EXTCOLORCTRL_H)
		#include "ExtColorCtrl.h"
	#endif

	#if (!defined __EXTCOLORDLG_H)
		#include "ExtColorDlg.h"
	#endif

	#if (!defined __EXT_TEMPL_H)
		#include "ExtTempl.h"
	#endif

#endif // !__PROF_UIS_IMPL__

#endif // __PROF_UIS_H


