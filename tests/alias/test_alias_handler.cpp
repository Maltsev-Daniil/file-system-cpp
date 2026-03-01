#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "alias/alias_handler.h"

TEST_CASE("AliasHandler add") {
    AliasHandler handler;
    std::vector<std::string> commands = {"ls", "-l"};
    handler.add("ll", commands);
    REQUIRE(handler.exists("ll"));
}

TEST_CASE("AliasHandler exists") {
    AliasHandler handler;
    REQUIRE_FALSE(handler.exists("nonexistent"));

    std::vector<std::string> commands = {"ls"};
    handler.add("ls_alias", commands);
    REQUIRE(handler.exists("ls_alias"));
}

TEST_CASE("AliasHandler get") {
    AliasHandler handler;
    std::vector<std::string> commands = {"ls", "-l", "-a"};
    handler.add("ll", commands);

    const auto& retrieved = handler.get("ll");
    REQUIRE(retrieved.size() == 3);
    REQUIRE(retrieved[0] == "ls");
    REQUIRE(retrieved[1] == "-l");
    REQUIRE(retrieved[2] == "-a");
}

TEST_CASE("AliasHandler remove") {
    AliasHandler handler;
    std::vector<std::string> commands = {"ls"};
    handler.add("ll", commands);
    REQUIRE(handler.exists("ll"));

    handler.remove("ll");
    REQUIRE_FALSE(handler.exists("ll"));
}

TEST_CASE("AliasHandler update") {
    AliasHandler handler;
    std::vector<std::string> commands1 = {"ls"};
    handler.add("ll", commands1);

    std::vector<std::string> commands2 = {"ls", "-l"};
    handler.update("ll", commands2);

    const auto& retrieved = handler.get("ll");
    REQUIRE(retrieved.size() == 2);
    REQUIRE(retrieved[0] == "ls");
    REQUIRE(retrieved[1] == "-l");
}

TEST_CASE("AliasHandler multiple aliases") {
    AliasHandler handler;
    handler.add("ll", {"ls", "-l"});
    handler.add("la", {"ls", "-a"});
    handler.add("lll", {"ls", "-l", "-l"});

    REQUIRE(handler.exists("ll"));
    REQUIRE(handler.exists("la"));
    REQUIRE(handler.exists("lll"));
}

