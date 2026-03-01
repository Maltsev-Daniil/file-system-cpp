#pragma once
#include <memory>

namespace domain {
class IInMemoryFile;

class IFileObject {
public:
    virtual ~IFileObject() = default;

    virtual IInMemoryFile* getInMemoryFile() = 0;
    virtual const IInMemoryFile* getInMemoryFile() const = 0;

    virtual const std::string& getName() const = 0;
    virtual void setName(const std::string& new_name) = 0;

    virtual const std::string& getPath() const = 0;
    virtual void setPath(const std::string& new_path) = 0;

    virtual void updatePathRecursive(
        const std::string& parent_path) = 0;

    virtual std::unique_ptr<IFileObject> clone() const = 0;
};
}
