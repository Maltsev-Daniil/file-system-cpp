#include "../include/cmd_dispatcher.h"
#include "result_cmdlets/icmdlet_result.h"
#include "cmdlets/icmdlet.h"
#include "../include/cli_view.h"
#include <stdexcept>

std::unique_ptr<cmdlets::ICmdletResult> CmdDispatcher::executeCmdlet(const std::string& name,
    const CmdletRequest& req)
{
    auto it = cmdlets_map.find(name);
    if (it == cmdlets_map.end()) {
        throw std::runtime_error(
            "unknown command: " + name
        );
    }

    if (!it->second) {
        throw std::runtime_error(
            "cmdlet is not initialized: " + name
        );
    }

    return it->second->execute(req);
}

void CmdDispatcher::registerCmdlet(
        const std::string& name,
        std::unique_ptr<cmdlets::ICmdlet> cmdlet
        ) {
    if (!cmdlet) {
        throw std::invalid_argument(
            "CmdDispatcher::registerCmdlet: cmdlet is null"
        );
    }

    auto [it, inserted] = cmdlets_map.emplace(name, std::move(cmdlet));
    if (!inserted) {
        throw std::runtime_error(
            "CmdDispatcher::registerCmdlet: cmdlet already registered: " + name
        );
    }
}

void CmdDispatcher::clearCmdlets() {
    cmdlets_map.clear();
}