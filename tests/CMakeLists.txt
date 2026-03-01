find_package(Catch2 3 REQUIRED)

set(TEST_SOURCES
    domain/test_container_template.cpp
    domain/test_directory.cpp
    domain/entity/test_user.cpp
    domain/entity/test_group.cpp
    domain/objects/test_acl.cpp
    domain/objects/test_directory_structure.cpp
    domain/objects/test_inmem_file.cpp
    domain/objects/test_reg_file.cpp
    domain/tables/test_user_table.cpp
    domain/tables/test_group_table.cpp

    cmdlets/test_cmdlets_basic.cpp
    
    factories/entity/test_user_fac.cpp
    factories/entity/test_group_fac.cpp
    factories/objects/test_inmem_fac.cpp
    factories/objects/test_file_object_fac.cpp
    factories/init/test_kernel_fac.cpp
    factories/init/test_app_services_fac.cpp
    factories/init/test_domain_services_fac.cpp
    factories/persistence/test_persistence_fac.cpp
    
    alias/test_alias_handler.cpp
    alias/test_alias_registry.cpp
    
    dto/test_file_acl_dto.cpp
    dto/test_stat_file_dto.cpp
    dto/fs/test_disk_usage_dto.cpp
    
    mappers/entity/test_user_mapper.cpp
    mappers/entity/test_group_mapper.cpp
    mappers/acl/test_acl_mapper.cpp
    
    serializer/test_yaml_serializer.cpp
    
    services/domain/test_file_service.cpp
    services/domain/test_dir_service.cpp
    services/domain/test_user_service.cpp
    services/domain/test_group_service.cpp
    services/domain/test_rights_service.cpp
    services/domain/test_kernel_service.cpp
    services/application/test_app_user_service.cpp
    services/application/test_app_group_service.cpp
    services/application/test_app_file_service.cpp
    services/application/test_app_dir_service.cpp
    services/application/test_app_dirnfile_service.cpp
    services/application/test_app_rights_service.cpp

        containers/test_persistence_container.cpp
    
    mt/test_thread_pool.cpp
)

add_executable(test_all ${TEST_SOURCES}
fs_for_test_cmdlets.cpp)

target_link_libraries(test_all
    PRIVATE
    Catch2::Catch2WithMain
)

target_include_directories(test_all
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(test_all
    PRIVATE
    fs_domain_objects
    fs_domain_tables
    fs_containers
    fs_factories
    fs_alias
    fs_dto
    fs_mappers
    fs_serializer
    fs_domain_services
    fs_application_services
    fs_mt
    fs_cmdlets
    fs_console
    yaml-cpp
)

add_compile_options(
    -O0
    -g
    --coverage
)

add_link_options(
    --coverage
)

include(CTest)
include(Catch)
catch_discover_tests(test_all)
