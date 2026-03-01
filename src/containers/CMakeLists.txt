add_library(fs_containers STATIC)

target_sources(fs_containers PRIVATE
    ${PROJECT_SOURCE_DIR}/src/containers/system_container.cpp
        ${PROJECT_SOURCE_DIR}/src/containers/persistence_container.cpp
)

target_include_directories(fs_containers PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(fs_containers PUBLIC
    fs_factories
        fs_mappers
        # возможно надо console для dispatcher, будет циклическая зависимость
)