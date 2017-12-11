// config.h
// Базовый сервис загрузчика
#ifndef CONFIG_H
#define CONFIG_H

// Тип указателя на функцию-обработчик создания окон 
typedef void(*HANDLER_ON_CREATE_WND)(CWnd*);
// Типы событий Конфигуратора
#define CFG_ON_CREATE_MAIN_WND		0x1		// Создание основного окна
#define CFG_ON_CREATE_CFG_WND		0x2		// Создание окна "Конфигурация"

// Заголовок класса, предоставляющего сервис
class CConfigService{
public:
	virtual CString IBDir()=0;
	virtual CString BinDir()=0;
	virtual CDocument* OpenFile(CString path)=0;
	virtual bool AddConfigTab(CString name,HICON hIcon,CRuntimeClass* pRTCDoc,CRuntimeClass* pRTCView)=0;
	virtual void AddConfigEventHandler(WORD wEventType,void* pHandler)=0;
	virtual bool LoadModule(CString,CString&)=0;
	virtual void LoadToolBar(HINSTANCE hInst,UINT nID,LPCTSTR lpszName,int count)=0;
	virtual void AddCmdTarget(CCmdTarget* pTarget)=0;
	virtual void RemoveCmdTarget(CCmdTarget* pTarget)=0;
	virtual void AddDynLink(AFX_EXTENSION_MODULE& pModule)=0;
	virtual bool RegisterPanelClass(LPCTSTR lpszName,CRuntimeClass* pWndClass,HICON hIcon)=0;

};
/*
Для обращения к севису Ваша dll должна экспортировать функцию "SetConvigService"
extern "C" _declspec(dllexport) void SetConvigService(void* p);
Если такая функция будет найдена в dll, загрузчик вызовет
ее и передаст указатель на объект класса CConfigService, через
который и можно будет вызывать функции.
На данный момент предоставляется следующий сервис:

CString IBDir();
Возвращает каталог базы данных (со слэшем в конце)

CString BinDir();
Возвращает каталог исполнямых файлов (со слэшем в конце)

CDocument* OpenFile(CString path);
Открывает файл в Конфигураторе.
Возвращает указатель на документ файла.
Для ert и mxl - CWorkBookDoc,
для остальных - CBodyTextDoc.
Параметры:
path - путь к файлу.

bool AddConfigTab(CString name,HICON hIcon,CRuntimeClass* pRTCDoc,CRuntimeClass* pRTCView);
Создает закладку в окне Конфигурации.
Возвращает true в случае успешного создания, иначе - false.
Параметры:
name		- подпись закладки
hIcon		- иконка закладки
pRTCDoc		- указатель на CRuntimeClass документа.
pRTCView	- указатель на CRuntimeClass отображения.
В качестве закладки создается экземпляр вашего view'а с документом.

void AddConfigEventHandler(WORD wEventType,void* pHandler);
Зарегистрировать функции-обработчики различных событий в Конфигураторе.
Параметры:
wEventType	- Тип события
pHandler	- Указатель на функцию-обработчик.
На данный момент пока существует два типа событий:
CFG_ON_CREATE_MAIN_WND	- создание основного окна Конфигуратора.
CFG_ON_CREATE_CFG_WND	- создание окна "Конфигурация".
Для обоих событий функция-обработчик должна иметь тип
void (CWnd*)
В качестве параметра передается указатель на CWnd создаваемого окна.
*/
#endif/////////////////////////////////////////////////////////////////////////////
// IConfigurator wrapper class

class IConfigurator : public COleDispatchDriver
{
public:
	IConfigurator() {}		// Calls COleDispatchDriver default constructor
	IConfigurator(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IConfigurator(const IConfigurator& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetIBName();
	void SetIBName(LPCTSTR lpszNewValue);
	CString GetIBDir();
	CString GetBinDir();
	CString GetCommandLine();
	void Message(LPCTSTR msgLine, long marker);
	void Status(LPCTSTR text);
	LPDISPATCH OpenOLEForm(LPCTSTR progID, LPCTSTR Title);
	long GetVersion();
	LPDISPATCH GetMetaData();
	void SendCommand(long cmd);
	BOOL IsConfigWndOpen();
	LPDISPATCH GetPlugins();
	LPDISPATCH GetScripts();
	LPDISPATCH GetWindows();
	void Quit(BOOL PromptSave);
	void LogMessageWrite(long EventCategory, LPCTSTR Message, LPCTSTR strEventType, LPCTSTR Event);
	void Run1CApp(long mode);
	LPDISPATCH GetDocuments();
	LPDISPATCH GetCfgTimer();
	CString GetAppProps(long idx);
};
