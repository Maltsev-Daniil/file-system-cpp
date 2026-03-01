#include "serializer/yaml_serializer.h"

#include <fstream>
#include <stdexcept>

#include "snapshots/entity/user_snapshot.h"
#include "snapshots/entity/group_snapshot.h"
#include "snapshots/objects/directory_snapshot.h"
#include "snapshots/objects/regular_file_snapshot.h"
#include "snapshots/file_system/file_system_snapshot.h"

void YamlFileSystemSerializer::requireNode(
    const YAML::Node& node,
    const std::string& field
) const {
    if (!node || !node[field]) {
        throw std::logic_error("missing required field: " + field);
    }
}

std::string YamlFileSystemSerializer::rightTypeToString(
    domain::RightType type
) const {
    switch (type) {
        case domain::RightType::READ: return "read";
        case domain::RightType::WRITE: return "write";
        case domain::RightType::EXECUTE: return "execute";
        case domain::RightType::CHANGE_RIGHTS: return "change_rights";
        case domain::RightType::CHANGE_OWNER: return "change_owner";
        case domain::RightType::USER_MODIFY: return "user_modify";
    }
    throw std::logic_error("invalid RightType");
}

domain::RightType YamlFileSystemSerializer::stringToRightType(
    const std::string& value
) const {
    if (value == "read") return domain::RightType::READ;
    if (value == "write") return domain::RightType::WRITE;
    if (value == "execute") return domain::RightType::EXECUTE;
    if (value == "change_rights") return domain::RightType::CHANGE_RIGHTS;
    if (value == "change_owner") return domain::RightType::CHANGE_OWNER;
    if (value == "user_modify") return domain::RightType::USER_MODIFY;
    throw std::logic_error("invalid RightType string");
}

std::string YamlFileSystemSerializer::accessTypeToString(
    domain::AccessType type
) const {
    if (type == domain::AccessType::ALLOW) return "allow";
    if (type == domain::AccessType::DENY) return "deny";
    throw std::logic_error("invalid AccessType");
}

domain::AccessType YamlFileSystemSerializer::stringToAccessType(
    const std::string& value
) const {
    if (value == "allow") return domain::AccessType::ALLOW;
    if (value == "deny") return domain::AccessType::DENY;
    throw std::logic_error("invalid AccessType string");
}

std::string YamlFileSystemSerializer::subjectKindToString(
    AclSubjectKind kind
) const {
    switch (kind) {
        case AclSubjectKind::OwnerUser: return "owner_user";
        case AclSubjectKind::OwnerGroup: return "owner_group";
        case AclSubjectKind::Other: return "other";
        case AclSubjectKind::UserSpec: return "user";
        case AclSubjectKind::GroupSpec: return "group";
    }
    throw std::logic_error("invalid AclSubjectKind");
}

AclSubjectKind YamlFileSystemSerializer::stringToSubjectKind(
    const std::string& value
) const {
    if (value == "owner_user") return AclSubjectKind::OwnerUser;
    if (value == "owner_group") return AclSubjectKind::OwnerGroup;
    if (value == "other") return AclSubjectKind::Other;
    if (value == "user") return AclSubjectKind::UserSpec;
    if (value == "group") return AclSubjectKind::GroupSpec;
    throw std::logic_error("invalid AclSubjectKind string");
}

YAML::Node YamlFileSystemSerializer::serializeAclItem(
    const AclItemSnapshot& item
) const {
    YAML::Node node;
    const auto& subject = item.getSubject();
    auto kind = subject.getKind();

    node["subject"]["kind"] = subjectKindToString(kind);

    if (kind == AclSubjectKind::UserSpec ||
        kind == AclSubjectKind::GroupSpec) {
        node["subject"]["id"] = subject.getId();
    }

    node["permission"] = rightTypeToString(item.getPerm());
    node["access"] = accessTypeToString(item.getType());
    return node;
}

AclItemSnapshot YamlFileSystemSerializer::deserializeAclItem(
    const YAML::Node& node
) const {
    requireNode(node, "subject");
    requireNode(node["subject"], "kind");
    requireNode(node, "permission");
    requireNode(node, "access");

    AclSubjectKind kind =
        stringToSubjectKind(
            node["subject"]["kind"].as<std::string>()
        );

    std::uint64_t id = 0;

    if (kind == AclSubjectKind::UserSpec ||
        kind == AclSubjectKind::GroupSpec) {
        requireNode(node["subject"], "id");
        id = node["subject"]["id"].as<std::uint64_t>();
    }

    return AclItemSnapshot(
        AclSubjectSnapshot(kind, id),
        stringToRightType(node["permission"].as<std::string>()),
        stringToAccessType(node["access"].as<std::string>())
    );
}

YAML::Node YamlFileSystemSerializer::serializeAcl(
    const AclSnapshot& acl
) const {
    YAML::Node node(YAML::NodeType::Sequence);
    for (const auto& item : acl.getItems()) {
        node.push_back(serializeAclItem(item));
    }
    return node;
}

std::unique_ptr<AclSnapshot> YamlFileSystemSerializer::deserializeAcl(
    const YAML::Node& node
) const {
    if (!node || !node.IsSequence()) {
        throw std::logic_error("acl must be sequence");
    }

    std::vector<AclItemSnapshot> items;
    for (const auto& it : node) {
        items.push_back(deserializeAclItem(it));
    }

    return std::make_unique<AclSnapshot>(std::move(items));
}

YAML::Node YamlFileSystemSerializer::serializeMetadata(
    const FileMetadataSnapshot& meta
) const {
    YAML::Node node;
    node["id"] = meta.getId();
    node["created"] = meta.getCreatedAt();
    node["modified"] = meta.getModifiedAt();
    node["uid"] = meta.getOwnerUserId();
    node["gid"] = meta.getOwnerGroupId();
    node["size"] = meta.getSize();
    node["acl"] = serializeAcl(*meta.getAcl());
    return node;
}

FileMetadataSnapshot YamlFileSystemSerializer::deserializeMetadata(
    const YAML::Node& node
) const {
    requireNode(node, "id");
    requireNode(node, "created");
    requireNode(node, "modified");
    requireNode(node, "uid");
    requireNode(node, "gid");
    requireNode(node, "size");
    requireNode(node, "acl");

    return FileMetadataSnapshot(
        node["id"].as<std::size_t>(),
        node["created"].as<std::uint64_t>(),
        node["modified"].as<std::uint64_t>(),
        node["uid"].as<std::uint64_t>(),
        node["gid"].as<std::uint64_t>(),
        node["size"].as<std::uint64_t>(),
        deserializeAcl(node["acl"])
    );
}

YAML::Node YamlFileSystemSerializer::serializeFileObject(
    const FileObjectSnapshot& obj
) const {
    YAML::Node node;
    node["name"] = obj.getName();
    node["path"] = obj.getPath();
    node["metadata"] = serializeMetadata(obj.getMetadata());

    if (auto file = dynamic_cast<const RegularFileSnapshot*>(&obj)) {
        node["type"] = "file";
        node["content"] = file->getContent();
        return node;
    }

    if (auto dir = dynamic_cast<const DirectorySnapshot*>(&obj)) {
        node["type"] = "dir";
        YAML::Node children(YAML::NodeType::Sequence);
        for (const auto& ch : dir->getChildren()) {
            children.push_back(serializeFileObject(*ch));
        }
        node["children"] = children;
        return node;
    }

    throw std::logic_error("unknown FileObjectSnapshot type");
}

std::unique_ptr<FileObjectSnapshot>
YamlFileSystemSerializer::deserializeFileObject(
    const YAML::Node& node
) const {
    requireNode(node, "type");
    requireNode(node, "name");
    requireNode(node, "path");
    requireNode(node, "metadata");

    auto metadata = deserializeMetadata(node["metadata"]);
    std::string type = node["type"].as<std::string>();

    if (type == "file") {
        requireNode(node, "content");
        return std::make_unique<RegularFileSnapshot>(
            node["name"].as<std::string>(),
            node["path"].as<std::string>(),
            std::move(metadata),
            node["content"].as<std::string>()
        );
    }

    if (type == "dir") {
        requireNode(node, "children");
        if (!node["children"].IsSequence()) {
            throw std::logic_error("children must be sequence");
        }

        std::vector<std::unique_ptr<FileObjectSnapshot>> children;
        for (const auto& ch : node["children"]) {
            children.push_back(deserializeFileObject(ch));
        }

        return std::make_unique<DirectorySnapshot>(
            node["name"].as<std::string>(),
            node["path"].as<std::string>(),
            std::move(metadata),
            std::move(children)
        );
    }

    throw std::logic_error("invalid file object type");
}

YAML::Node YamlFileSystemSerializer::serializeUser(
    const UserSnapshot& user
) const {
    YAML::Node node;
    node["id"] = user.getUid();
    node["name"] = user.getName();
    node["main_group"] = user.getMainGroupId();

    YAML::Node secondary(YAML::NodeType::Sequence);
    for (auto gid : user.getSecondaryGroups()) {
        secondary.push_back(gid);
    }
    node["secondary_groups"] = secondary;
    return node;
}

UserSnapshot YamlFileSystemSerializer::deserializeUser(
    const YAML::Node& node
) const {
    requireNode(node, "id");
    requireNode(node, "name");
    requireNode(node, "main_group");
    requireNode(node, "secondary_groups");

    if (!node["secondary_groups"].IsSequence()) {
        throw std::logic_error("secondary_groups must be sequence");
    }

    std::vector<std::uint64_t> groups;
    for (const auto& g : node["secondary_groups"]) {
        groups.push_back(g.as<std::uint64_t>());
    }

    return UserSnapshot(
        node["id"].as<std::uint64_t>(),
        node["name"].as<std::string>(),
        node["main_group"].as<std::uint64_t>(),
        std::move(groups)
    );
}

YAML::Node YamlFileSystemSerializer::serializeGroup(
    const GroupSnapshot& group
) const {
    YAML::Node node;
    node["id"] = group.getGid();
    node["name"] = group.getName();

    YAML::Node users(YAML::NodeType::Sequence);
    for (auto uid : group.getUsers()) {
        users.push_back(uid);
    }
    node["users"] = users;

    YAML::Node sub(YAML::NodeType::Sequence);
    for (auto gid : group.getSubGroups()) {
        sub.push_back(gid);
    }
    node["sub_groups"] = sub;

    return node;
}

GroupSnapshot YamlFileSystemSerializer::deserializeGroup(
    const YAML::Node& node
) const {
    requireNode(node, "id");
    requireNode(node, "name");
    requireNode(node, "users");
    requireNode(node, "sub_groups");

    if (!node["users"].IsSequence() ||
        !node["sub_groups"].IsSequence()) {
        throw std::logic_error("group lists must be sequence");
    }

    std::vector<std::uint64_t> users;
    for (const auto& u : node["users"]) {
        users.push_back(u.as<std::uint64_t>());
    }

    std::vector<std::uint64_t> sub_groups;
    for (const auto& g : node["sub_groups"]) {
        sub_groups.push_back(g.as<std::uint64_t>());
    }

    return GroupSnapshot(
        node["id"].as<std::uint64_t>(),
        node["name"].as<std::string>(),
        std::move(users),
        std::move(sub_groups)
    );
}

void YamlFileSystemSerializer::write(
    const std::string& path,
    const FileSystemSnapshot& snapshot
) const {
    YAML::Node root;
    root["root"] = serializeFileObject(snapshot.getRoot());

    YAML::Node users(YAML::NodeType::Sequence);
    for (const auto& u : snapshot.getUsers()) {
        users.push_back(serializeUser(u));
    }
    root["users"] = users;

    YAML::Node groups(YAML::NodeType::Sequence);
    for (const auto& g : snapshot.getGroups()) {
        groups.push_back(serializeGroup(g));
    }
    root["groups"] = groups;

    std::ofstream out(path);
    if (!out.is_open()) {
        throw std::runtime_error("yaml_serializer: cannot open file '" + path + "' for writing");
    }

    out << root;
    
    if (out.fail() || out.bad()) {
        out.close();
        throw std::runtime_error("yaml_serializer: failed to write to file '" + path + "'");
    }

    out.close();
}

FileSystemSnapshot YamlFileSystemSerializer::read(
    const std::string& path
) const {
    std::ifstream in(path);
    if (!in.is_open()) {
        throw std::runtime_error("yaml_serializer: cannot open file '" + path + "' for reading");
    }
    
    YAML::Node root = YAML::Load(in);
    in.close();

    requireNode(root, "root");
    requireNode(root, "users");
    requireNode(root, "groups");

    auto root_obj = deserializeFileObject(root["root"]);
    auto* dir = dynamic_cast<DirectorySnapshot*>(root_obj.get());

    if (!dir) {
        throw std::logic_error("root must be directory");
    }

    std::unique_ptr<DirectorySnapshot> root_dir(
        static_cast<DirectorySnapshot*>(root_obj.release())
    );

    std::vector<UserSnapshot> users;
    for (const auto& u : root["users"]) {
        users.push_back(deserializeUser(u));
    }

    std::vector<GroupSnapshot> groups;
    for (const auto& g : root["groups"]) {
        groups.push_back(deserializeGroup(g));
    }

    return FileSystemSnapshot(
        std::move(root_dir),
        std::move(users),
        std::move(groups)
    );
}
