/*
 * Copyright (C) Sergey P. Derevyago, 2003-2004.
 *
 * Permission to copy, use, modify, sell and distribute this software is granted
 * provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied warranty, and
 * with no claim as to its suitability for any purpose.
 *
 */

/** @file
 * Определения классов sh_ptr и sh_array.
 */

#ifndef __SH_PTR_HPP__
 #define __SH_PTR_HPP__

#include "fix_alloc.hpp"

/**
 * Данный класс является разделяемым "умным указателем" с подсчетом ссылок на
 * объект типа T. В его задачу входит автоматическое уничтожение переданного
 * объекта, когда на него не останется больше ссылок.
 * Если необходимо "забрать" владение переданным указателем у всех копий sh_ptr,
 * то нужно воспользоваться вызовом set(0), в результате чего деструктор sh_ptr
 * ничего не уничтожит, т.к. delete 0 является пустой операцией.
 */
template <class T>
class sh_ptr {
      /** разделяемое представление */
      struct Rep {
             /** указатель на разделяемый объект */
             T* ptr;
             /** количество ссылок */
             size_t refs;

             /** создает представление с единственной ссылкой */
             Rep(T* ptr_) : ptr(ptr_), refs(1) {}

             /** удаляет разделяемый объект */
             ~Rep() { delete ptr; }

             /** для ускорения работы */
             void* operator new(size_t)
             {
              return fixed_alloc<Rep>::alloc();
             }

             /** для ускорения работы */
             void operator delete(void* ptr, size_t)
             {
              fixed_alloc<Rep>::free(ptr);
             }
      };

      /** указатель на разделяемое представление */
      Rep* rep;

      /**
       * Уменьшает количество ссылок
       */
      void empty() const { 
		  if (0 == --rep->refs) 
			  delete rep;
	  }
      
 public:
      /**
       * Создает объект, ответственный за уничтожение переданного указателя.
       * Указатель будет корректно удален даже в случае возникновения исключения
       * в конструкторе sh_ptr, т.о. вызов sh_ptr<T>(new T) не приведет к утечке
       * ресурсов. Тем не менее, не стоит забывать, что вызов вроде
       * f(sh_ptr<T1>(new T1), sh_ptr<T2>(new T2)) уже чреват потерей объектов,
       * т.к. порядок вычисления подвыражений не определен и новые объекты T1 и
       * T2 могут быть созданы до соответствующих sh_ptr, отвечающих за
       * освобождение ресурсов.
       */
      explicit sh_ptr(T* ptr=0)
      {
       try { rep=new Rep(ptr); }
       catch (...) {
             delete ptr;
             throw;
       }
      }

      /**
       * Копирует переданный объект просто увеличивая количество ссылок на
       * разделяемый указатель.
       */
      sh_ptr(const sh_ptr& shp)
      {
       rep=shp.rep;
       rep->refs++;
      }

      /**
       * Уменьшает количество ссылок на разделяемый указатель и удаляет его,
       * если на него никто больше не ссылается.
       */
      ~sh_ptr() { empty(); }

      /**
       * Присваивает новое значение. Количество ссылок на старый разделяемый
       * указатель уменьшается (что может привести к его удалению), а на новый
       * -- увеличивается.
       */
      sh_ptr& operator=(const sh_ptr& shp)
      {
       shp.rep->refs++;
       empty();
       rep=shp.rep;
       
       return *this;
      }

      /**
       * Обменивает содержимое объектов.
       */
      void swap(sh_ptr& shp)
      {
       Rep* tmp=rep;
       rep=shp.rep;
       shp.rep=tmp;
      }

      /**
       * Возвращает ссылку на разделяемый объект при применении к sh_ptr
       * оператора разыменования.
       */
      T& operator*() const { return *rep->ptr; }

      /**
       * Возвращает указатель на разделяемый объект при применении к sh_ptr
       * оператора ->.
       */
      T* operator->() const { return rep->ptr; }

      /**
       * Возвращает значение разделяемого указателя.
       */
      T* get() const { return rep->ptr; }

      /**
       * Устанавливает значение разделяемого указателя, т.е. все копии текущего
       * sh_ptr станут указывать на новый объект.
       */
      void set(T* ptr) { rep->ptr=ptr; }

      /**
       * Возвращает количество ссылок.
       */
      size_t refs() const { return rep->refs; }

      /** artbear
       * Проверяет указатель
       */

      bool isNull() const { return NULL == rep->ptr; }
      bool isValid() const { return !isNull(); }
      bool operator !() const	{ return isNull(); }
     
	  // без этих методов объект нельзя использовать в контейнерах STL
	  bool operator==(const sh_ptr& rhs) const
	 	{ return rep == rhs.rep; }
      bool operator!=(const sh_ptr& rhs) const
 		{ return !operator ==(rhs); }

};

/**
 * Данный класс является разделяемым "умным указателем" с подсчетом ссылок на
 * массив объектов типа T. В его задачу входит автоматическое уничтожение
 * переданного массива, когда на него не останется больше ссылок. Если
 * необходимо "забрать" владение переданным указателем у всех копий sh_array,
 * то нужно воспользоваться вызовом set(0), в результате чего деструктор
 * sh_array ничего не уничтожит, т.к. delete 0 является пустой операцией.
 * Как правило, использование данного "умного указателя" на массив не очень
 * удачная идея и лучше выбрать комбинацию sh_ptr<std::vector>. 
 */
template <class T>
class sh_array {
      /** разделяемое представление */
      struct Rep {
             /** указатель на разделяемый массив объектов */
             T* ptr;
             /** количество ссылок */
             size_t refs;

             /** создает представление с единственной ссылкой */
             Rep(T* ptr_) : ptr(ptr_), refs(1) {}

             /** удаляет разделяемый массив объектов */
             ~Rep() { delete [] ptr; }

             /** для ускорения работы */
             void* operator new(size_t)
             {
              return fixed_alloc<Rep>::alloc();
             }

             /** для ускорения работы */
             void operator delete(void* ptr, size_t)
             {
              fixed_alloc<Rep>::free(ptr);
             }
      };

      /** указатель на разделяемое представление */
      Rep* rep;
      
 public:
      /**
       * Создает объект, ответственный за уничтожение переданного указателя на
       * массив объектов. Указатель будет корректно удален даже в случае
       * возникновения исключения в конструкторе sh_array, т.о. вызов
       * sh_array<T>(new T[10]) не приведет к утечке ресурсов.
       */
      explicit sh_array(T* ptr=0)
      {
       try { rep=new Rep(ptr); }
       catch (...) {
             delete [] ptr;
             throw;
       }
      }

      /**
       * Копирует переданный объект просто увеличивая количество ссылок на
       * разделяемый указатель.
       */
      sh_array(const sh_array& sha)
      {
       rep=sha.rep;
       rep->refs++;
      }

      /**
       * Уменьшает количество ссылок на разделяемый указатель и удаляет его,
       * если на него никто больше не ссылается.
       */
      ~sh_array() { if (--rep->refs==0) delete rep; }

      /**
       * Присваивает новое значение. Количество ссылок на старый разделяемый
       * указатель уменьшается (что может привести к его удалению), а на новый
       * -- увеличивается.
       */
      sh_array& operator=(const sh_array& sha)
      {
       sha.rep->refs++;
       if (--rep->refs==0) delete rep;
       rep=sha.rep;
       
       return *this;
      }

      /**
       * Обменивает содержимое объектов.
       */
      void swap(sh_array& sha)
      {
       Rep* tmp=rep;
       rep=sha.rep;
       sha.rep=tmp;
      }

      /**
       * Возвращает ссылку на i-тый элемент разделяемого массива.
       */
      T& operator[](size_t i) const { return rep->ptr[i]; }

      /**
       * Возвращает значение разделяемого указателя.
       */
      T* get() const { return rep->ptr; }

      /**
       * Устанавливает значение разделяемого указателя, т.е. все копии текущего
       * sh_array станут указывать на новый массив объектов.
       */
      void set(T* ptr) { rep->ptr=ptr; }

      /**
       * Возвращает количество ссылок.
       */
      size_t refs() const { return rep->refs; }
};

#endif
