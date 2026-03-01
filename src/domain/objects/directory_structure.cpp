#include "../../../include/domain/objects/directory_structure.h"
#include "../../../include/domain/objects/file_object.h"
#include <cstdio>
#include "domain/objects/dan_map.h"

namespace domain {

DirectoryStructure::DirectoryStructure()
    : table(std::make_unique<Tree<
                std::string,
                std::unique_ptr<IFileObject>>>())
{
}

bool DirectoryStructure::addEntry(std::unique_ptr<IFileObject> file)
{
    if (!file)
        return false;

    const std::string& name = file->getName();

    if (table->find(name) != table->end())
        return false;

    auto result = table->emplace(table->end(), name, std::move(file));

    assert(result.second && "addEntry: emplace failed");
    assert(table->find(name) != table->end() && "addEntry: inserted element not found");

    return result.second;
}


bool DirectoryStructure::removeEntry(const std::string& name)
{
    return table->erase(name);
}

bool DirectoryStructure::renameEntry(const std::string& old_name,
                                     const std::string& new_name)
{
    if (old_name == new_name)
        return true;

    if (table->find(new_name) != table->end())
        return false;

    auto it = table->find(old_name);
    if (it == table->end())
        return false;

    auto file = std::move(it->second);
    file->setName(new_name);
    table->erase(it);

    table->emplace(table->end(), new_name, std::move(file));
    return true;
}

bool DirectoryStructure::contains(const std::string& name) const
{
    return table->find(name) != table->end();
}

IFileObject* DirectoryStructure::get(const std::string& name)
{
    auto ptr = table->find(name);
    return ptr != table->end() ? ptr->second.get() : nullptr;
}

const IFileObject* DirectoryStructure::get(const std::string& name) const
{
    auto ptr = table->find(name);
    return ptr != table->end() ? ptr->second.get() : nullptr;
}

std::unique_ptr<IFileObject> DirectoryStructure::extract(const std::string& name)
{
    auto it = table->find(name);
    if (it == table->end())
        return nullptr;

    auto extracted = std::move(it->second);
    table->erase(it);
    return extracted;
}


size_t DirectoryStructure::size() const
{
    return table->size();
}

void DirectoryStructure::clear()
{
    table->clear();
}

std::vector<std::string> DirectoryStructure::listNames() const
{
    return table->keys();
}

std::vector<IFileObject *> DirectoryStructure::getObjs() const
{
    std::vector<IFileObject*> result;
    result.reserve(table->size());
    for (const auto& [name, obj] : *table) {
        result.push_back(obj.get());
    }
    return result;
}

std::unique_ptr<IDirectoryStructure> DirectoryStructure::clone() const
{
    auto copy = std::make_unique<DirectoryStructure>();

    for (const auto& [name, obj] : *table) {
        copy->addEntry(obj->clone());
    }

    return copy;
}

} // namespace domain