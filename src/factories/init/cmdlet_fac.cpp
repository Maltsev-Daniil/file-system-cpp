#include "factories/init/cmdlet_fac.h"

#include "../../../include/cmd_dispatcher.h"

#include "cmdlets/file/cat.h"
#include "cmdlets/file/touch.h"
#include "cmdlets/file/write.h"

#include "cmdlets/dir/ls.h"
#include "cmdlets/dir/mkdir.h"

#include "cmdlets/dir_n_file/cp.h"
#include "cmdlets/dir_n_file/mv.h"
#include "cmdlets/dir_n_file/rm.h"
#include "cmdlets/dir_n_file/stat.h"

#include "cmdlets/file_system/du.h"
#include "cmdlets/file_system/loadfs.h"
#include "cmdlets/file_system/savefs.h"
#include "cmdlets/group/groupmod.h"

#include "cmdlets/rights/chmod.h"
#include "cmdlets/rights/chown.h"
#include "cmdlets/rights/geteff.h"
#include "cmdlets/rights/getfacl.h"

#include "cmdlets/user/login.h"
#include "cmdlets/user/logout.h"
#include "cmdlets/user/useradd.h"
#include "cmdlets/user/usermod.h"

using namespace services;
using namespace view;
using namespace cmdlets;

CmdletFactory::CmdletFactory(
    std::shared_ptr<IFileAppService> file_app,
    std::shared_ptr<IDirectoryAppService> dir_app,
    std::shared_ptr<IRightsAppService> right_app,
    std::shared_ptr<IDirNFileAppService> dir_and_file_app,
    std::shared_ptr<IFileSystemAppService> file_system_app,
    std::shared_ptr<IFileSystemPersistenceAppService> file_system_persistence_app,
    std::shared_ptr<IUserAppService> user_app,
    std::shared_ptr<IGroupAppService> group_app,
    std::shared_ptr<IPathPatternResolver> path_resolver,
    std::shared_ptr<::ICmdDispatcher> dispatcher,
    IOutput& output
)
    : file_app(std::move(file_app)),
      dir_app(std::move(dir_app)),
      right_app(std::move(right_app)),
      dir_and_file_app(std::move(dir_and_file_app)),
      file_system_app(std::move(file_system_app)),
    file_system_persistence_app(std::move(file_system_persistence_app)),
      user_app(std::move(user_app)),
      group_app(std::move(group_app)),
      path_resolver(std::move(path_resolver)),
      dispatcher(std::move(dispatcher)),
      output(output)
{
}

void CmdletFactory::registerCmdlets()
{
    registerFileCmdlets();
    registerDirectoryCmdlets();
    registerDirNFileCmdlets();
    registerFileSystemCmdlets();
    registerRightsCmdlets();
    registerUserCmdlets();
    registerGroupCmdlets();
}

void CmdletFactory::registerFileCmdlets()
{
    dispatcher->registerCmdlet(
        "cat",
        std::make_unique<CatCmdlet>(*file_app, *path_resolver, output)
    );

    dispatcher->registerCmdlet(
        "touch",
        std::make_unique<TouchCmdlet>(*file_app, *path_resolver)
    );

    dispatcher->registerCmdlet(
        "write",
        std::make_unique<WriteCmdlet>(*file_app, *path_resolver)
    );
}

void CmdletFactory::registerDirectoryCmdlets()
{
    dispatcher->registerCmdlet(
        "ls",
        std::make_unique<LsCmdlet>(*dir_app, *path_resolver, output)
    );

    dispatcher->registerCmdlet(
        "mkdir",
        std::make_unique<MkdirCmdlet>(*dir_app, *path_resolver)
    );
}

void CmdletFactory::registerDirNFileCmdlets()
{
    dispatcher->registerCmdlet(
        "cp",
        std::make_unique<CpCmdlet>(*dir_and_file_app, *path_resolver)
    );

    dispatcher->registerCmdlet(
        "mv",
        std::make_unique<MvCmdlet>(*dir_and_file_app, *path_resolver)
    );

    dispatcher->registerCmdlet(
        "rm",
        std::make_unique<RmCmdlet>(*dir_and_file_app, *path_resolver)
    );

    dispatcher->registerCmdlet(
        "stat",
        std::make_unique<StatCmdlet>(*dir_and_file_app, *path_resolver, output)
    );
}

void CmdletFactory::registerFileSystemCmdlets()
{
    dispatcher->registerCmdlet(
        "du",
        std::make_unique<DuCmdlet>(*file_system_app, output)
    );

    dispatcher->registerCmdlet(
        "loadfs",
        std::make_unique<LoadfsCmdlet>(*file_system_persistence_app));

    dispatcher->registerCmdlet(
        "savefs",
        std::make_unique<SavefsCmdlet>(*file_system_persistence_app));
}

void CmdletFactory::registerRightsCmdlets()
{
    dispatcher->registerCmdlet(
        "chmod",
        std::make_unique<ChmodCmdlet>(*right_app, *path_resolver)
    );

    dispatcher->registerCmdlet(
        "chown",
        std::make_unique<ChownCmdlet>(*right_app, *path_resolver)
    );

    dispatcher->registerCmdlet(
        "geteff",
        std::make_unique<GeteffCmdlet>(*right_app, *path_resolver, output)
    );

    dispatcher->registerCmdlet(
        "getfacl",
        std::make_unique<GetfaclCmdlet>(*right_app, *path_resolver, output)
    );
}

void CmdletFactory::registerUserCmdlets()
{
    dispatcher->registerCmdlet(
        "login",
        std::make_unique<LoginCmdlet>(*user_app)
    );

    dispatcher->registerCmdlet(
        "usermod",
        std::make_unique<UsermodCmdlet>(*user_app)
    );

    dispatcher->registerCmdlet(
        "logout",
        std::make_unique<LogoutCmdlet>(*user_app));

    dispatcher->registerCmdlet(
        "useradd",
        std::make_unique<UseraddCmdlet>(*user_app));
}

void CmdletFactory::registerGroupCmdlets()
{
    dispatcher->registerCmdlet(
        "groupmod",
        std::make_unique<GroupmodCmdlet>(*group_app));
}