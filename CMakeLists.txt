cmake_minimum_required(VERSION 3.20)
project(fs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

find_package(yaml-cpp REQUIRED)

add_subdirectory(tests)

add_subdirectory(src/domain/objects)
add_subdirectory(src/domain/tables)
add_subdirectory(src/containers)
add_subdirectory(src/cmdlets)
add_subdirectory(src/services/application)
add_subdirectory(src/services/domain)
add_subdirectory(src/mappers)
add_subdirectory(src/serializer)
add_subdirectory(src/factories)
add_subdirectory(src/alias)
add_subdirectory(src/dto)
add_subdirectory(src/mt)
add_subdirectory(src)

add_executable(fs
        main.cpp
        include/factories/persistence/file_object_mapper_fac.h
)

target_include_directories(fs
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/debug.h
)

target_link_libraries(fs
        PRIVATE
        fs_console
        fs_containers
        fs_mappers
        fs_serializer
        fs_factories
)