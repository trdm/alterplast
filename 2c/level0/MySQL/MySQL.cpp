// MySQL.cpp: implementation of the MySQL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MySQL.h" 
#include "mysql-h/mysql.h" 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

 
//////////////////////////////////////////////////////////////////////
// Динамическая загрузка 
//////////////////////////////////////////////////////////////////////

#if MYSQL_VERSION_ID >= 32200
#define MYSQL_RcConnect_Param \
MYSQL *mysql, const char *host,\
					   const char *user,\
					   const char *passwd,\
					   const char *db,\
					   unsigned int port,\
					   const char *unix_socket,\
					   unsigned int clientflag
#else
#define MYSQL_RcConnect_Param \
					MYSQL *mysql, const char *host,\
					   const char *user,\
					   const char *passwd,\
					   unsigned int port,\
					   const char *unix_socket,\
					   unsigned int clientflag

#endif


//Параметры списка 
//OperFunc (имя ф-ции, тип возврата, (аргументы) , возврат )

#define MySQL_ListFunc \
OperFunc(mysql_error,char*,(MYSQL *mysql),return "NOSQL" ) \
OperFunc(mysql_info,char*,(MYSQL *mysql),return NULL ) \
OperFunc(mysql_query, int	 , (MYSQL *mysql, const char *q) , return NULL) \
OperFunc(mysql_data_seek,void,(MYSQL_RES*result,my_ulonglong),return )\
OperFunc(mysql_free_result,void		 , (MYSQL_RES *result), return )\
OperFunc(mysql_options, int	 , (MYSQL *mysql,enum mysql_option option,const char *arg), return NULL )\
OperFunc(mysql_num_rows, my_ulonglong   , (MYSQL_RES *res),return NULL )\
OperFunc(mysql_select_db, int	  , (MYSQL *mysql, const char *db) , return NULL)\
OperFunc(mysql_insert_id,my_ulonglong, (MYSQL *mysql),return NULL )\
OperFunc(mysql_fetch_row,MYSQL_ROW , (MYSQL_RES *result),return NULL )\
OperFunc(mysql_num_fields,unsigned int, (MYSQL_RES *res),return NULL )\
OperFunc(mysql_init,MYSQL* ,(MYSQL* mysql),return NULL )\
OperFunc(mysql_row_seek, MYSQL_ROW_OFFSET , (MYSQL_RES *result, MYSQL_ROW_OFFSET) , return NULL)\
OperFunc(mysql_store_result, MYSQL_RES *, (MYSQL *mysql) , return NULL) \
OperFunc(mysql_real_connect, MYSQL * , (MYSQL_RcConnect_Param) , return NULL)\
OperFunc(mysql_fetch_field, MYSQL_FIELD* , (MYSQL_RES *result),return NULL )

//Заглушки

#define OperFunc(func, typeret, args,ret)\
typeret STDCALL func##_STUB args {ret;}

MySQL_ListFunc

#undef OperFunc

//Структура вызовов
struct CMySQL_Callers{

HMODULE mMySQL;
CMySQL_Callers();
~CMySQL_Callers();

#define OperFunc(func,typeret,args,ret) typeret ( STDCALL *func) args ;
MySQL_ListFunc
#undef OperFunc

};

static CMySQL_Callers MySQL_Callers;
//Вызовы
#define OperFunc(func, typeret, args,ret) \
__declspec(naked) typeret STDCALL func args { __asm jmp  MySQL_Callers.##func }

MySQL_ListFunc

#undef OperFunc

template <typename TTYPE> inline void TypeGetProcAddress(TTYPE &p,HMODULE hmod,LPCSTR lpProc)
{
	 p = (TTYPE) ::GetProcAddress(hmod,lpProc);
}


//Инициализация
CMySQL_Callers::CMySQL_Callers(){
	mMySQL=::LoadLibrary("libmySQL.dll");
	if(mMySQL){
		//Загрузка ф-ций

	#define OperFunc(func, typeret, args,ret) TypeGetProcAddress(func,mMySQL,#func);
	MySQL_ListFunc
	#undef OperFunc
	}

	else{
		//Загрузка заглушек
	#define OperFunc(func, typeret, args,ret) func=func##_STUB;
	MySQL_ListFunc
	#undef OperFunc
	}
}

//Выгрузка
CMySQL_Callers::~CMySQL_Callers(){
//	if(mMySQL)
//		::FreeLibrary(mMySQL);
//	mMySQL=NULL;
}


BOOL CMySQL::MySQLLoaded(){
	return NULL != MySQL_Callers.mMySQL;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
