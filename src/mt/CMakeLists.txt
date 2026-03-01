add_library(fs_mt STATIC)

target_sources(fs_mt PRIVATE
thread_pool.cpp)

target_include_directories(fs_mt PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
