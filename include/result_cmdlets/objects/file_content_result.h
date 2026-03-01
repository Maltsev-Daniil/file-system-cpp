#pragma once
#include <algorithm>

#include "../icmdlet_result.h"
#include "../../cli_view.h"

struct FileContentDTO;

namespace cmdlets {
/**
 * @class FileContentResult
 * 
 * @brief результат выполнения команды содержимого файла
 * 
 * содержит данные о содержимом одного или нескольких файлов
 */
class FileContentResult : public ICmdletResult {
    std::vector<FileContentDTO> dtos_;
public:
    /**
     * @brief конструктор результата содержимого файла
     * 
     * @param dtos вектор данных о содержимом файлов
     */
    FileContentResult(std::vector<FileContentDTO> dtos) : dtos_(std::move(dtos)) {}

    /**
     * @brief отрисовать результат содержимого файла
     * 
     * @param view представление для отрисовки
     */
    void render(view::ICliView &view) const override {
        for (const auto& dto : dtos_) {
            view.showFileContent(dto);
        }
    }
};
}