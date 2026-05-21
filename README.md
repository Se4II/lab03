# Лабораторная работа №3. Автоматизация сборки проектов на примере CMake

**Выполнил:** Петров Владислав, группа ИУ8-22  
**Операционная система:** Fedora Linux  
**Дата выполнения:** 22.05.2026  

## Цель работы
Освоить базовые возможности системы сборки CMake: создание статических библиотек, настройка зависимостей между модулями, сборка исполнительных приложений и управление путями к заголовочным файлам.

---

## 1. Подготовка рабочего окружения

Создание корневой директории и разбиение проекта на логические модули.

```bash
mkdir -p ~/Se4II/lab03
cd ~/Se4II/lab03
mkdir -p formatter_lib formatter_ex hello_world solver_lib solver
```

2. Создание статической библиотеки formatter
2.1 Исходные файлы

formatter_lib/formatter.h
C++

#pragma once
#include <string>
#include <ostream>

std::string formatter(const std::string& msg);
void formatter(std::ostream& out, const std::string& msg);

formatter_lib/formatter.cpp
C++

#include "formatter.h"

std::string formatter(const std::string& msg) {
    return "[" + msg + "]";
}

void formatter(std::ostream& out, const std::string& msg) {
    out << "[" << msg << "]";
}

2.2 Скрипт сборки

formatter_lib/CMakeLists.txt
Используем PUBLIC в target_include_directories, чтобы заголовки автоматически пробрасывались в зависимые цели.
CMake

cmake_minimum_required(VERSION 3.10)
project(formatter)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(formatter STATIC formatter.cpp)
target_include_directories(formatter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

2.3 Сборка
Shell

cd formatter_lib
rm -rf build
cmake -H. -Bbuild
cmake --build build
cd ..

Результат: статический файл libformatter.a успешно создан, компиляция прошла без предупреждений.
3. Модуль formatter_ex (расширение с зависимостью)

Библиотека делегирует форматирование в formatter, поэтому требуется явно указать линковку.
3.1 Исходные файлы

formatter_ex/formatter_ex.h
C++

#pragma once
#include <string>
#include <ostream>

std::string formatter_ex(const std::string& msg);
void formatter_ex(std::ostream& out, const std::string& msg);

formatter_ex/formatter_ex.cpp
C++

#include "formatter_ex.h"
#include "formatter.h"

std::string formatter_ex(const std::string& msg) {
    return formatter(msg);
}

void formatter_ex(std::ostream& out, const std::string& msg) {
    formatter(out, msg);
}

3.2 Скрипт сборки

formatter_ex/CMakeLists.txt
CMake

cmake_minimum_required(VERSION 3.10)
project(formatter_ex)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(formatter_ex STATIC formatter_ex.cpp)
target_include_directories(formatter_ex PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(formatter_ex PUBLIC formatter)

4. Создание приложения hello_world
4.1 Исходный код

hello_world/main.cpp
C++

#include "formatter_ex.h"
#include <iostream>

int main() {
    formatter_ex(std::cout, "Hello, World!");
    std::cout << std::endl;
    return 0;
}

4.2 Скрипт сборки

hello_world/CMakeLists.txt
CMake

cmake_minimum_required(VERSION 3.10)
project(hello_world)

set(CMAKE_CXX_STANDARD 11)

add_executable(hello_world main.cpp)
target_include_directories(hello_world PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../formatter_ex
    ${CMAKE_CURRENT_SOURCE_DIR}/../formatter_lib
)
target_link_libraries(hello_world PRIVATE formatter_ex formatter)

5. Создание библиотеки solver_lib для решения квадратных уравнений
5.1 Исходные файлы

solver_lib/solver.h
C++

#pragma once
#include <complex>
#include <utility>

using Roots = std::pair<std::complex<double>, std::complex<double>>;
Roots solve_quadratic(double a, double b, double c);

solver_lib/solver.cpp
C++

#include "solver.h"
#include <cmath>

Roots solve_quadratic(double a, double b, double c) {
    double D = b * b - 4.0 * a * c;
    std::complex<double> sqrtD = std::sqrt(std::complex<double>(D, 0.0));
    return {
        (-b + sqrtD) / (2.0 * a),
        (-b - sqrtD) / (2.0 * a)
    };
}

5.2 Скрипт сборки

solver_lib/CMakeLists.txt
CMake

cmake_minimum_required(VERSION 3.10)
project(solver_lib)
set(CMAKE_CXX_STANDARD 11)
add_library(solver_lib STATIC solver.cpp)
target_include_directories(solver_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

6. Создание основного приложения solver
6.1 Исходный код

solver/main.cpp
C++

#include "formatter_ex.h"
#include "solver.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "Equation: x^2 - 3x + 2 = 0" << std::endl;
    
    auto [r1, r2] = solve_quadratic(1.0, -3.0, 2.0);
    
    formatter_ex(std::cout, "x1 = " + std::to_string(r1.real()));
    std::cout << std::endl;
    formatter_ex(std::cout, "x2 = " + std::to_string(r2.real()));
    std::cout << std::endl;
    
    return 0;
}

6.2 Скрипт сборки

solver/CMakeLists.txt
CMake

cmake_minimum_required(VERSION 3.10)
project(solver)

set(CMAKE_CXX_STANDARD 11)

add_executable(solver main.cpp)
target_include_directories(solver PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../formatter_ex
    ${CMAKE_CURRENT_SOURCE_DIR}/../formatter_lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../solver_lib
)
target_link_libraries(solver PRIVATE formatter_ex solver_lib)

7. Итоговая структура проекта
Plaintext

~/Se4II/lab03/
├── formatter_lib/
│   ├── formatter.h
│   ├── formatter.cpp
│   └── CMakeLists.txt
├── formatter_ex/
│   ├── formatter_ex.h
│   ├── formatter_ex.cpp
│   └── CMakeLists.txt
├── hello_world/
│   ├── main.cpp
│   └── CMakeLists.txt
├── solver_lib/
│   ├── solver.h
│   ├── solver.cpp
│   └── CMakeLists.txt
├── solver/
│   ├── main.cpp
│   └── CMakeLists.txt
└── .gitignore
