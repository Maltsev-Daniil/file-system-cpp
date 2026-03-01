#pragma once
#include "cmdlets/icmdlet.h"
#include "dto/rights/file_acl_dto.h"
#include "result_cmdlets/icmdlet_result.h"
#include "../../cli_view.h"


namespace cmdlets {
/**
 * @class FileAclResult
 * 
 * @brief результат выполнения команды списка контроля доступа файла
 * 
 * содержит данные о списке контроля доступа для одного или нескольких файлов
 */
class FileAclResult : public ICmdletResult {
    std::vector<FileAclDTO> dtos_;
    public:
    /**
     * @brief конструктор результата списка контроля доступа
     * 
     * @param dtos вектор данных о списке контроля доступа
     */
    FileAclResult(std::vector<FileAclDTO> dtos) : dtos_(std::move(dtos)) {}

    /**
     * @brief отрисовать результат списка контроля доступа
     * 
     * @param view представление для отрисовки
     */
    void render(view::ICliView &view) const override {
        for (const auto& dto : dtos_) {
            view.showFileAcl(dto);
        }
    }
};
}
