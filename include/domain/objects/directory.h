#pragma once
#include <memory>
#include <string>

#include "file_object.h"

namespace domain {
class IDirectoryStructure;
class IInMemoryFile;

class IDirectory : public IFileObject {
public:
    virtual ~IDirectory() = default;

    virtual const IDirectoryStructure* getDirectoryStructure() const = 0;
    virtual IDirectoryStructure* getDirectoryStructure() = 0;

    virtual IInMemoryFile* getInMemoryFile() = 0;
    virtual const IInMemoryFile* getInMemoryFile() const = 0;
};

class Directory : public IDirectory {
private:
    std::unique_ptr<IInMemoryFile> in_memory;
    std::unique_ptr<IDirectoryStructure> dir_struct;

    std::string name;
    std::string path;

public:
    Directory(std::unique_ptr<IInMemoryFile> in_memory_file,
              std::unique_ptr<IDirectoryStructure> directory_structure,
              const std::string& name,
              const std::string& path);

    ~Directory();

    const IDirectoryStructure* getDirectoryStructure() const override { return dir_struct.get(); }
    IDirectoryStructure* getDirectoryStructure() override { return dir_struct.get(); }

    IInMemoryFile* getInMemoryFile() override { return in_memory.get(); }
    const IInMemoryFile* getInMemoryFile() const override { return in_memory.get(); }

    const std::string& getName() const override { return name; }
    void setName(const std::string& new_name) override { name = new_name; }

    const std::string& getPath() const override { return path; }
    void setPath(const std::string& new_path) override { path = new_path; }

    void updatePathRecursive(const std::string& parent_path) override;
    std::unique_ptr<IFileObject> clone() const override;
};
}
