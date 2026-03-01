#pragma once

#include <string>

/**
 * @class IOutput
 * 
 * @brief интерфейс для вывода текста
 * 
 * предоставляет методы для записи текста и строк с переводом строки
 */
class IOutput {
public:
    /**
     * @brief деструктор интерфейса вывода
     */
    virtual ~IOutput() = default;

    /**
     * @brief записать текст в вывод
     * 
     * @param text текст для записи
     */
    virtual void write(const std::string& text) = 0;

    /**
     * @brief записать строку в вывод с переводом строки
     * 
     * @param line строка для записи
     */
    virtual void writeln(const std::string& line) = 0;
};

/**
 * @class ConsoleOutput
 * 
 * @brief реализация вывода в консоль
 * 
 * выводит текст в стандартный поток вывода
 */
class ConsoleOutput : public IOutput {
public:
    /**
     * @brief записать текст в консоль
     * 
     * @param text текст для записи
     */
    void write(const std::string& text) override;

    /**
     * @brief записать строку в консоль с переводом строки
     * 
     * @param line строка для записи
     */
    void writeln(const std::string& line) override;
};