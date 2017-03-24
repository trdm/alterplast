// savewrapper.h
#ifndef SAVEWRAPPER_H
#define SAVEWRAPPER_H
class CSaveWrapper:public CDocument
{
public:
	int DoFileSaveMXL();
	int DoFileSaveDoc();	// Сохранение мдшника без изменения структуры
	int DoFileSaveText();	// Сохранение текстовых файлов
	int DoFileSaveErt();	// Сохранение ертшек
	int DoFileSaveToPath(LPCTSTR name,int param);	// Сохранение мдшника с изменением структуры (или вручную в файл)
	static bool FastSaveMDToFile(LPCTSTR lpPath,bool FireEvent=false);
	static void Init();
};
#endif
