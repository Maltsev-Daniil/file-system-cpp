#pragma once
#include "icmdlet_result.h"
#include "../cli_view.h"

namespace cmdlets {
/**
 * @class EmptyResult
 * 
 * @brief результат выполнения команды без вывода
 * 
 * используется для команд, которые не требуют вывода данных
 */
class EmptyResult : public ICmdletResult {
    public:
    /**
     * @brief отрисовать пустой результат
     * 
     * @param view представление для отрисовки
     */
    void render(view::ICliView &view) const override {
        view.showMessage("");
    }
};
}
