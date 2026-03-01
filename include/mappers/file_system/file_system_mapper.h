#pragma once

#include <memory>
#include <vector>

#include "mappers/entity/group_mapper.h"
#include "mappers/entity/user_mapper.h"
#include "mappers/objects/directory_mapper.h"
#include "snapshots/file_system/file_system_snapshot.h"

namespace domain {
class IKernelFactory;
class IKernelContainer;
class IKernelService;
class IDirectory;
class IUser;
class IGroup;
}

/**
 * @class IFileSystemMapper
 *
 * @brief интерфейс для преобразования файловой системы в снапшот и обратно
 *
 * обеспечивает полное преобразование состояния файловой системы, включая
 * корневую директорию, пользователей и группы, в сериализуемый снапшот
 * и восстановление состояния ядра из снапшота
 */
class IFileSystemMapper {
public:
    /**
     * @brief деструктор интерфейса маппера файловой системы
     */
    virtual ~IFileSystemMapper() = default;

    /**
     * @brief преобразовать состояние файловой системы в снапшот
     *
     * сериализует все компоненты файловой системы: корневую директорию,
     * список пользователей и список групп
     *
     * @param root_dir ссылка на корневую директорию файловой системы
     * @param users список пользователей файловой системы
     * @param groups список групп файловой системы
     * @return FileSystemSnapshot полный снапшот файловой системы
     */
    virtual FileSystemSnapshot
    toSnapshot(const domain::IDirectory& root_dir,
        const std::vector<const domain::IUser*>& users,
        const std::vector<const domain::IGroup*>& groups) const = 0;

    /**
     * @brief восстановить контейнер ядра из снапшота файловой системы
     *
     * восстанавливает полное состояние ядра, включая пользователей,
     * группы и корневую директорию файловой системы
     *
     * @param snapshot ссылка на снапшот файловой системы
     * @param kernel_factory фабрика для создания объектов ядра
     * @return std::shared_ptr<domain::IKernelContainer> указатель на восстановленный контейнер ядра
     */
    virtual std::shared_ptr<domain::IKernelContainer>
    kernelFromSnapshot(
        const FileSystemSnapshot& snapshot,
        domain::IKernelFactory& kernel_factory
    ) const = 0;

};

/**
 * @class FileSystemMapper
 *
 * @brief реализация маппера для файловой системы
 */
class FileSystemMapper final : public IFileSystemMapper {
public:
    /**
     * @brief конструктор маппера файловой системы
     *
     * @param user_mapper маппер для преобразования пользователей
     * @param group_mapper маппер для преобразования групп
     * @param directory_mapper маппер для преобразования директорий
     */
    FileSystemMapper(
        const IUserMapper& user_mapper,
        const IGroupMapper& group_mapper,
        const IDirectoryMapper& directory_mapper
    );

    FileSystemSnapshot toSnapshot(const domain::IDirectory& root_dir,
        const std::vector<const domain::IUser*>& users,
        const std::vector<const domain::IGroup*>& groups) const override;

    std::shared_ptr<domain::IKernelContainer>
        kernelFromSnapshot(
            const FileSystemSnapshot& snapshot,
            domain::IKernelFactory& kernel_factory) const override;

private:
    const IUserMapper& user_mapper_;
    const IGroupMapper& group_mapper_;
    const IDirectoryMapper& directory_mapper_;

    /**
     * @brief восстановить список пользователей из снапшота
     *
     * @param snapshot ссылка на снапшот файловой системы
     * @return std::vector<std::unique_ptr<domain::IUser>> вектор восстановленных пользователей
     */
    std::vector<std::unique_ptr<domain::IUser>>
        usersFromSnapshot(const FileSystemSnapshot& snapshot) const;

    /**
     * @brief восстановить список групп из снапшота
     *
     * @param snapshot ссылка на снапшот файловой системы
     * @return std::vector<std::unique_ptr<domain::IGroup>> вектор восстановленных групп
     */
    std::vector<std::unique_ptr<domain::IGroup>>
        groupsFromSnapshot(const FileSystemSnapshot& snapshot) const;

    /**
     * @brief восстановить корневую директорию из снапшота
     *
     * @param snapshot ссылка на снапшот файловой системы
     * @return std::unique_ptr<domain::IDirectory> указатель на восстановленную корневую директорию
     */
    std::unique_ptr<domain::IDirectory>
        rootFromSnapshot(const FileSystemSnapshot& snapshot) const;
};