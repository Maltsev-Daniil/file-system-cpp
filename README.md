# 🚀 File System Simulator (C++ / Architecture / Multithreading)

## 📌 О проекте

File System Simulator — это модульная система, моделирующая файловую структуру с поддержкой CLI, алиасов, прав доступа и сериализации состояния.

Ключевой фокус проекта — **архитектура, расширяемость и инженерные практики уровня production-систем**.

---

## 🧠 Ключевые особенности

- расширяемая архитектура (MVC + Layered + DDD-подход)  
- система команд (Command Pattern, 20+ cmdlets)  
- многопоточная обработка файловой системы (thread pool)  
- поддержка ACL (Access Control List)  
- сериализация состояния (YAML/JSON через mapper)  
- alias-система для CLI  

---

## 🏗 Архитектура

Проект построен как многослойная система:

- **Presentation Layer** — CLI + абстракция вывода  
- **Application Layer** — orchestration логики  
- **Domain Layer** — бизнес-логика и модели  
- **Infrastructure Layer** — сериализация, контейнеры, фабрики  

### Расширяемый View

Интерфейс `ICliView` спроектирован как **абстракция представления**, не привязанная к CLI.

Это позволяет:
- заменить консоль на GUI (например, через SFML)  
- подключить альтернативные способы отображения  
- переиспользовать бизнес-логику без изменений  

---

## ⚡ Многопоточность

Реализован собственный **Thread Pool** (`include/mt/thread_pool.h`):

- task-based модель  
- переиспользование потоков  
- параллельный обход файловой системы (BFS)  
- потокобезопасность через atomic и синхронизацию  

Используется в команде `du`.

---

## 📦 Custom container

В проекте реализован собственный ассоциативный контейнер (`include/domain/objects/dan_map.h`) на основе **AVL-дерева** (self-balancing BST), обеспечивающего `O(log n)` для `find / insert / erase`.

Поддерживается **bidirectional iterator** с in-order обходом (элементы всегда отсортированы по ключу).

Корректность обеспечивается через:
- проверку уникальности ключей (`find != end`)
- контроль вставки (`emplace`)
- автоматическую балансировку (rotation + balance factor)

Используется в `DirectoryStructure` для хранения объектов файловой системы:
`cpp
Tree<std::string, std::unique_ptr<IFileObject>>`.

---

## 🧩 Система команд

Реализовано **20+ команд** (`include/cmdlets/`):

- файловые операции: `ls`, `mkdir`, `rm`, `touch`, `cat`, `echo`  
- пользователи и группы: `login`, `useradd`, `groupmod`  
- права доступа: `chmod`, `chown`, `getfacl`, `geteff`  
- система: `du`, `savefs`, `loadfs`, `alias`  

Архитектура позволяет добавлять новые команды без изменения существующего кода.

---

## 🔐 Права доступа (ACL)

Поддержка расширенной модели прав:

- owner / group / other  
- specific user / group  
- вычисление **эффективных прав**  

---

## 🔄 Сериализация и Mapper Layer

Реализован слой маппинга:

- объекты → YAML / JSON  
- восстановление состояния системы  
- snapshot всей файловой системы  

---

## 🧱 Используемые паттерны

**Поведенческие:**
- Command  
- Visitor  
- Strategy  

**Порождающие:**
- Factory  

**Архитектурные:**
- MVC  
- Layered Architecture  
- Repository  
- Mapper  
- DTO  

---

## 🛠 Стек технологий

- **C++20**
- **CMake 4.0**

### Библиотеки
- SFML 2.6+  
- yaml-cpp  
- Catch2  

---

## 🧪 Тестирование

- 40+ unit-тестов  
- покрытие ключевых компонентов (domain, services, multithreading)

---

## 📚 Документация

Проект документирован с использованием **Doxygen**:

- документация классов и интерфейсов  
- описание параметров и контрактов  
- поддержка навигации по архитектуре  

---

## 🛠 Сборка

```bash
git clone https://github.com/Maltsev-Daniil/file_system_cpp.git
cd file_system_cpp

mkdir build
cd build

cmake ..
cmake --build .
