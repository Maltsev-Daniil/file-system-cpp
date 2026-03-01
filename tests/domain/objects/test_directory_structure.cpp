#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "domain/objects/directory_structure.h"
#include "domain/objects/reg_file.h"
#include "domain/objects/inmem_file.h"
#include "factories/objects/acl_fac.h"

using namespace domain;

std::unique_ptr<IFileObject> createTestFile(const std::string& name, const std::string& path) {
    ACLFactory acl_fac;
    auto acl = acl_fac.createDefaultFileACL();
    auto inmem = std::make_unique<InMemoryFile>(1, 0, 0, 0, std::move(acl));
    return std::make_unique<RegularFile>(std::move(inmem), name, path);
}

TEST_CASE("DirectoryStructure addEntry") {
    DirectoryStructure ds;
    auto file = createTestFile("test.txt", "/");
    REQUIRE(ds.addEntry(std::move(file)));
    REQUIRE(ds.size() == 1);
}

TEST_CASE("DirectoryStructure contains") {
    DirectoryStructure ds;
    auto file = createTestFile("test.txt", "/");
    ds.addEntry(std::move(file));
    REQUIRE(ds.contains("test.txt"));
    REQUIRE_FALSE(ds.contains("nonexistent.txt"));
}

TEST_CASE("DirectoryStructure get") {
    DirectoryStructure ds;
    auto file = createTestFile("test.txt", "/");
    ds.addEntry(std::move(file));

    auto* retrieved = ds.get("test.txt");
    REQUIRE(retrieved != nullptr);
    REQUIRE(retrieved->getName() == "test.txt");
}

TEST_CASE("DirectoryStructure get const") {
    DirectoryStructure ds;
    auto file = createTestFile("test.txt", "/");
    ds.addEntry(std::move(file));

    const DirectoryStructure& cds = ds;
    const auto* retrieved = cds.get("test.txt");
    REQUIRE(retrieved != nullptr);
    REQUIRE(retrieved->getName() == "test.txt");
}

TEST_CASE("DirectoryStructure removeEntry") {
    DirectoryStructure ds;
    auto file = createTestFile("test.txt", "/");
    ds.addEntry(std::move(file));
    REQUIRE(ds.size() == 1);

    REQUIRE(ds.removeEntry("test.txt"));
    REQUIRE(ds.size() == 0);
    REQUIRE_FALSE(ds.contains("test.txt"));
}

TEST_CASE("DirectoryStructure renameEntry") {
    DirectoryStructure ds;
    auto file = createTestFile("old.txt", "/");
    ds.addEntry(std::move(file));

    REQUIRE(ds.renameEntry("old.txt", "new.txt"));
    REQUIRE_FALSE(ds.contains("old.txt"));
    REQUIRE(ds.contains("new.txt"));
    REQUIRE(ds.get("new.txt")->getName() == "new.txt");
}

TEST_CASE("DirectoryStructure extract") {
    DirectoryStructure ds;
    auto file = createTestFile("test.txt", "/");
    ds.addEntry(std::move(file));

    auto extracted = ds.extract("test.txt");
    REQUIRE(extracted != nullptr);
    REQUIRE(extracted->getName() == "test.txt");
    REQUIRE_FALSE(ds.contains("test.txt"));
}

TEST_CASE("DirectoryStructure listNames") {
    DirectoryStructure ds;
    ds.addEntry(createTestFile("a.txt", "/"));
    ds.addEntry(createTestFile("b.txt", "/"));
    ds.addEntry(createTestFile("c.txt", "/"));

    auto names = ds.listNames();
    REQUIRE(names.size() == 3);
    REQUIRE(std::find(names.begin(), names.end(), "a.txt") != names.end());
    REQUIRE(std::find(names.begin(), names.end(), "b.txt") != names.end());
    REQUIRE(std::find(names.begin(), names.end(), "c.txt") != names.end());
}

TEST_CASE("DirectoryStructure getObjs") {
    DirectoryStructure ds;
    ds.addEntry(createTestFile("a.txt", "/"));
    ds.addEntry(createTestFile("b.txt", "/"));

    auto objs = ds.getObjs();
    REQUIRE(objs.size() == 2);
}

TEST_CASE("DirectoryStructure clear") {
    DirectoryStructure ds;
    ds.addEntry(createTestFile("a.txt", "/"));
    ds.addEntry(createTestFile("b.txt", "/"));
    REQUIRE(ds.size() == 2);

    ds.clear();
    REQUIRE(ds.size() == 0);
}

TEST_CASE("DirectoryStructure size") {
    DirectoryStructure ds;
    REQUIRE(ds.size() == 0);

    ds.addEntry(createTestFile("a.txt", "/"));
    REQUIRE(ds.size() == 1);

    ds.addEntry(createTestFile("b.txt", "/"));
    REQUIRE(ds.size() == 2);
}

TEST_CASE("DirectoryStructure multiple entries") {
    DirectoryStructure ds;
    ds.addEntry(createTestFile("file1.txt", "/"));
    ds.addEntry(createTestFile("file2.txt", "/"));
    ds.addEntry(createTestFile("file3.txt", "/"));

    REQUIRE(ds.size() == 3);
    REQUIRE(ds.contains("file1.txt"));
    REQUIRE(ds.contains("file2.txt"));
    REQUIRE(ds.contains("file3.txt"));
}

TEST_CASE("DirectoryStructure duplicate add") {
    DirectoryStructure ds;
    auto file1 = createTestFile("test.txt", "/");
    REQUIRE(ds.addEntry(std::move(file1)));

    auto file2 = createTestFile("test.txt", "/");
    REQUIRE_FALSE(ds.addEntry(std::move(file2)));
    REQUIRE(ds.size() == 1);
}

TEST_CASE("DirectoryStructure clone") {
    DirectoryStructure ds;
    ds.addEntry(createTestFile("a.txt", "/"));
    ds.addEntry(createTestFile("b.txt", "/"));

    auto cloned = ds.clone();
    REQUIRE(cloned->size() == 2);
    REQUIRE(cloned->contains("a.txt"));
    REQUIRE(cloned->contains("b.txt"));
}

