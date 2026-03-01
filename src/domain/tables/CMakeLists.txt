add_library(fs_domain_tables STATIC)

target_sources(fs_domain_tables
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/domain/tables/group_table.cpp
        ${PROJECT_SOURCE_DIR}/src/domain/tables/user_table.cpp
)

target_include_directories(fs_domain_tables
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include)


# аккуратно мб надо линкануть с объектами