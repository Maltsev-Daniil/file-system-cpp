#include "services/shell/path_pattern_resolver.h"
#include "services/domain/kernel_service.h"
#include "services/session/session_service.h"

#include <cassert>

using namespace services;

std::vector<std::string> PathPatternResolver::resolveGlob(
    const std::string& absolute_pattern
) const {

    // работаем только с абсолютными путями!!!
    assert(!absolute_pattern.empty());
    assert(absolute_pattern[0] == '/');

    return kernel_service.expandAbsolutePattern(absolute_pattern);
}

std::string PathPatternResolver::pathToAbsolute(const std::string &input_path) const noexcept
{
    std::string cwd = session.getCurrentDir();
    std::string absolute_path;
    if (!input_path.empty() && input_path[0] == '/') {
        absolute_path = input_path;
    } else {
        if (cwd.back() == '/') {
            absolute_path = cwd + input_path;
        } else {
            absolute_path = cwd + "/" + input_path;
        }
    }
    return absolute_path;
}
