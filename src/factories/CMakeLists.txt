add_library(fs_factories STATIC)

target_sources(fs_factories PRIVATE
        ${PROJECT_SOURCE_DIR}/src/factories/acl_fac.cpp
        ${PROJECT_SOURCE_DIR}/src/factories/file_object_fac.cpp
        ${PROJECT_SOURCE_DIR}/src/factories/group_fac.cpp
        ${PROJECT_SOURCE_DIR}/src/factories/inmem_fac.cpp
        ${PROJECT_SOURCE_DIR}/src/factories/user_fac.cpp

        ${PROJECT_SOURCE_DIR}/src/factories/init/app_services_fac.cpp
        ${PROJECT_SOURCE_DIR}/src/factories/init/cmdlet_fac.cpp
        ${PROJECT_SOURCE_DIR}/src/factories/init/domain_services_fac.cpp
        ${PROJECT_SOURCE_DIR}/src/factories/init/kernel_fac.cpp
        ${PROJECT_SOURCE_DIR}/src/factories/init/kernel_fac_validator.cpp

        ${PROJECT_SOURCE_DIR}/src/factories/persistence/persistence_fac.cpp
)

target_include_directories(fs_factories PUBLIC
${PROJECT_SOURCE_DIR}/include)

target_link_libraries(fs_factories PUBLIC
fs_cmdlets
fs_domain_objects
fs_domain_services
fs_application_services
fs_containers
fs_domain_tables
)