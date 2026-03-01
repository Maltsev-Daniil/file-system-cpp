#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <cstdint>
#include "dan_map.h"

namespace domain {

class IFileObject;

template<typename K, typename V>
class IOrderedTable {
    public:
    virtual ~IOrderedTable() = default;
    
    virtual bool insert(const K& key, V value) = 0;
    virtual bool erase(const K& key) = 0;
    
    virtual V* find(const K& key) = 0;
    virtual const V* find(const K& key) const = 0;
    
    virtual size_t size() const = 0;
    virtual void clear() = 0;
    
    virtual std::vector<K> keys() const = 0;

    virtual V extract(const K& key) = 0;
    
    using iterator = typename std::map<K, V>::iterator;
    using const_iterator = typename std::map<K, V>::const_iterator;

    virtual iterator begin() = 0;
    virtual iterator end() = 0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
};

template<typename K, typename V>
class StdMapOrderedTable : public IOrderedTable<K,V> {
    std::map<K, V> map;
    public:
    bool insert(const K& key, V value) override {
        map[key] = std::move(value);
        return true;
    }
    
    bool erase(const K& key) override {
        return map.erase(key) > 0;
    }
    
    V* find(const K& key) override {
        auto it = map.find(key);
        return it == map.end() ? nullptr : &it->second;
    }
    
    const V* find(const K& key) const override {
        auto it = map.find(key);
        return it == map.end() ? nullptr : &it->second;
    }
    
    size_t size() const override { return map.size(); }
    void clear() override { map.clear(); }
    
    std::vector<K> keys() const override {
        std::vector<K> result;
        result.reserve(map.size());
        for (auto& [k, _] : map) result.push_back(k);
        return result;
    }

    V extract(const K& key) override {
        auto it = map.find(key);
        if (it == map.end()) return nullptr;

        V ptr = std::move(it->second);
        map.erase(it);
        return ptr;
    }
    using iterator = typename std::map<K, V>::iterator;
    using const_iterator = typename std::map<K, V>::const_iterator;
    
    iterator begin() override { return map.begin(); }
    iterator end() override { return map.end(); }
    
    const_iterator begin() const override { return map.begin(); }
    const_iterator end() const override { return map.end(); } 
};

class IDirectoryStructure {
public:
    virtual ~IDirectoryStructure() = default;

    virtual bool addEntry(std::unique_ptr<IFileObject> file) = 0;

    virtual bool removeEntry(const std::string& name) = 0;
    virtual bool renameEntry(const std::string& old_name, 
                        const std::string& new_name) = 0;
    virtual bool contains(const std::string& name) const = 0;

    virtual IFileObject* get(const std::string& name) = 0;
    virtual const IFileObject* get(const std::string& name) const = 0;

    virtual std::unique_ptr<IFileObject> extract(const std::string& name) = 0;

    virtual size_t size() const = 0;
    virtual void clear() = 0;

    virtual std::vector<std::string> listNames() const = 0;
    virtual std::vector<IFileObject*> getObjs() const = 0;

    virtual std::unique_ptr<IDirectoryStructure> clone() const = 0;
};


class DirectoryStructure : public IDirectoryStructure {
    using table_t = Tree<std::string, std::unique_ptr<IFileObject>>;
    std::unique_ptr<table_t> table;

public:
    DirectoryStructure();

    bool addEntry(std::unique_ptr<IFileObject> file) override;

    bool removeEntry(const std::string& name) override;
    bool renameEntry(const std::string& old_name, 
                        const std::string& new_name) override;
    bool contains(const std::string& name) const override;

    IFileObject* get(const std::string& name) override;
    const IFileObject* get(const std::string& name) const override;

    std::unique_ptr<IFileObject> extract(const std::string& name) override;

    size_t size() const override;
    void clear() override;

    std::vector<std::string> listNames() const override;
    std::vector<IFileObject*> getObjs() const override;

    std::unique_ptr<IDirectoryStructure> clone() const override;
};

} // namespace domain