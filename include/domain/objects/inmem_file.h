#pragma once
#include <memory>

#include "../types.h"

namespace domain {
class IACL;

class IInMemoryFile {
public:
    virtual ~IInMemoryFile() = default;

    virtual userId_t getUid() const = 0;
    virtual void setUid(userId_t) = 0;
    virtual groupId_t getGid() const = 0;
    virtual void setGid(groupId_t) = 0;
    virtual TimePoint getModTime() const = 0;
    virtual TimePoint getCreateTime() const = 0;
    virtual size_t getSize() const = 0;
    virtual void setSize(size_t new_size) = 0;
    virtual void updateModificationTime() = 0;
    virtual inode_t getId() const = 0;
    virtual IACL* getAcl() = 0;
    virtual const IACL* getAcl() const = 0;
    virtual void setId(inode_t id) = 0;

    virtual std::unique_ptr<IInMemoryFile> clone() const = 0;
};

class InMemoryFile : public IInMemoryFile {
private:
    TimePoint creation_time;
    TimePoint modification_time;
    userId_t uid;
    groupId_t gid;
    std::unique_ptr<IACL> acl;
    size_t size;
    inode_t id;

public:
    InMemoryFile(inode_t id,
                 userId_t uid = 0,
                 groupId_t gid = 0,
                 size_t size = 0,
                 std::unique_ptr<IACL> acl = nullptr,
                 TimePoint creation_time = std::chrono::system_clock::now(),
                 TimePoint modification_time = std::chrono::system_clock::now());

    ~InMemoryFile();

    userId_t getUid() const override { return uid; }
    void setUid(userId_t uid) override { this->uid = uid; }
    groupId_t getGid() const override { return gid; }
    void setGid(groupId_t gid) override { this->gid = gid; }
    TimePoint getModTime() const override { return modification_time; }
    TimePoint getCreateTime() const override { return creation_time; }
    size_t getSize() const override { return size; }
    void setSize(size_t new_size) override { size = new_size; }
    void updateModificationTime() override { modification_time = std::chrono::system_clock::now(); }
    inode_t getId() const override { return id; }
    void setId(inode_t id) override { this->id = id; }
    IACL* getAcl() override { return acl.get(); }
    const IACL* getAcl() const override { return acl.get(); }

    // TODO важно учитывать что копирует уникальный inode
    std::unique_ptr<IInMemoryFile> clone() const override;
};
}
