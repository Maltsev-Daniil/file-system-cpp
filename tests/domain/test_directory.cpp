


#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "domain/objects/directory_structure.h"
#include "domain/objects/reg_file.h"
#include "domain/objects/inmem_file.h"

TEST_CASE("insert/find string + unique_ptr") {
    Tree<std::string, std::unique_ptr<domain::IFileObject>> t;

    auto f = std::make_unique<domain::RegularFile>(nullptr, "foo", "/");
    t.emplace(t.end(), "foo", std::move(f));

    REQUIRE(t.find("foo") != t.end());
}

TEST_CASE("DirectoryStructure add/find") {
    domain::DirectoryStructure ds;
    auto f = std::make_unique<domain::RegularFile>(nullptr, "foo", "/");
    REQUIRE(ds.addEntry(std::move(f)));
    REQUIRE(ds.contains("foo"));
    auto names = ds.listNames();
    for (auto& n : names)
        std::cerr << "name: " << n << "\n";

    REQUIRE(ds.get("foo") != nullptr);
}
