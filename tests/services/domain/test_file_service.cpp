#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "services/domain/file_service.h"
#include "factories/objects/file_object_fac.h"
#include "factories/objects/inmem_fac.h"
#include "factories/objects/acl_fac.h"
#include "domain/objects/reg_file.h"
#include "domain/objects/inmem_file.h"

using namespace services;
using namespace domain;

TEST_CASE("FileService createFile") {
    auto file_fac = std::make_shared<FileFactory>();
    InodeGenerator inode_gen;
    auto inmem_fac = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_fac = std::make_shared<ACLFactory>();

    FileService service(file_fac, inmem_fac, acl_fac);

    auto file = service.createFile("/", "test.txt", 1000, 2000);

    REQUIRE(file != nullptr);
    REQUIRE(file->getName() == "test.txt");
    REQUIRE(file->getPath() == "/test.txt");
}

TEST_CASE("FileService createFile with subdirectory") {
    auto file_fac = std::make_shared<FileFactory>();
    InodeGenerator inode_gen;
    auto inmem_fac = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_fac = std::make_shared<ACLFactory>();

    FileService service(file_fac, inmem_fac, acl_fac);

    auto file = service.createFile("/home", "test.txt", 1000, 2000);

    REQUIRE(file != nullptr);
    REQUIRE(file->getName() == "test.txt");
    REQUIRE(file->getPath() == "/home/test.txt");
}

TEST_CASE("FileService openFileForRead and closeFile") {
    auto file_fac = std::make_shared<FileFactory>();
    InodeGenerator inode_gen;
    auto inmem_fac = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_fac = std::make_shared<ACLFactory>();

    FileService service(file_fac, inmem_fac, acl_fac);

    auto file = service.createFile("/", "test.txt", 1000, 2000);
    auto* reg_file = dynamic_cast<IRegularFile*>(file.get());
    REQUIRE(reg_file != nullptr);

    service.openFileForRead(file.get());
    REQUIRE(reg_file->getLock() == LockType::READ);

    service.closeFile(file.get());
    REQUIRE(reg_file->getLock() == LockType::NONE);
}

TEST_CASE("FileService openFileForWrite and closeFile") {
    auto file_fac = std::make_shared<FileFactory>();
    InodeGenerator inode_gen;
    auto inmem_fac = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_fac = std::make_shared<ACLFactory>();

    FileService service(file_fac, inmem_fac, acl_fac);

    auto file = service.createFile("/", "test.txt", 1000, 2000);
    auto* reg_file = dynamic_cast<IRegularFile*>(file.get());
    REQUIRE(reg_file != nullptr);

    service.openFileForWrite(file.get());
    REQUIRE(reg_file->getLock() == LockType::WRITE);

    service.closeFile(file.get());
    REQUIRE(reg_file->getLock() == LockType::NONE);
}

TEST_CASE("FileService writeFile and readFile") {
    auto file_fac = std::make_shared<FileFactory>();
    InodeGenerator inode_gen;
    auto inmem_fac = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_fac = std::make_shared<ACLFactory>();

    FileService service(file_fac, inmem_fac, acl_fac);

    auto file = service.createFile("/", "test.txt", 1000, 2000);

    service.openFileForWrite(file.get());
    service.writeFile(file.get(), "Hello, World!");
    service.closeFile(file.get());

    service.openFileForRead(file.get());
    const auto& content = service.readFile(file.get());
    REQUIRE(content == "Hello, World!");
    service.closeFile(file.get());
}

TEST_CASE("FileService appendToFile") {
    auto file_fac = std::make_shared<FileFactory>();
    InodeGenerator inode_gen;
    auto inmem_fac = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_fac = std::make_shared<ACLFactory>();

    FileService service(file_fac, inmem_fac, acl_fac);

    auto file = service.createFile("/", "test.txt", 1000, 2000);

    service.openFileForWrite(file.get());
    service.writeFile(file.get(), "Hello");
    service.closeFile(file.get());

    service.openFileForWrite(file.get());
    service.appendToFile(file.get(), ", World!");
    service.closeFile(file.get());

    service.openFileForRead(file.get());
    const auto& content = service.readFile(file.get());
    REQUIRE(content == "Hello, World!");
    service.closeFile(file.get());
}

TEST_CASE("FileService renameFile") {
    auto file_fac = std::make_shared<FileFactory>();
    InodeGenerator inode_gen;
    auto inmem_fac = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_fac = std::make_shared<ACLFactory>();

    FileService service(file_fac, inmem_fac, acl_fac);

    auto file = service.createFile("/", "old.txt", 1000, 2000);

    service.renameFile(file.get(), "new.txt");
    REQUIRE(file->getName() == "new.txt");
}

TEST_CASE("FileService getSize") {
    auto file_fac = std::make_shared<FileFactory>();
    InodeGenerator inode_gen;
    auto inmem_fac = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_fac = std::make_shared<ACLFactory>();

    FileService service(file_fac, inmem_fac, acl_fac);

    auto file = service.createFile("/", "test.txt", 1000, 2000);

    REQUIRE(service.getSize(file.get()) == 0);

    service.openFileForWrite(file.get());
    service.writeFile(file.get(), "Hello");
    service.closeFile(file.get());

    REQUIRE(service.getSize(file.get()) == 5);
}

TEST_CASE("FileService getFileInfo") {
    auto file_fac = std::make_shared<FileFactory>();
    InodeGenerator inode_gen;
    auto inmem_fac = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_fac = std::make_shared<ACLFactory>();

    FileService service(file_fac, inmem_fac, acl_fac);

    auto file = service.createFile("/", "test.txt", 1000, 2000);

    auto* info = service.getFileInfo(file.get());
    REQUIRE(info != nullptr);
    REQUIRE(info->getUid() == 1000);
    REQUIRE(info->getGid() == 2000);
}

