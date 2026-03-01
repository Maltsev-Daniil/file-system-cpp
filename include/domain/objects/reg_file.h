#pragma once
#include "file_object.h"
#include <string>
#include "../types.h"

namespace domain {
class IRegularFile : public IFileObject {
public:
    virtual ~IRegularFile() = default;

    virtual const std::string& getContent() const = 0;
    virtual LockType getLock() const = 0;
    virtual void setContent(const std::string& content) = 0;
    virtual void setLock(LockType lock) = 0;
};

class RegularFile : public IRegularFile {
private:
    LockType lock;
    std::string content{};
    std::unique_ptr<IInMemoryFile> in_memory;

    std::string name{};
    std::string path{};

public:
    RegularFile(std::unique_ptr<IInMemoryFile> in_memory_file,
                const std::string& name,
                const std::string& path,
                LockType lock_type = LockType::NONE,
                const std::string& initial_content = "");

    const std::string& getContent() const override { return content; }
    LockType getLock() const override { return lock; }
    void setContent(const std::string& content) override { this->content = content; }
    void setLock(LockType lock) override { this->lock = lock; }

    IInMemoryFile* getInMemoryFile() override { return in_memory.get(); }
    const IInMemoryFile* getInMemoryFile() const override { return in_memory.get(); }

    const std::string& getName() const override { return name; }
    void setName(const std::string& new_name) override { name = new_name; }

    const std::string& getPath() const override { return path; }
    void setPath(const std::string& new_path) override { path = new_path; }

    void updatePathRecursive(const std::string &parent_path) override;
    std::unique_ptr<IFileObject> clone() const override;
};
}
