#include <catch2/catch_all.hpp>

#include "services/application/app_both_dirnfile_service.h"
#include "services/application/app_dir_service.h"
#include "services/application/app_file_service.h"
#include "services/domain/user_service.h"
#include "services/domain/group_service.h"
#include "services/domain/rights_service.h"
#include "services/domain/kernel_service.h"
#include "services/domain/file_service.h"
#include "services/domain/dir_service.h"
#include "services/session/session_service.h"
#include "factories/entity/user_fac.h"
#include "factories/entity/group_fac.h"
#include "factories/objects/file_object_fac.h"
#include "factories/objects/inmem_fac.h"
#include "factories/objects/acl_fac.h"
#include "containers/kernel_container.h"
#include "factories/init/kernel_fac.h"
#include "dto/objects/stat_file_dto.h"
#include "dto/objects/file_content_dto.h"
#include "domain/objects/directory.h"
#include "domain/objects/directory_structure.h"
#include "domain/objects/reg_file.h"

using namespace services;
using namespace domain;

namespace {

struct TestFixture {
    UidGenerator uid_gen;
    GidGenerator gid_gen;
    InodeGenerator inode_gen;

    std::shared_ptr<UserFactory> user_factory;
    std::shared_ptr<GroupFactory> group_factory;
    std::shared_ptr<InMemoryFactory> inmem_factory;
    std::shared_ptr<ACLFactory> acl_factory;
    std::shared_ptr<FileFactory> file_factory;
    std::shared_ptr<DirectoryFactory> dir_factory;

    std::shared_ptr<IKernelContainer> kernel_container;

    std::shared_ptr<UserService> user_service;
    std::shared_ptr<GroupService> group_service;
    std::shared_ptr<RightsService> rights_service;
    std::shared_ptr<KernelService> kernel_service;
    std::shared_ptr<FileService> file_service;
    std::shared_ptr<DirectoryService> dir_service;

    SessionService session_service;

    std::unique_ptr<DirNFileAppService> dirnfile_app;
    std::unique_ptr<DirectoryAppService> dir_app;
    std::unique_ptr<FileAppService> file_app;

    TestFixture() {
        user_factory = std::make_shared<UserFactory>(uid_gen);
        group_factory = std::make_shared<GroupFactory>(gid_gen);
        inmem_factory = std::make_shared<InMemoryFactory>(inode_gen);
        acl_factory = std::make_shared<ACLFactory>();
        file_factory = std::make_shared<FileFactory>();
        dir_factory = std::make_shared<DirectoryFactory>();

        KernelFactory kernel_factory(group_factory, inmem_factory, user_factory);
        kernel_container = kernel_factory.createKernelContainerColdStart(
            user_factory, group_factory, dir_factory, inmem_factory, acl_factory
        );

        user_service = std::make_shared<UserService>(user_factory, *kernel_container);
        group_service = std::make_shared<GroupService>(group_factory, *kernel_container);
        rights_service = std::make_shared<RightsService>(*user_service, *group_service);
        kernel_service = std::make_shared<KernelService>(kernel_container, inmem_factory);
        file_service = std::make_shared<FileService>(file_factory, inmem_factory, acl_factory);
        dir_service = std::make_shared<DirectoryService>(dir_factory, inmem_factory, acl_factory);

        session_service.login("admin", 0, "/");

        dirnfile_app = std::make_unique<DirNFileAppService>(
            rights_service, dir_service, kernel_service, session_service
        );

        dir_app = std::make_unique<DirectoryAppService>(
            rights_service, dir_service, kernel_service, session_service
        );

        file_app = std::make_unique<FileAppService>(
            rights_service, file_service, dir_service, kernel_service, session_service
        );
    }
};

}

TEST_CASE("DirNFileAppService move directory updates paths recursively") {
    TestFixture f;


    f.dir_app->createDirectory("src", "/");
    f.dir_app->createDirectory("subdir", "/src");
    f.file_app->createFile("file.txt", "/src/subdir");
    f.file_app->writeFile("/src/subdir/file.txt", "test content");


    f.dir_app->createDirectory("dst", "/");


    auto* subdir_before = f.kernel_service->resolvePath("/src/subdir");
    REQUIRE(subdir_before != nullptr);
    REQUIRE(subdir_before->getPath() == "/src/subdir");

    auto* file_before = f.kernel_service->resolvePath("/src/subdir/file.txt");
    REQUIRE(file_before != nullptr);
    REQUIRE(file_before->getPath() == "/src/subdir/file.txt");


    f.dirnfile_app->move("/src/subdir", "/dst");


    auto* subdir_after = f.kernel_service->resolvePath("/dst/subdir");
    REQUIRE(subdir_after != nullptr);
    REQUIRE(subdir_after->getPath() == "/dst/subdir");

    auto* file_after = f.kernel_service->resolvePath("/dst/subdir/file.txt");
    REQUIRE(file_after != nullptr);
    REQUIRE(file_after->getPath() == "/dst/subdir/file.txt");


    REQUIRE_THROWS(f.kernel_service->resolvePath("/src/subdir"));
}

TEST_CASE("DirNFileAppService copy directory clones with correct paths") {
    TestFixture f;


    f.dir_app->createDirectory("original", "/");
    f.dir_app->createDirectory("nested", "/original");
    f.file_app->createFile("data.txt", "/original/nested");
    f.file_app->writeFile("/original/nested/data.txt", "important data");


    f.dir_app->createDirectory("backup", "/");


    f.dirnfile_app->copy("/original", "/backup");


    auto* orig = f.kernel_service->resolvePath("/original");
    REQUIRE(orig != nullptr);
    REQUIRE(orig->getPath() == "/original");

    auto* orig_nested = f.kernel_service->resolvePath("/original/nested");
    REQUIRE(orig_nested != nullptr);
    REQUIRE(orig_nested->getPath() == "/original/nested");

    auto* orig_file = f.kernel_service->resolvePath("/original/nested/data.txt");
    REQUIRE(orig_file != nullptr);
    REQUIRE(orig_file->getPath() == "/original/nested/data.txt");


    auto* copy_dir = f.kernel_service->resolvePath("/backup/original");
    REQUIRE(copy_dir != nullptr);
    REQUIRE(copy_dir->getPath() == "/backup/original");

    auto* copy_nested = f.kernel_service->resolvePath("/backup/original/nested");
    REQUIRE(copy_nested != nullptr);
    REQUIRE(copy_nested->getPath() == "/backup/original/nested");

    auto* copy_file = f.kernel_service->resolvePath("/backup/original/nested/data.txt");
    REQUIRE(copy_file != nullptr);
    REQUIRE(copy_file->getPath() == "/backup/original/nested/data.txt");


    REQUIRE(orig != copy_dir);
    REQUIRE(orig_nested != copy_nested);
    REQUIRE(orig_file != copy_file);


    auto content = f.file_app->readFile("/backup/original/nested/data.txt");
    REQUIRE(content.data == "important data");
}

TEST_CASE("DirNFileAppService move file updates path") {
    TestFixture f;


    f.file_app->createFile("moveme.txt", "/");
    f.file_app->writeFile("/moveme.txt", "moving file");
    f.dir_app->createDirectory("target", "/");

    auto* file_before = f.kernel_service->resolvePath("/moveme.txt");
    REQUIRE(file_before->getPath() == "/moveme.txt");


    f.dirnfile_app->move("/moveme.txt", "/target");


    auto* file_after = f.kernel_service->resolvePath("/target/moveme.txt");
    REQUIRE(file_after != nullptr);
    REQUIRE(file_after->getPath() == "/target/moveme.txt");


    REQUIRE_THROWS(f.kernel_service->resolvePath("/moveme.txt"));
}

TEST_CASE("DirNFileAppService copy file clones correctly") {
    TestFixture f;


    f.file_app->createFile("source.txt", "/");
    f.file_app->writeFile("/source.txt", "source content");
    f.dir_app->createDirectory("copies", "/");


    f.dirnfile_app->copy("/source.txt", "/copies");


    auto orig_content = f.file_app->readFile("/source.txt");
    REQUIRE(orig_content.data == "source content");


    auto copy_content = f.file_app->readFile("/copies/source.txt");
    REQUIRE(copy_content.data == "source content");


    auto* orig = f.kernel_service->resolvePath("/source.txt");
    auto* copy = f.kernel_service->resolvePath("/copies/source.txt");
    REQUIRE(orig != copy);
    REQUIRE(copy->getPath() == "/copies/source.txt");
}

TEST_CASE("DirNFileAppService remove file") {
    TestFixture f;

    f.file_app->createFile("todelete.txt", "/");

    auto* root = dynamic_cast<IDirectory*>(f.kernel_service->resolvePath("/"));
    REQUIRE(root->getDirectoryStructure()->contains("todelete.txt"));

    f.dirnfile_app->remove("/todelete.txt");

    REQUIRE_FALSE(root->getDirectoryStructure()->contains("todelete.txt"));
}

TEST_CASE("DirNFileAppService remove empty directory") {
    TestFixture f;

    f.dir_app->createDirectory("toremove", "/");

    auto* root = dynamic_cast<IDirectory*>(f.kernel_service->resolvePath("/"));
    REQUIRE(root->getDirectoryStructure()->contains("toremove"));

    f.dirnfile_app->remove("/toremove");

    REQUIRE_FALSE(root->getDirectoryStructure()->contains("toremove"));
}

TEST_CASE("DirNFileAppService remove non-empty directory throws") {
    TestFixture f;

    f.dir_app->createDirectory("nonempty", "/");
    f.file_app->createFile("inside.txt", "/nonempty");


    REQUIRE_THROWS_WITH(f.dirnfile_app->remove("/nonempty"),
                        Catch::Matchers::ContainsSubstring("not empty"));
}

TEST_CASE("DirNFileAppService getStat") {
    TestFixture f;

    f.file_app->createFile("statme.txt", "/");

    auto stat = f.dirnfile_app->getStat("/statme.txt");

    REQUIRE(stat.uid == 0);
    REQUIRE(stat.gid == 0);
}

TEST_CASE("DirNFileAppService move nested directories") {
    TestFixture f;


    f.dir_app->createDirectory("a", "/");
    f.dir_app->createDirectory("b", "/a");
    f.dir_app->createDirectory("c", "/a/b");
    f.dir_app->createDirectory("d", "/a/b/c");
    f.file_app->createFile("file.txt", "/a/b/c/d");

    f.dir_app->createDirectory("target", "/");


    f.dirnfile_app->move("/a/b", "/target");


    auto* b = f.kernel_service->resolvePath("/target/b");
    REQUIRE(b->getPath() == "/target/b");

    auto* c = f.kernel_service->resolvePath("/target/b/c");
    REQUIRE(c->getPath() == "/target/b/c");

    auto* d = f.kernel_service->resolvePath("/target/b/c/d");
    REQUIRE(d->getPath() == "/target/b/c/d");

    auto* file = f.kernel_service->resolvePath("/target/b/c/d/file.txt");
    REQUIRE(file->getPath() == "/target/b/c/d/file.txt");
}

TEST_CASE("DirNFileAppService copy nested directories") {
    TestFixture f;


    f.dir_app->createDirectory("project", "/");
    f.dir_app->createDirectory("src", "/project");
    f.dir_app->createDirectory("include", "/project");
    f.file_app->createFile("main.cpp", "/project/src");
    f.file_app->createFile("header.h", "/project/include");
    f.file_app->writeFile("/project/src/main.cpp", "int main() {}");
    f.file_app->writeFile("/project/include/header.h", "#pragma once");

    f.dir_app->createDirectory("backup", "/");


    f.dirnfile_app->copy("/project", "/backup");


    auto* proj_copy = f.kernel_service->resolvePath("/backup/project");
    REQUIRE(proj_copy->getPath() == "/backup/project");

    auto* src_copy = f.kernel_service->resolvePath("/backup/project/src");
    REQUIRE(src_copy->getPath() == "/backup/project/src");

    auto* inc_copy = f.kernel_service->resolvePath("/backup/project/include");
    REQUIRE(inc_copy->getPath() == "/backup/project/include");


    auto main_content = f.file_app->readFile("/backup/project/src/main.cpp");
    REQUIRE(main_content.data == "int main() {}");

    auto header_content = f.file_app->readFile("/backup/project/include/header.h");
    REQUIRE(header_content.data == "#pragma once");
}

