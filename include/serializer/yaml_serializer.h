#pragma once

#include <string>
#include <memory>

#include <yaml-cpp/yaml.h>

#include "iserializer.h"
#include "domain/types.h"

#include "snapshots/acl/acl_snapshot.h"
#include "snapshots/acl/acl_item_snapshot.h"
#include "snapshots/acl/acl_subject_snapshot.h"

class FileObjectSnapshot;
class FileMetadataSnapshot;
class UserSnapshot;
class GroupSnapshot;
class FileSystemSnapshot;

/**
 * @class YamlFileSystemSerializer
 * 
 * @brief сериализатор файловой системы в формат yaml
 * 
 * выполняет сериализацию и десериализацию снимка файловой системы в формат yaml
 */
class YamlFileSystemSerializer final
    : public ISerializer<FileSystemSnapshot>
{
public:
    /**
     * @brief записать снимок файловой системы в файл
     * 
     * @param path путь к файлу
     * @param snapshot снимок файловой системы для записи
     */
    void write(
        const std::string& path,
        const FileSystemSnapshot& snapshot
    ) const override;

    /**
     * @brief прочитать снимок файловой системы из файла
     * 
     * @param path путь к файлу
     * @return FileSystemSnapshot прочитанный снимок файловой системы
     */
    FileSystemSnapshot read(
        const std::string& path
    ) const override;

private:
    /**
     * @brief сериализовать пользователя в yaml узел
     * 
     * @param user снимок пользователя
     * @return YAML::Node yaml узел с данными пользователя
     */
    YAML::Node serializeUser(
        const UserSnapshot& user
    ) const;

    /**
     * @brief десериализовать пользователя из yaml узла
     * 
     * @param node yaml узел с данными пользователя
     * @return UserSnapshot снимок пользователя
     */
    UserSnapshot deserializeUser(
        const YAML::Node& node
    ) const;

    /**
     * @brief сериализовать группу в yaml узел
     * 
     * @param group снимок группы
     * @return YAML::Node yaml узел с данными группы
     */
    YAML::Node serializeGroup(
        const GroupSnapshot& group
    ) const;

    /**
     * @brief десериализовать группу из yaml узла
     * 
     * @param node yaml узел с данными группы
     * @return GroupSnapshot снимок группы
     */
    GroupSnapshot deserializeGroup(
        const YAML::Node& node
    ) const;

    /**
     * @brief сериализовать список контроля доступа в yaml узел
     * 
     * @param acl снимок списка контроля доступа
     * @return YAML::Node yaml узел с данными списка контроля доступа
     */
    YAML::Node serializeAcl(
        const AclSnapshot& acl
    ) const;

    /**
     * @brief десериализовать список контроля доступа из yaml узла
     * 
     * @param node yaml узел с данными списка контроля доступа
     * @return std::unique_ptr<AclSnapshot> снимок списка контроля доступа
     */
    std::unique_ptr<AclSnapshot> deserializeAcl(
        const YAML::Node& node
    ) const;

    /**
     * @brief сериализовать элемент списка контроля доступа в yaml узел
     * 
     * @param item снимок элемента списка контроля доступа
     * @return YAML::Node yaml узел с данными элемента
     */
    YAML::Node serializeAclItem(
        const AclItemSnapshot& item
    ) const;

    /**
     * @brief десериализовать элемент списка контроля доступа из yaml узла
     * 
     * @param node yaml узел с данными элемента
     * @return AclItemSnapshot снимок элемента списка контроля доступа
     */
    AclItemSnapshot deserializeAclItem(
        const YAML::Node& node
    ) const;

    /**
     * @brief сериализовать метаданные файла в yaml узел
     * 
     * @param meta снимок метаданных файла
     * @return YAML::Node yaml узел с метаданными файла
     */
    YAML::Node serializeMetadata(
        const FileMetadataSnapshot& meta
    ) const;

    /**
     * @brief десериализовать метаданные файла из yaml узла
     * 
     * @param node yaml узел с метаданными файла
     * @return FileMetadataSnapshot снимок метаданных файла
     */
    FileMetadataSnapshot deserializeMetadata(
        const YAML::Node& node
    ) const;

    /**
     * @brief сериализовать объект файловой системы в yaml узел
     * 
     * @param obj снимок объекта файловой системы
     * @return YAML::Node yaml узел с данными объекта
     */
    YAML::Node serializeFileObject(
        const FileObjectSnapshot& obj
    ) const;

    /**
     * @brief десериализовать объект файловой системы из yaml узла
     * 
     * @param node yaml узел с данными объекта
     * @return std::unique_ptr<FileObjectSnapshot> снимок объекта файловой системы
     */
    std::unique_ptr<FileObjectSnapshot> deserializeFileObject(
        const YAML::Node& node
    ) const;

    /**
     * @brief преобразовать тип права доступа в строку
     * 
     * @param type тип права доступа
     * @return std::string строковое представление типа права
     */
    std::string rightTypeToString(
        domain::RightType type
    ) const;

    /**
     * @brief преобразовать строку в тип права доступа
     * 
     * @param value строковое представление типа права
     * @return domain::RightType тип права доступа
     */
    domain::RightType stringToRightType(
        const std::string& value
    ) const;

    /**
     * @brief преобразовать тип доступа в строку
     * 
     * @param type тип доступа
     * @return std::string строковое представление типа доступа
     */
    std::string accessTypeToString(
        domain::AccessType type
    ) const;

    /**
     * @brief преобразовать строку в тип доступа
     * 
     * @param value строковое представление типа доступа
     * @return domain::AccessType тип доступа
     */
    domain::AccessType stringToAccessType(
        const std::string& value
    ) const;

    /**
     * @brief преобразовать тип субъекта списка контроля доступа в строку
     * 
     * @param kind тип субъекта
     * @return std::string строковое представление типа субъекта
     */
    std::string subjectKindToString(
        AclSubjectKind kind
    ) const;

    /**
     * @brief преобразовать строку в тип субъекта списка контроля доступа
     * 
     * @param value строковое представление типа субъекта
     * @return AclSubjectKind тип субъекта
     */
    AclSubjectKind stringToSubjectKind(
        const std::string& value
    ) const;


    /**
     * @brief проверить наличие поля в yaml узле
     * 
     * @param node yaml узел для проверки
     * @param field имя поля
     */
    void requireNode(
        const YAML::Node& node,
        const std::string& field
    ) const;

};