#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "containers/persistence_container.h"
#include "factories/objects/acl_fac.h"
#include "factories/objects/inmem_fac.h"
#include "mappers/acl/acl_mapper.h"
#include "mappers/entity/user_mapper.h"
#include "mappers/entity/group_mapper.h"
#include "mappers/file_system/file_system_mapper.h"
#include "mappers/objects/file_object_mapper.h"
#include "mappers/objects/metadata_mapper.h"
#include "mappers/objects/regfile_mapper.h"
#include "mappers/objects/directory_mapper.h"
#include "serializer/yaml_serializer.h"

using namespace domain;

TEST_CASE("PersistenceContainer construction") {
    ACLFactory acl_factory;
    InodeGenerator inode_gen;
    InMemoryFactory inmem_factory(inode_gen);

    PersistenceContainer container(acl_factory, inmem_factory);

    REQUIRE(container.getFsMapper() != nullptr);
    REQUIRE(container.getSerializer() != nullptr);
}

TEST_CASE("PersistenceContainer getFsMapper") {
    ACLFactory acl_factory;
    InodeGenerator inode_gen;
    InMemoryFactory inmem_factory(inode_gen);

    PersistenceContainer container(acl_factory, inmem_factory);

    auto* mapper = container.getFsMapper();
    REQUIRE(mapper != nullptr);
}

TEST_CASE("PersistenceContainer getSerializer") {
    ACLFactory acl_factory;
    InodeGenerator inode_gen;
    InMemoryFactory inmem_factory(inode_gen);

    PersistenceContainer container(acl_factory, inmem_factory);

    auto* serializer = container.getSerializer();
    REQUIRE(serializer != nullptr);
}

