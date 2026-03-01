#pragma once

#include <string>

/**
 * @class ISerializer
 * 
 * @brief интерфейс для сериализации объектов
 * 
 * определяет методы записи и чтения объектов в файл
 * 
 * @tparam T тип объекта для сериализации
 */
template<typename T>
class ISerializer {
public:
    /**
     * @brief деструктор интерфейса сериализатора
     */
    virtual ~ISerializer() = default;

    /**
     * @brief записать объект в файл
     * 
     * @param path путь к файлу
     * @param object объект для записи
     */
    virtual void write(
        const std::string& path,
        const T& object
    ) const = 0;

    /**
     * @brief прочитать объект из файла
     * 
     * @param path путь к файлу
     * @return T прочитанный объект
     */
    virtual T read(
        const std::string& path
    ) const = 0;
};