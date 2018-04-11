// Begin forward declarations
class  CRealInterface;
class  CRightDoc;
class  CRightListView;
class  CSubInterface;
class  CUserInterface;
class  CWPIDoc;
class  CWPIListView;
// End forward declarations
typedef CArray<class CCommandStub,class CCommandStub &> type_3;
// Begin class descriptions

class IMPORT_1C CRealInterface:public CObject
{
DECLARE_DYNCREATE(CRealInterface)

protected:
	 CRealInterface(void);	//1
public:
	virtual  ~CRealInterface(void);	//8
	void  ActivateMenuEditor(void);	//22
	void  ActivateToolbarEditor(void);	//23
	virtual void  Create(class CWPIDoc *,char const *);	//33
protected:
	void  CutDublicate(class CMenu *,class CMenu *);	//43
	void  CutDublicate(class CUserDefInfo *,class CUserDefInfo *);	//44
	void  CutExtra(class CMenu *,class CUserCommandContaner *);	//45
	void  CutExtra(class CUserDefInfo *,class CUserCommandContaner *);	//46
public:
	int  FindID(unsigned int,int);	//57
	virtual class CUserCommandContaner *  GetContainer(void);	//60
	virtual class CString   GetDescription(void);	//65
protected:
	class CMenuDoc *  GetMenuPageDoc(void);	//77
public:
	virtual class CMenu *  GetMenuShape(void);	//78
protected:
	class CToolbarDoc *  GetTBPageDoc(void);	//102
public:
	virtual class CUsrDescriptionDoc *  GetTextDoc(void);	//103
	virtual class CUserDefInfo *  GetUserDefInfo(void);	//106
	virtual int  InitWPI(class CApp7 *);	//110
	void  InsertCommand(class CCommandInsert *);	//113
protected:
	int  IsCommandDifference(class CUserCommandContaner *,unsigned int,class CUserCommandContaner *,unsigned int);	//119
public:
	virtual int  IsDifference(class CUserInterface *);	//121
protected:
	int  IsMenuDifference(class CUserCommandContaner *,class CMenu *,class CUserCommandContaner *,class CMenu *);	//125
	int  IsTBDifference(class CUserCommandContaner *,class CUserDefInfo *,class CUserCommandContaner *,class CUserDefInfo *);	//127
	void  MergeCommands(class CUserCommandContaner *,int);	//130
public:
	virtual void  MergeContent(class CUserInterface *,int);	//131
	void  ModifyPages(int);	//136
protected:
	void  OnDelItem(class CMessage7 *,long *);	//147
public:
	virtual int  OnSaveDocument(char const *);	//172
	void  RemoveExtra(void);	//203
	void  RemoveID(unsigned int);	//204
	virtual void  Rename(char const *);	//206
	virtual void  Serialize(class CArchive &);	//216
	void  SerializePage(class CArchive &);	//218
	virtual void  SetDescription(class CString);	//223
	virtual void  Test(void);	//231
protected:
	void  TestPopUp(class CMenu *);	//234
public:
	void  UseFilter(class CMenu *,class CUIntArray *);	//238
	int  _InitWPI(class CApp7 *,class CUIntArray *);	//253

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CRightDoc:public CContainer
{
DECLARE_DYNCREATE(CRightDoc)

protected:
	 CRightDoc(void);	//2
public:
	virtual  ~CRightDoc(void);	//9
	virtual int  AnalizeErr(unsigned int);	//24
	virtual void  Commit(int,class CPropertySchema *);	//29
	virtual int  DeleteItem(int);	//47
	void  DescribeRights(class CString &);	//49
	static unsigned int  DocID;	//51
	virtual void  FillPropety(int,class CPropertySchema *);	//53
protected:
	void  GetAdmin(class CMessage7 *,long *);	//58
public:
	static class CRightDoc *  GetContainer(void);	//61
	virtual class CDocument *  GetItemDoc(int);	//75
	class CRightsContainer *  GetRightContainer(int);	//92
	class CRightItemDoc *  GetRightItem(int);	//93
	virtual int  InsertNewItem(int);	//114
protected:
	void  IsAdmin(class CMessage7 *,long *);	//117
public:
	void  Modified(int,int);	//134
protected:
	void  OnActivateRights(class CMessage7 *,long *);	//138
public:
	virtual int  OnNewDocument(void);	//162
protected:
	void  OnRights(class CMessage7 *,long *);	//171
public:
	virtual int  OnSaveDocument(char const *);	//173
protected:
	void  OnSetRights(class CMessage7 *,long *);	//176
public:
	void  OnUsrDelr(int);	//189
	void  OnUsrSetr(int);	//191
	virtual void  RenameItem(int,char const *);	//208
protected:
	virtual int  SaveModified(void);	//213
public:
	virtual unsigned int  SetData(int,class CPropertySchema *);	//219
	void  SetDescription(int,class CString);	//224
protected:
	void  UpdateModifiedFlag(int);	//235

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CRightListView:public CObject
{
DECLARE_DYNCREATE(CRightListView)

protected:
	 CRightListView(void);	//3
	virtual  ~CRightListView(void);	//10
public:
	class CRightDoc *  GetDocument(void);	//68
protected:
	virtual class CPropertySchema *  GetMySchema(void);	//88
public:
	virtual void  InsertNewItem(int,int);	//115
protected:
	int  OnCreate(struct tagCREATESTRUCTA *);	//145
	void  OnDblclk(struct tagNMHDR *,long *);	//146
	void  OnDestroy(void);	//148
	virtual void  OnEnter(void);	//152
public:
	virtual void  OnInitialUpdate(void);	//154
protected:
	void  OnRightEdt(void);	//170
	void  OnUpdateUserHelpEdit(class CCmdUI *);	//186
	void  OnUserHelpEdit(void);	//187
	void  OnUsrDelr(void);	//190
	void  OnUsrSetr(void);	//192
public:
	virtual void  ReplaceItem(int,unsigned int);	//210

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CSubInterface:public CObject
{
DECLARE_DYNCREATE(CSubInterface)

protected:
	 CSubInterface(void);	//4
public:
	virtual  ~CSubInterface(void);	//11
	void  ChangeCommandStyle(int);	//26
	virtual class CUserCommandContaner *  GetContainer(void);	//62
	virtual class CString   GetDescription(void);	//66
	virtual void  GetFilter(class type_3 &,int);	//70
	virtual class CString   GetFullName(class CString);	//72
	void  GetIntFilter(class type_3 &,class CUIntArray &,class CUserCommandContaner *);	//74
	class CString   GetMasterName(void);	//76
	virtual class CMenu *  GetMenuShape(void);	//79
	class CUserInterface *  GetParentInterface(int);	//89
	int  GetResourse(class CUIntArray *,class CBitmap *,class CUIntArray *,class CStringArray *);	//91
	virtual class CUsrDescriptionDoc *  GetTextDoc(void);	//104
	virtual class CUserDefInfo *  GetUserDefInfo(void);	//107
	virtual int  InitWPI(class CApp7 *);	//111
	int  IsAncestor(char const *);	//118
	virtual int  IsDifference(class CUserInterface *);	//122
	virtual void  MergeContent(class CUserInterface *,int);	//132
protected:
	void  OnIntrfChange(class CMessage7 *,long *);	//157
public:
	virtual int  OnSaveDocument(char const *);	//174
	virtual void  Serialize(class CArchive &);	//217
	virtual void  SetDescription(class CString);	//225
	void  SetFilter(class type_3 &);	//228
	void  SetMasterName(class CString);	//229
	virtual void  Test(void);	//232

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CUserInterface:public CObject
{
DECLARE_DYNCREATE(CUserInterface)

protected:
	 CUserInterface(void);	//5
	virtual  ~CUserInterface(void);	//12
public:
	void  CloseWindows(void);	//27
	static class CString   CommandToPresent(char const *);	//28
	virtual void  Create(class CWPIDoc *,char const *);	//34
	virtual int  DoFileSave(void);	//50
	int  GetCommandName(unsigned int,class CMenu *,class CString &);	//59
	virtual class CUserCommandContaner *  GetContainer(void);	//63
	virtual class CString   GetDescription(void);	//67
	virtual void  GetFilter(class type_3 &,int);	//71
	virtual class CString   GetFullName(class CString);	//73
	virtual class CMenu *  GetMenuShape(void);	//80
	virtual class CUsrDescriptionDoc *  GetTextDoc(void);	//105
	virtual class CUserDefInfo *  GetUserDefInfo(void);	//108
	virtual int  InitWPI(class CApp7 *);	//112
	int  IsDefault(void);	//120
	virtual int  IsDifference(class CUserInterface *);	//123
	int  IsDisableOperation(void);	//124
	int  IsRealInterface(void);	//126
	virtual void  MergeContent(class CUserInterface *,int);	//133
protected:
	virtual void  OnCloseDocument(void);	//142
	void  OnFontChange(class CMessage7 *,long *);	//153
	virtual int  OnOpenDocument(char const *);	//163
	virtual int  OnSaveDocument(char const *);	//175
public:
	static class CString   ParamToPresent(char const *);	//197
	static class CString   PresentToCommand(char const *);	//201
	static class CString   PresentToParam(char const *);	//202
	virtual void  Rename(char const *);	//207
protected:
	virtual int  SaveModified(void);	//214
public:
	void  SetDefault(int);	//221
	virtual void  SetDescription(class CString);	//226
	void  SetDisableOperation(int);	//227
	virtual void  SetModifiedFlag(int);	//230
	virtual void  Test(void);	//233

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CWPIDoc:public CObject
{
DECLARE_DYNCREATE(CWPIDoc)

protected:
	 CWPIDoc(void);	//6
public:
	virtual  ~CWPIDoc(void);	//13
	virtual int  AnalizeErr(unsigned int);	//25
protected:
	virtual void  Commit(int,class CPropertySchema *);	//30
public:
	void *  CopyItems(int,int);	//31
	virtual int  DeleteItem(int);	//48
	static unsigned int  DocID;	//52
	virtual void  FillPropety(int,class CPropertySchema *);	//54
	class CUserInterface *  FindDefault(void);	//56
	static class CWPIDoc *  GetContainer(void);	//64
protected:
	void  GetRealListItem(class CMessage7 *,long *);	//90
public:
	class CUserInterface *  GetUserInterface(int);	//109
	int  InsertNewItem(int,unsigned int,int);	//116
	void  Modified(int,int);	//135
	void  MovePages(int,int,int);	//137
protected:
	void  OnActivateUIntrf(class CMessage7 *,long *);	//139
	void  OnInsertItem(class CMessage7 *,long *);	//156
public:
	int  PasterNewItems(void *,int,int,int);	//199
	virtual void  RenameItem(int,char const *);	//209
protected:
	virtual int  SaveModified(void);	//215
public:
	virtual unsigned int  SetData(int,class CPropertySchema *);	//220
	void  SetDefault(int);	//222
protected:
	void  UpdateModifiedFlag(int);	//236

	DECLARE_MESSAGE_MAP()
};


class IMPORT_1C CWPIListView:public CObject
{
DECLARE_DYNCREATE(CWPIListView)

protected:
	 CWPIListView(void);	//7
public:
	virtual  ~CWPIListView(void);	//14
protected:
	int  CopyToClipBoard(void);	//32
public:
	virtual int  Create(char const *,char const *,unsigned long,struct tagRECT const &,class CWnd *,unsigned int,struct CCreateContext *);	//35
protected:
	void  FillView(int,int,int,struct _TREEITEM *,struct _TREEITEM *);	//55
public:
	class CWPIDoc *  GetDocument(void);	//69
protected:
	class CUserInterface *  GetSelectedInterface(void);	//101
	struct _TREEITEM *  ItemFromPos(struct _TREEITEM *,int);	//128
	void  MakeContextMenu(class CPoint);	//129
	void  OnCfgEdit2(void);	//140
	void  OnChooseBtn(void);	//141
	void  OnContextMenu(class CWnd *,class CPoint);	//143
	void  OnControlProperty(class CCmdUI *);	//144
	void  OnEditCopy(void);	//149
	void  OnEditCut(void);	//150
	void  OnEditPaste(void);	//151
public:
	virtual void  OnInitialUpdate(void);	//155
protected:
	void  OnLButtonDblClk(unsigned int,class CPoint);	//158
	void  OnLButtonDown(unsigned int,class CPoint);	//159
	void  OnMoveDown(void);	//160
	void  OnMoveUp(void);	//161
	void  OnPropertyCancel(struct tagNMHDR *,long *);	//164
	void  OnPropertyNotVal(struct tagNMHDR *,long *);	//165
	void  OnPropertyOk(struct tagNMHDR *,long *);	//166
	void  OnRButtonDown(unsigned int,class CPoint);	//167
	void  OnRclick(struct tagNMHDR *,long *);	//168
	void  OnRealWpNew(void);	//169
	void  OnShowProperty(void);	//177
	void  OnSort(void);	//178
	void  OnSubWpNew(void);	//179
	virtual void  OnUpdate(class CView *,long,class CObject *);	//180
	void  OnUpdateEditCopy(class CCmdUI *);	//181
	void  OnUpdateEditPaste(class CCmdUI *);	//182
	void  OnUpdateHasSibling(class CCmdUI *);	//183
	void  OnUpdateSelReal(class CCmdUI *);	//184
	void  OnUpdateSelect(class CCmdUI *);	//185
	void  OnUserHelpEdit(void);	//188
	void  OnWpDel(void);	//193
	void  OnWpMedt(void);	//194
	void  OnWpNew(void);	//195
	void  OnWpTbedit(void);	//196
	void  Paster(void *,unsigned long);	//198
public:
	virtual int  PreTranslateMessage(struct tagMSG *);	//200
protected:
	void  RemoveSelItems(int);	//205
	void  ReplaceItem(int);	//211
	void  ReplaceItem(struct _TREEITEM *);	//212
	void  UpdatePos(struct _TREEITEM *,int &);	//237

	DECLARE_MESSAGE_MAP()
};
// End class descriptions
// Begin free functions definitions
// End free functions definitions