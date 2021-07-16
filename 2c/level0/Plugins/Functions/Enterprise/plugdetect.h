
#ifndef FORPLUGIN

#define PLUG_VER 3

struct PLUGIN_INFO{
	size_t cb;				//Размер структуры
	int VerNeed;			//Необходимая версия обработчика
	CString	csTitle;		//Имя плагина
	CString	csDescription;	//Краткое описание, авторские права...

	//остальные поля, если понадобятся.

};

#ifdef IS_PLUGIN
#define FORPLUGIN _declspec(dllimport)
#define FORMAINPRG  _declspec(dllexport)

#ifdef _DEBUG
// Отдельно для DEBUG-версий - суффикс D
#define InitPlugin InitPlugin_D
#define DonePlugin DonePlugin_D
#define PluginInfo PluginInfo_D
#endif // DEBUG

extern "C" FORMAINPRG BOOL _cdecl InitPlugin();
extern "C" FORMAINPRG BOOL _cdecl DonePlugin();
extern "C" FORMAINPRG BOOL _cdecl PluginInfo(PLUGIN_INFO*);

#else 

#ifdef _DEBUG
#define PLUGINFUNC_SUFFIX "_D"
#else
#define PLUGINFUNC_SUFFIX 
#endif //DEBUG

#define FORPLUGIN _declspec(dllexport)
#define FORMAINPRG _declspec(dllimport)
#endif //IS_PLUGIN



#endif //FORPLUGIN
