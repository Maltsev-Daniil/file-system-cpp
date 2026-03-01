#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "factories/objects/file_object_fac.h"
#include "factories/objects/inmem_fac.h"
#include "factories/objects/acl_fac.h"
#include "domain/objects/directory.h"
#include "domain/objects/reg_file.h"
#include "domain/objects/inmem_file.h"

using namespace domain;
namespace {
std::unique_ptr<IInMemoryFile> createTestInMem() {
    InodeGenerator inode_gen;
    InMemoryFactory factory(inode_gen);
    ACLFactory acl_fac;
    auto acl = acl_fac.createDefaultFileACL();
    return factory.createInMemory(100, 200, std::move(acl));
}
}

TEST_CASE("FileFactory createObject") {
    FileFactory factory;
    auto inmem = createTestInMem();

    auto file = factory.createObject("test.txt", "/test.txt", std::move(inmem));
    REQUIRE(file != nullptr);
    REQUIRE(file->getName() == "test.txt");
    REQUIRE(file->getPath() == "/test.txt");

    auto* reg_file = dynamic_cast<IRegularFile*>(file.get());
    REQUIRE(reg_file != nullptr);
}

TEST_CASE("DirectoryFactory createObject") {
    DirectoryFactory factory;
    auto inmem = createTestInMem();

    auto dir = factory.createObject("testdir", "/testdir", std::move(inmem));
    REQUIRE(dir != nullptr);
    REQUIRE(dir->getName() == "testdir");
    REQUIRE(dir->getPath() == "/testdir");

    auto* directory = dynamic_cast<IDirectory*>(dir.get());
    REQUIRE(directory != nullptr);
    REQUIRE(directory->getDirectoryStructure() != nullptr);
}

TEST_CASE("FileFactory creates RegularFile") {
    FileFactory factory;
    auto inmem = createTestInMem();

    auto file = factory.createObject("test.txt", "/test.txt", std::move(inmem));
    auto* reg_file = dynamic_cast<RegularFile*>(file.get());
    REQUIRE(reg_file != nullptr);
}

TEST_CASE("DirectoryFactory creates Directory") {
    DirectoryFactory factory;
    auto inmem = createTestInMem();

    auto dir = factory.createObject("testdir", "/testdir", std::move(inmem));
    auto* directory = dynamic_cast<Directory*>(dir.get());
    REQUIRE(directory != nullptr);
}

