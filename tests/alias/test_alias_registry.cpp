#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "alias/alias_registry.h"
#include "alias/alias_handler.h"

TEST_CASE("AliasRegistry registerAlias") {
    AliasHandler handler;
    AliasRegistry registry(handler);

    std::vector<std::string> tokens = {"alias", "ll=ls -l"};
    registry.registerAlias(tokens);

    REQUIRE(handler.exists("ll"));
}

TEST_CASE("AliasRegistry expandIfAlias") {
    AliasHandler handler;
    AliasRegistry registry(handler);

    handler.add("ll", {"ls", "-l"});

    auto expanded = registry.expandIfAlias("ll");
    REQUIRE(expanded.size() == 2);
    REQUIRE(expanded[0] == "ls");
    REQUIRE(expanded[1] == "-l");
}

TEST_CASE("AliasRegistry expandIfAlias no alias") {
    AliasHandler handler;
    AliasRegistry registry(handler);

    auto expanded = registry.expandIfAlias("ls");
    REQUIRE(expanded.size() == 1);
    REQUIRE(expanded[0] == "ls");
}

TEST_CASE("AliasRegistry expandIfAlias with args") {
    AliasHandler handler;
    AliasRegistry registry(handler);

    handler.add("ll", {"ls", "-l"});

    auto expanded = registry.expandIfAlias("ll /tmp");
    REQUIRE(expanded.size() >= 2);
    REQUIRE(expanded[0] == "ls");
    REQUIRE(expanded[1] == "-l");
}

