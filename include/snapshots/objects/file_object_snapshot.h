#pragma once

#include "file_metadata_snapshot.h"

#include <string>

#include "snapshots/acl/acl_snapshot.h"

class FileObjectSnapshot {
public:
    virtual ~FileObjectSnapshot() = default;

    const std::string& getName() const noexcept { return name; }
    const std::string& getPath() const noexcept { return path; }
    const FileMetadataSnapshot& getMetadata() const noexcept { return metadata; }

protected:
    FileObjectSnapshot(
        std::string name,
        std::string path,
        FileMetadataSnapshot metadata
    )
        : name(std::move(name)),
          path(std::move(path)),
          metadata(std::move(metadata)) {}

private:
    std::string name;
    std::string path;
    FileMetadataSnapshot metadata;
};