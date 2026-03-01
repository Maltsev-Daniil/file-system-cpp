add_library(fs_domain_objects STATIC)

target_sources(fs_domain_objects
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/domain/objects/acl.cpp
        ${PROJECT_SOURCE_DIR}/src/domain/objects/directory.cpp
        ${PROJECT_SOURCE_DIR}/src/domain/objects/directory_structure.cpp
        ${PROJECT_SOURCE_DIR}/src/domain/objects/inmem_file.cpp
        ${PROJECT_SOURCE_DIR}/src/domain/objects/reg_file.cpp)

target_include_directories(fs_domain_objects
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include)