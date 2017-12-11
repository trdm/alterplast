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
	virtual void AddConfigEventHandler(WORD wEventType,HANDLER_ON_CREATE_WND pHandler)=0;
};
/*
Для обращения к севису Ваша dll должна экспортировать функцию "SetConvigService"
extern "C" _declspec(dllexport) void SetConvigService(void* p);
Если такая функция будет найдена в dll, загрузчик вызовет
ее и передаст указатель на объект класса CConfigService, через
который и можно будет вызывать функции.
*/
#endif