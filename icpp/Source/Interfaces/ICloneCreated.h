// Vector.h: interface ICloneCreated.
//
//	Автор - Артур Аюханов aka artbear, 2008
//
//	Интерфейс ICloneCreated будет использоваться для быстрого создания собственных встроенных объектов 1С++ и других ВК
//	Используется техника клонирования или прототипов
//	Каждый класс должен сам уметь создавать своих аналогов
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ICloneCreated__INCLUDED_)
#define _ICloneCreated__INCLUDED_

#pragma once

namespace Utils1cpp
{

class ICloneCreated
{
public:
	virtual ~ICloneCreated(void) {}

	virtual CBLContext* Clone(LPCSTR szClassName) const = 0;
};

} // namespace

#endif