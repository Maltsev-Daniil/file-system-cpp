add_library(fs_mappers STATIC)

file(GLOB_RECURSE MAPPERS
${PROJECT_SOURCE_DIR}/src/mappers/*.cpp)

target_sources(fs_mappers PRIVATE ${MAPPERS})

target_include_directories(fs_mappers
PUBLIC
${PROJECT_SOURCE_DIR}/include)

target_link_libraries(fs_mappers
PUBLIC
fs_domain_objects
fs_factories)