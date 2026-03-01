add_library(fs_serializer STATIC)

target_sources(fs_serializer PRIVATE
${PROJECT_SOURCE_DIR}/src/serializer/yaml_serializer.cpp)

target_include_directories(fs_serializer PUBLIC
${PROJECT_SOURCE_DIR}/include)

target_link_libraries(fs_serializer PUBLIC
fs_dto
yaml-cpp)