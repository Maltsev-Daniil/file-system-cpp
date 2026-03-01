add_library(fs_cmdlets STATIC)

file(GLOB_RECURSE CMDLETS
        ${PROJECT_SOURCE_DIR}/src/cmdlets/*.cpp
)

target_sources(fs_cmdlets PRIVATE
${CMDLETS})

target_include_directories(fs_cmdlets PUBLIC
${PROJECT_SOURCE_DIR}/include)

target_link_libraries(fs_cmdlets PUBLIC fs_application_services fs_dto)