#pragma once
#include <string>

#include "domain/types.h"


class IOutput;

namespace view {
class ICmdProcessor;
}

class FileAclDTO;
struct FileERightsDTO;
struct DiskUsageDTO;
struct StatFileDTO;
struct FileContentDTO;
struct DirContentDTO;

namespace view {
/**
 * @class ICliView
 * 
 * @brief интерфейс для представления командной строки
 * 
 * определяет методы для отображения различных типов данных в консоли
 */
class ICliView {
public:
    /**
     * @brief деструктор интерфейса представления
     */
    virtual ~ICliView() = default;

    /**
     * @brief показать текстовое сообщение
     * 
     * @param text текст сообщения
     */
    virtual void showMessage(const std::string& text) = 0;

    /**
     * @brief показать содержимое директории
     * 
     * @param dto данные о содержимом директории
     */
    virtual void showDirectoryContent(const DirContentDTO& dto) = 0;

    /**
     * @brief показать содержимое файла
     * 
     * @param dto данные о содержимом файла
     */
    virtual void showFileContent(const FileContentDTO& dto) = 0;

    /**
     * @brief показать статистику файла
     * 
     * @param dto данные о статистике файла
     */
    virtual void showFileStat(const StatFileDTO& dto) = 0;

    /**
     * @brief показать использование диска
     * 
     * @param dto данные об использовании диска
     */
    virtual void showDiskUsage(const DiskUsageDTO& dto) = 0;

    /**
     * @brief показать эффективные права доступа
     * 
     * @param dto данные об эффективных правах доступа
     */
    virtual void showEffectiveRights(const FileERightsDTO& dto) = 0;

    /**
     * @brief показать список контроля доступа файла
     * 
     * @param dto данные о списке контроля доступа
     */
    virtual void showFileAcl(const FileAclDTO& dto) = 0;

    /**
     * @brief запустить цикл обработки команд
     */
    virtual void run() = 0;
};

/**
 * @class CliView
 * 
 * @brief реализация представления командной строки
 * 
 * обрабатывает команды и отображает результаты в консоли
 */
class CliView : public ICliView {
    ICmdProcessor& cmdProcessor_;
    IOutput& output_;
    public:
    /**
     * @brief конструктор представления командной строки
     * 
     * @param cmdProcessor процессор команд
     * @param output вывод для результатов
     */
    CliView(ICmdProcessor& cmdProcessor, IOutput& output) :
        cmdProcessor_(cmdProcessor), output_(output) {}

    /**
     * @brief показать текстовое сообщение
     * 
     * @param text текст сообщения
     */
    void showMessage(const std::string& text) override;

    /**
     * @brief показать содержимое директории
     * 
     * @param dto данные о содержимом директории
     */
    void showDirectoryContent(const DirContentDTO& dto) override;

    /**
     * @brief показать содержимое файла
     * 
     * @param dto данные о содержимом файла
     */
    void showFileContent(const FileContentDTO& dto) override;

    /**
     * @brief показать статистику файла
     * 
     * @param dto данные о статистике файла
     */
    void showFileStat(const StatFileDTO& dto) override;

    /**
     * @brief показать использование диска
     * 
     * @param dto данные об использовании диска
     */
    void showDiskUsage(const DiskUsageDTO& dto) override;

    /**
     * @brief показать эффективные права доступа
     * 
     * @param dto данные об эффективных правах доступа
     */
    void showEffectiveRights(const FileERightsDTO& dto) override;

    /**
     * @brief показать список контроля доступа файла
     * 
     * @param dto данные о списке контроля доступа
     */
    void showFileAcl(const FileAclDTO& dto) override;

    /**
     * @brief запустить цикл обработки команд
     */
    void run() override;
private:
    /**
     * @brief отформатировать время в 12-часовом формате
     * 
     * @param tp точка времени
     * @return std::string отформатированная строка времени
     */
    std::string formatTime12h(const domain::TimePoint& tp);

    /**
     * @brief преобразовать тип права в строку
     * 
     * @param right тип права
     * @return std::string строковое представление права
     */
    std::string formatRightToStr(const domain::RightType& right);
};
}
