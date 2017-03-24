#ifndef UTILS_CORRECT_MIN_MAX_H
#define UTILS_CORRECT_MIN_MAX_H

/* Файл для исправления ситуации с min и max, определенными в SDK.
Т.к. для перехода на std::min и std::max нужны корректировки исходников
компоненты, то пока оставляем ситуацию как есть. В новом коде достаточно
включить этот файл */

#pragma once

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include <algorithm>

#endif // UTILS_CORRECT_MIN_MAX_H
