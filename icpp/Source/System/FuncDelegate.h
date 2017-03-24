//////////////////////////////////////////////////////////////////////
//
//	FuncDelegate.h
//	Implementation of CFuncDelegate class
//	Design and code: artbear (Артур Аюханов)
//	Contacts: EMAIL artbear@inbox.ru
//
//	Несколько шаблонных классов-делегатов
//		CFuncDelegate0, CFuncDelegate1, CFuncDelegate2, CFuncDelegate3
//
//	они позволяют вызывать несколько добавленных функций (Add, Remove)
//	строго в порядке добавления через Invoke
//	для каждого класса метод Invoke принимает соответствущее число параметров
//
//	Классы удобно использовать при перехвате функций или вызове разнообразных событий
//	не добавляя ничего в код перехватчика.
//	т.е. перехватчик должен предоставить доступ к конкретному экземпляру диспетчера
//	нужные классы подключатся к диспетчеру, добавив свои функции-обработчики
//	в момент возникновения события диспетчер по очереди в порядке добавления вызывет каждую функцию-обработчик
//
//	ВАЖНО:
//		1. возвращаемое значение никак не используется
//			можно сделать, не меняя указ. классы, просто добавить в функции-обработчики еще один параметр
//			но все функции должны правильно обрабатывать этот параметр (типа если стоит 0, значит, 1 уже ставить нельзя)
//		2. Нельзя прервать цепочку вызова, все обработчики вызываются всегда, если не было исключения !
//			но опять же можно в каждой функции анализировать некий параметр (типа стоит 0, нужно просто выходить)
//
//	Пример использования:
//		typedef int(CBLModule::*PF_FF)(char const*); // тип функции
//		// сначала имя класса, затем тип функции, далее параметры функции
// 		CFuncDelegate1<CBLModule, PF_FF, char const*> Delegate; // делегат
// 		bool b = Delegate.IsEmpty();
//
// 		Delegate.Add(CMyModule1::func1);
// 		Delegate.Add(CMyModule2::func2);
//
// 		CBLModule pMod = this; // объект-владелец перехваченного метода
// 		Delegate.Invoke(pMod, "ПараметрМетода"); // сначала func1 затем func2
//
//  	Delegate.Remove(CMyModule1::func1);
// 		Delegate.Add(CMyModule1::func1);
//
// 		Delegate.Invoke(pMod, "ПараметрМетода"); // сначала func2 затем func1
// 		Delegate.Clear();
//
//
//////////////////////////////////////////////////////////////////////

#ifndef FUNCDELEGATE_H
#define FUNCDELEGATE_H

#pragma once

#include <list>
#include <algorithm>

using namespace std;

template<class TClass, class TFunction>
class CFuncDelegate0
{
public:
	void Add(TFunction func);
	void Remove(TFunction func);

	bool IsEmpty() const;
	void Clear(void);

	void Invoke(const TClass& object);
protected:
	list<DWORD> m_list; // если указать прямо TFunction - не компилится, куча ошибок в шаблонах :(
};

template<class TClass, class TFunction, typename Arg1>
class CFuncDelegate1 : public CFuncDelegate0<TClass, TFunction>
{
public:
	void Invoke(const TClass& object, Arg1& arg1);
};

template<class TClass, class TFunction, class Arg1, class Arg2>
class CFuncDelegate2 : public CFuncDelegate0<TClass, TFunction>
{
public:
	void Invoke(const TClass& object, Arg1& arg1, Arg2& arg2);
};

template<class TClass, class TFunction, class Arg1, class Arg2, class Arg3>
class CFuncDelegate3 : public CFuncDelegate0<TClass, TFunction>
{
public:
	void Invoke(const TClass& object, Arg1& arg1, Arg2& arg2, Arg2& arg3);
};

template<class TClass, typename TFunction>
inline void CFuncDelegate0<TClass, TFunction>::Add(TFunction func)
{
	DWORD pFunc = *reinterpret_cast<DWORD*>(&func);
	if (m_list.end() == find(m_list.begin(), m_list.end(), pFunc))
		m_list.push_back(pFunc);
}

template<class TClass, typename TFunction>
inline void CFuncDelegate0<TClass, TFunction>::Remove(TFunction func)
{
	DWORD pFunc = *reinterpret_cast<DWORD*>(&func);
	m_list.remove(pFunc);
}

template<class TClass, typename TFunction>
inline bool CFuncDelegate0<TClass, TFunction>::IsEmpty() const
{
	return m_list.empty();
}

template<class TClass, typename TFunction>
inline void CFuncDelegate0<TClass, TFunction>::Clear(void)
{
	m_list.clear();
}

template<class TClass, typename TFunction>
inline void CFuncDelegate0<TClass, TFunction>::Invoke(const TClass& object)
{
	list<DWORD>::const_iterator iter = m_list.begin();
	for (;iter != m_list.end(); ++iter)
	{
 		 // вызываю
 		TFunction pFunc = *reinterpret_cast<TFunction*>((void*)&*iter);
 		(object.*pFunc)();
	}
}

template<class TClass, class TFunction, class Arg1>
inline void CFuncDelegate1<TClass, TFunction, Arg1>::Invoke(const TClass& object, Arg1& arg1)
{
	list<DWORD>::const_iterator iter = m_list.begin();
	for (;iter != m_list.end(); ++iter)
	{
		 // вызываю
		TFunction pFunc = *(TFunction*)(void*)(&*iter);
		(object.*pFunc)(arg1);
	}
}

template<class TClass, class TFunction, class Arg1, class Arg2>
inline void CFuncDelegate2<TClass, TFunction, Arg1, Arg2>::Invoke(const TClass& object, Arg1& arg1, Arg2& arg2)
{
	list<DWORD>::const_iterator iter = m_list.begin();
	for (;iter != m_list.end(); ++iter)
	{
		 // вызываю
		TFunction pFunc = *(TFunction*)(void*)(&*iter);
		(object.*pFunc)(arg1, arg2);
	}
}

template<class TClass, class TFunction, class Arg1, class Arg2, class Arg3>
inline void CFuncDelegate3<TClass, TFunction, Arg1, Arg2, Arg3>::Invoke(const TClass& object, Arg1& arg1, Arg2& arg2, Arg2& arg3)
{
	list<DWORD>::const_iterator iter = m_list.begin();
	for (;iter != m_list.end(); ++iter)
	{
		 // вызываю
		TFunction pFunc = *(TFunction*)(void*)(&*iter);
		(object.*pFunc)(arg1, arg2, arg3);
	}
}


#endif