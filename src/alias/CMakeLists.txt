add_library(fs_alias STATIC)

target_sources(fs_alias PRIVATE
        ${PROJECT_SOURCE_DIR}/src/alias/alias_handler.cpp
        ${PROJECT_SOURCE_DIR}/src/alias/alias_registry.cpp
)

target_include_directories(fs_alias PUBLIC
        ${PROJECT_SOURCE_DIR}/include)
