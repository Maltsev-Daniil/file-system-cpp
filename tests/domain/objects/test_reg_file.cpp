#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "domain/objects/reg_file.h"
#include "domain/objects/inmem_file.h"
#include "factories/objects/acl_fac.h"

using namespace domain;
namespace {
std::unique_ptr<IInMemoryFile> createTestInMem() {
    ACLFactory acl_fac;
    auto acl = acl_fac.createDefaultFileACL();
    return std::make_unique<InMemoryFile>(1, 0, 0, 0, std::move(acl));
}
}

TEST_CASE("RegularFile basic creation") {
    auto inmem = createTestInMem();
    RegularFile file(std::move(inmem), "test.txt", "/test.txt");

    REQUIRE(file.getName() == "test.txt");
    REQUIRE(file.getPath() == "/test.txt");
    REQUIRE(file.getContent().empty());
    REQUIRE(file.getLock() == LockType::NONE);
}

TEST_CASE("RegularFile with content") {
    auto inmem = createTestInMem();
    RegularFile file(std::move(inmem), "test.txt", "/test.txt", LockType::NONE, "hello world");

    REQUIRE(file.getContent() == "hello world");
}

TEST_CASE("RegularFile with lock") {
    auto inmem = createTestInMem();
    RegularFile file(std::move(inmem), "test.txt", "/test.txt", LockType::READ);

    REQUIRE(file.getLock() == LockType::READ);
}

TEST_CASE("RegularFile setContent") {
    auto inmem = createTestInMem();
    RegularFile file(std::move(inmem), "test.txt", "/test.txt");

    file.setContent("new content");
    REQUIRE(file.getContent() == "new content");
}

TEST_CASE("RegularFile setLock") {
    auto inmem = createTestInMem();
    RegularFile file(std::move(inmem), "test.txt", "/test.txt");

    file.setLock(LockType::WRITE);
    REQUIRE(file.getLock() == LockType::WRITE);
}

TEST_CASE("RegularFile setName") {
    auto inmem = createTestInMem();
    RegularFile file(std::move(inmem), "old.txt", "/old.txt");

    file.setName("new.txt");
    REQUIRE(file.getName() == "new.txt");
}

TEST_CASE("RegularFile setPath") {
    auto inmem = createTestInMem();
    RegularFile file(std::move(inmem), "test.txt", "/old/test.txt");

    file.setPath("/new/test.txt");
    REQUIRE(file.getPath() == "/new/test.txt");
}

TEST_CASE("RegularFile getInMemoryFile") {
    auto inmem = createTestInMem();
    RegularFile file(std::move(inmem), "test.txt", "/test.txt");

    REQUIRE(file.getInMemoryFile() != nullptr);
}

TEST_CASE("RegularFile getInMemoryFile const") {
    auto inmem = createTestInMem();
    RegularFile file(std::move(inmem), "test.txt", "/test.txt");

    const RegularFile& cfile = file;
    REQUIRE(cfile.getInMemoryFile() != nullptr);
}

TEST_CASE("RegularFile updatePathRecursive") {
    auto inmem = createTestInMem();
    RegularFile file(std::move(inmem), "test.txt", "/old/test.txt");

    file.updatePathRecursive("/new");
    REQUIRE(file.getPath() == "/new/test.txt");
}

TEST_CASE("RegularFile clone") {
    auto inmem = createTestInMem();
    RegularFile file(std::move(inmem), "test.txt", "/test.txt", LockType::READ, "content");

    auto cloned = file.clone();
    REQUIRE(cloned->getName() == "test.txt");
    REQUIRE(cloned->getPath() == "/test.txt");
    REQUIRE(static_cast<const IRegularFile*>(cloned.get())->getContent() == "content");
    REQUIRE(static_cast<const IRegularFile*>(cloned.get())->getLock() == LockType::READ);
}

TEST_CASE("RegularFile all lock types") {
    auto inmem1 = createTestInMem();
    RegularFile file1(std::move(inmem1), "f1.txt", "/f1.txt", LockType::NONE);
    REQUIRE(file1.getLock() == LockType::NONE);

    auto inmem2 = createTestInMem();
    RegularFile file2(std::move(inmem2), "f2.txt", "/f2.txt", LockType::READ);
    REQUIRE(file2.getLock() == LockType::READ);

    auto inmem3 = createTestInMem();
    RegularFile file3(std::move(inmem3), "f3.txt", "/f3.txt", LockType::WRITE);
    REQUIRE(file3.getLock() == LockType::WRITE);

    auto inmem4 = createTestInMem();
    RegularFile file4(std::move(inmem4), "f4.txt", "/f4.txt", LockType::READ_WRITE);
    REQUIRE(file4.getLock() == LockType::READ_WRITE);
}

