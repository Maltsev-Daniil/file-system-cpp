#include <memory>
#include <iostream>

#include "debug.h"
#include "alias/alias_handler.h"
#include "alias/alias_registry.h"
#include "alias/ialias_handler.h"
#include "include/factories/objects/acl_fac.h"

#include "include/factories/init/kernel_fac.h"
#include "containers/kernel_container.h"

#include "factories/init/domain_services_fac.h"

#include "factories/init/app_services_fac.h"

#include "cmdlets/icmdlet.h"
#include "factories/init/cmdlet_fac.h"
#include "include/cmd_dispatcher.h"
#include "services/shell/path_pattern_resolver.h"

#include "containers/system_container.h"
#include "dto/fs/disk_usage_dto.h"
#include "include/factories/objects/file_object_fac.h"
#include "include/factories/entity/group_fac.h"
#include "include/factories/objects/inmem_fac.h"
#include "include/factories/entity/user_fac.h"
#include "include/cli_view.h"

#include "include/io.h"
#include "include/processor.h"
#include "services/application/app_filesystem_persistance_service.h"
#include "services/application/app_filesystem_service.h"

int main() {
    domain::InodeGenerator inode_gen;
    domain::UidGenerator uid_gen;
    domain::GidGenerator gid_gen;

    auto inmem_factory = std::make_shared<domain::InMemoryFactory>(inode_gen);
    auto file_factory = std::make_shared<domain::FileFactory>();
    auto dir_factory = std::make_shared<domain::DirectoryFactory>();
    auto user_factory = std::make_shared<domain::UserFactory>(uid_gen);
    auto group_factory = std::make_shared<domain::GroupFactory>(gid_gen);
    auto acl_factory = std::make_shared<domain::ACLFactory>();

    auto kernel_factory = std::make_unique<domain::KernelFactory>(
        group_factory,
        inmem_factory,
        user_factory
    );

    auto kernel_container =
            kernel_factory->createKernelContainerColdStart(
                user_factory,
                group_factory,
                dir_factory,
                inmem_factory,
                acl_factory
            );

    auto domain_services_factory =
            std::make_unique<services::DomainServicesFactory>(
                file_factory,
                dir_factory,
                user_factory,
                group_factory,
                inmem_factory,
                acl_factory,
                kernel_container
            );

    auto dispatcher = std::make_shared<CmdDispatcher>();
    auto output = std::make_shared<ConsoleOutput>();
    auto alias_handler = std::make_shared<AliasHandler>();
    auto alias_registry = std::make_shared<AliasRegistry>(*alias_handler);
    auto processor = std::make_shared<view::CmdProcessor>(*dispatcher, *alias_registry);
    auto view = std::make_shared<view::CliView>(*processor, *output);

    SystemContainer system(
        kernel_container,
        std::move(domain_services_factory),
        dispatcher,
        *output,
        std::move(kernel_factory),
        acl_factory,
        inmem_factory
    );

    system.init();
    view->run();

    // std::string ans;
    // std::cout << "do you want timing? y/n" << std::endl;
    // std::getline(std::cin, ans);
    // if (ans == "y") {
    //     auto pers_service = system.getFileSystemPersistenceAppService();
    //     auto fs_service = system.getFileSystemAppService();
    //
    //     std::string filename = "/home/nitsir/Projects/oop2025/3/cmake-build-debug/fs_snapshots/fs_";
    //     for (int i = 5000; i <= 100000; i += 5000) {
    //         pers_service = system.getFileSystemPersistenceAppService();
    //         pers_service->load(filename + std::to_string(i) + ".yaml");
    //         fs_service = system.getFileSystemAppService();
    //         std::cout << i << ": \n";
    //         fs_service->diskUsage();
    //     }
    //
    //     std::cout << "timing done!" << std::endl;
    // } else {
    //     view->run();
    // }
}
