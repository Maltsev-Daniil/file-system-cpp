add_library(fs_application_services STATIC)

target_sources(fs_application_services PRIVATE
        fs/app_filesystem_persistance_service.cpp
        ${PROJECT_SOURCE_DIR}/src/services/application/fs/app_filesystem_service.cpp
        ${PROJECT_SOURCE_DIR}/src/services/application/objects/app_file_service.cpp
        ${PROJECT_SOURCE_DIR}/src/services/application/objects/app_both_dirnfile_service.cpp
        ${PROJECT_SOURCE_DIR}/src/services/application/objects/app_dir_service.cpp
        ${PROJECT_SOURCE_DIR}/src/services/application/app_group_service.cpp
        ${PROJECT_SOURCE_DIR}/src/services/application/app_user_service.cpp
        ${PROJECT_SOURCE_DIR}/src/services/application/app_rights_service.cpp

        # запихнул session and path resolver сюда же
        ${PROJECT_SOURCE_DIR}/src/services/shell/path_pattern_resolver.cpp
        ${PROJECT_SOURCE_DIR}/src/services/session/session_service.cpp
)

target_include_directories(fs_application_services PUBLIC
${PROJECT_SOURCE_DIR}/include)

target_link_libraries(fs_application_services PUBLIC
fs_domain_services
fs_mt)