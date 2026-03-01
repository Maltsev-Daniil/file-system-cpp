add_library(fs_console STATIC)

target_sources(fs_console PRIVATE
        ${PROJECT_SOURCE_DIR}/src/cli_view.cpp
        ${PROJECT_SOURCE_DIR}/src/cmd_dispatcher.cpp
        ${PROJECT_SOURCE_DIR}/src/io.cpp
        ${PROJECT_SOURCE_DIR}/src/processor.cpp
)

target_include_directories(fs_console PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(fs_console PUBLIC
        fs_dto
        fs_cmdlets
        fs_alias
)