add_library(fs_dto STATIC)

target_sources(fs_dto PRIVATE
        ${PROJECT_SOURCE_DIR}/src/dto/file_acl_dto.cpp
        ${PROJECT_SOURCE_DIR}/src/dto/stat_file_dto.cpp
)

target_include_directories(fs_dto PUBLIC

        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(fs_dto PUBLIC fs_domain_objects)