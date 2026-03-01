#pragma once

#include "file_object_snapshot.h"

#include <memory>
#include <vector>

class DirectorySnapshot final : public FileObjectSnapshot {
public:
    DirectorySnapshot(
        std::string name,
        std::string path,
        FileMetadataSnapshot metadata,
        std::vector<std::unique_ptr<FileObjectSnapshot>> children
    )
        : FileObjectSnapshot(
              std::move(name),
              std::move(path),
              std::move(metadata)),
              children(std::move(children)) {}

    const std::vector<std::unique_ptr<FileObjectSnapshot>>&
    getChildren() const noexcept {
        return children;
    }

private:
    std::vector<std::unique_ptr<FileObjectSnapshot>> children;
};