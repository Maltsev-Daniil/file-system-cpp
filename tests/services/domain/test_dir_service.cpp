#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "services/domain/dir_service.h"
#include "factories/objects/file_object_fac.h"
#include "factories/objects/inmem_fac.h"
#include "factories/objects/acl_fac.h"
#include "domain/objects/directory.h"
#include "domain/objects/inmem_file.h"

using namespace services;
using namespace domain;

TEST_CASE("DirectoryService createDirectory") {
    auto dir_fac = std::make_shared<DirectoryFactory>();
    InodeGenerator inode_gen;
    auto inmem_fac = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_fac = std::make_shared<ACLFactory>();

    DirectoryService service(dir_fac, inmem_fac, acl_fac);

    auto dir = service.createDirectory("testdir", "/", 1000, 2000);

    REQUIRE(dir != nullptr);
    REQUIRE(dir->getName() == "testdir");
    REQUIRE(dir->getPath() == "/testdir");
}

TEST_CASE("DirectoryService renameDirectory") {
    auto dir_fac = std::make_shared<DirectoryFactory>();
    InodeGenerator inode_gen;
    auto inmem_fac = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_fac = std::make_shared<ACLFactory>();

    DirectoryService service(dir_fac, inmem_fac, acl_fac);

    auto dir = service.createDirectory("olddir", "/", 1000, 2000);

    service.renameDirectory(dir.get(), "newdir");
    REQUIRE(dir->getName() == "newdir");
}

TEST_CASE("DirectoryService insertEntry and listDirectory") {
    auto dir_fac = std::make_shared<DirectoryFactory>();
    auto file_fac = std::make_shared<FileFactory>();
    InodeGenerator inode_gen;
    auto inmem_fac = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_fac = std::make_shared<ACLFactory>();

    DirectoryService service(dir_fac, inmem_fac, acl_fac);

    auto dir = service.createDirectory("testdir", "/", 1000, 2000);

    auto acl = acl_fac->createDefaultFileACL();
    auto inmem = inmem_fac->createInMemory(1000, 2000, std::move(acl));
    auto file = file_fac->createObject("test.txt", "/testdir/test.txt", std::move(inmem));

    service.insertEntry(std::move(file), dir.get());

    auto entries = service.listDirectory(dir.get());
    REQUIRE(entries.size() == 1);
    REQUIRE(entries[0] == "test.txt");
}

TEST_CASE("DirectoryService removeEntry") {
    auto dir_fac = std::make_shared<DirectoryFactory>();
    auto file_fac = std::make_shared<FileFactory>();
    InodeGenerator inode_gen;
    auto inmem_fac = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_fac = std::make_shared<ACLFactory>();

    DirectoryService service(dir_fac, inmem_fac, acl_fac);

    auto dir = service.createDirectory("testdir", "/", 1000, 2000);

    auto acl = acl_fac->createDefaultFileACL();
    auto inmem = inmem_fac->createInMemory(1000, 2000, std::move(acl));
    auto file = file_fac->createObject("test.txt", "/testdir/test.txt", std::move(inmem));

    service.insertEntry(std::move(file), dir.get());
    REQUIRE(service.listDirectory(dir.get()).size() == 1);

    service.removeEntry("test.txt", dir.get());
    REQUIRE(service.listDirectory(dir.get()).empty());
}

TEST_CASE("DirectoryService renameEntry") {
    auto dir_fac = std::make_shared<DirectoryFactory>();
    auto file_fac = std::make_shared<FileFactory>();
    InodeGenerator inode_gen;
    auto inmem_fac = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_fac = std::make_shared<ACLFactory>();

    DirectoryService service(dir_fac, inmem_fac, acl_fac);

    auto dir = service.createDirectory("testdir", "/", 1000, 2000);

    auto acl = acl_fac->createDefaultFileACL();
    auto inmem = inmem_fac->createInMemory(1000, 2000, std::move(acl));
    auto file = file_fac->createObject("old.txt", "/testdir/old.txt", std::move(inmem));

    service.insertEntry(std::move(file), dir.get());

    service.renameEntry("old.txt", "new.txt", dir.get());

    auto entries = service.listDirectory(dir.get());
    REQUIRE(entries.size() == 1);
    REQUIRE(entries[0] == "new.txt");

    auto* found = service.findEntry("new.txt", dir.get());
    REQUIRE(found != nullptr);
    REQUIRE(found->getName() == "new.txt");
}

TEST_CASE("DirectoryService findEntry") {
    auto dir_fac = std::make_shared<DirectoryFactory>();
    auto file_fac = std::make_shared<FileFactory>();
    InodeGenerator inode_gen;
    auto inmem_fac = std::make_shared<InMemoryFactory>(inode_gen);
    auto acl_fac = std::make_shared<ACLFactory>();

    DirectoryService service(dir_fac, inmem_fac, acl_fac);

    auto dir = service.createDirectory("testdir", "/", 1000, 2000);

    auto acl = acl_fac->createDefaultFileACL();
    auto inmem = inmem_fac->createInMemory(1000, 2000, std::move(acl));
    auto file = file_fac->createObject("test.txt", "/testdir/test.txt", std::move(inmem));

    service.insertEntry(std::move(file), dir.get());

    auto* found = service.findEntry("test.txt", dir.get());
    REQUIRE(found != nullptr);
    REQUIRE(found->getName() == "test.txt");
}

