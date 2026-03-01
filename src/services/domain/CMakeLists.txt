add_library(fs_domain_services STATIC)

target_sources(fs_domain_services
    PRIVATE
        objects/dir_service.cpp
        objects/file_service.cpp
        ${PROJECT_SOURCE_DIR}/src/services/domain/group_service.cpp
        kernel_service.cpp
        ${PROJECT_SOURCE_DIR}/src/services/domain/rights_service.cpp
        ${PROJECT_SOURCE_DIR}/src/services/domain/user_service.cpp
)

target_include_directories(fs_domain_services
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(fs_domain_services PUBLIC fs_domain_objects fs_domain_tables)