#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "factories/persistence/persistence_fac.h"
#include "containers/persistence_container.h"
#include "factories/objects/acl_fac.h"
#include "factories/objects/inmem_fac.h"
#include "factories/init/kernel_fac.h"
#include "factories/entity/user_fac.h"
#include "factories/entity/group_fac.h"
#include "factories/objects/file_object_fac.h"
#include "services/session/session_service.h"

using namespace domain;
using namespace services;



TEST_CASE("PersistenceServiceFactory construction") {


    REQUIRE(true);
}

