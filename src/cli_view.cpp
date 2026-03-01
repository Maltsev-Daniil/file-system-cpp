#include "../include/cli_view.h"

#include <exception>
#include <iostream>

#include "../include/io.h"
#include "../include/processor.h"

#include "../include/dto/fs/disk_usage_dto.h"

#include "dto/objects/dir_content_dto.h"
#include "dto/objects/file_content_dto.h"
#include "dto/objects/stat_file_dto.h"

#include "dto/rights/e_rights_dto.h"
#include "dto/rights/file_acl_dto.h"
//
// Created by nitsir on 1/2/26.
//

namespace view {

std::string CliView::formatTime12h(const domain::TimePoint& tp)
{
    std::time_t tt =
        std::chrono::system_clock::to_time_t(tp);

    std::tm* tm = std::localtime(&tt);

    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %I:%M:%S %p");
    return oss.str();
}

std::string CliView::formatRightToStr(const domain::RightType& right) {
    std::string rights_str;
    switch (right) {
    case domain::RightType::READ:
        rights_str.push_back('r');
        break;
    case domain::RightType::WRITE:
        rights_str.push_back('w');
        break;
    case domain::RightType::EXECUTE:
        rights_str.push_back('x');
        break;
    default:
        break;
    }
    return rights_str;
}

void CliView::run() {
    std::string line;

    while (true) {
        output_.write("> ");

        if (!std::getline(std::cin, line)) {
            break;
        }

        if (line == "exit") {
            break;
        }

        if (line.empty()) {
            continue;
        }

        try {
            cmdProcessor_.process(line, *this);
        } catch (const std::exception& e) {
            showMessage(e.what());
        }
    }
}

void CliView::showMessage(const std::string& text) {
    output_.writeln(text);
}

void CliView::showDirectoryContent(const DirContentDTO& dto) {
    for (const auto& name : dto.list) {
        output_.writeln(name);
    }
}

void CliView::showFileContent(const FileContentDTO& dto) {
    output_.write(dto.data);

    if (!dto.data.empty() && dto.data.back() != '\n') {
        output_.writeln("");
    }
}

void CliView::showFileStat(const StatFileDTO& dto) {
    output_.writeln("File statistics:");
    output_.writeln(" inode: " + std::to_string(dto.id));
    output_.writeln(" size: " + std::to_string(dto.size) + " bytes");
    output_.writeln(" uid: " + std::to_string(dto.uid));
    output_.writeln(" gid: " + std::to_string(dto.gid));
    output_.writeln(" created: " + formatTime12h(dto.creation_time));
    output_.writeln(" modified: " + formatTime12h(dto.modification_time));
}

void CliView::showDiskUsage(const DiskUsageDTO& dto) {
    output_.writeln("Disk usage:");
    output_.writeln(" files: " + std::to_string(dto.total_files));
    output_.writeln(" directories: " + std::to_string(dto.total_dirs));
    output_.writeln(" total size: " + std::to_string(dto.total_size) + " bytes");
}

void CliView::showEffectiveRights(const FileERightsDTO& dto) {
    output_.write("effective rights: ");

    bool first = true;
    for (const auto& right : dto.e_rights) {
        if (!first) {
            output_.write(", ");
        }
        output_.write(formatRightToStr(right));
        first = false;
    }

    output_.writeln("");
}

void CliView::showFileAcl(const FileAclDTO& dto) {
    output_.writeln("ACL for: " + dto.file_path);
    output_.writeln(" owner: " + dto.owner);
    output_.writeln(" group: " + dto.group);
    output_.writeln(" entries:");

    for (const auto& entry : dto.entries) {
        output_.write("  ");
        output_.write(entry.type);
        output_.write(" ");
        output_.write(entry.name);
        output_.write(" : ");
        output_.writeln(entry.perms);
    }
}

}