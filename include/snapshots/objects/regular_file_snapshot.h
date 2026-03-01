#pragma once

#include "file_object_snapshot.h"

class RegularFileSnapshot final : public FileObjectSnapshot {
public:
    RegularFileSnapshot(
        std::string name,
        std::string path,
        FileMetadataSnapshot metadata,
        std::string content
    )
        : FileObjectSnapshot(
              std::move(name),
              std::move(path),
              std::move(metadata)),
          content_(std::move(content)) {}

    const std::string& getContent() const noexcept {
        return content_;
    }

private:
    std::string content_;
};