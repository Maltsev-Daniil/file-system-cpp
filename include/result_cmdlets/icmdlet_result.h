#pragma once

namespace view {
class ICliView;
}

namespace cmdlets {
/**
 * @class ICmdletResult
 * 
 * @brief интерфейс для результата выполнения команды
 * 
 * определяет метод отрисовки результата в представлении
 */
class ICmdletResult {
public:
    /**
     * @brief деструктор интерфейса результата команды
     */
    virtual ~ICmdletResult() = default;

    /**
     * @brief отрисовать результат в представлении
     * 
     * @param view представление для отрисовки
     */
    virtual void render(view::ICliView& view) const = 0;
};
}