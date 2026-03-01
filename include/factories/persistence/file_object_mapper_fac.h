#pragma once

#include <memory>

class IDirectoryMapper;
class IRegularFileMapper;
class IFileObjectMapper;

class FileObjectMapperFactory {
public:
    std::unique_ptr<IFileObjectMapper> create(
        IRegularFileMapper& file_mapper,
        IDirectoryMapper* dir_mapper = nullptr);
};
