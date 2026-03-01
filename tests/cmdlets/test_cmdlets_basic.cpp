#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "../fs_for_test_cmdlets.h"
#include "requests/cmdlet/req_cmdlet.h"
#include "domain/objects/directory.h"
#include "domain/objects/directory_structure.h"
#include "domain/objects/reg_file.h"

#include "dto/objects/file_content_dto.h"

using namespace domain;
using namespace services;

TEST_CASE("touch - create file") {
    FsForTestCmdlets fs;
    fs.initialize();
    fs.registerBasicCmdlets();

    CmdletRequest req;
    req.args.push_back("test.txt");
    req.args.push_back("/");

    auto result = fs.getDispatcher()->executeCmdlet("touch", req);
    REQUIRE(result != nullptr);

    auto* file_obj = fs.getKernelService()->resolvePath("/test.txt");
    REQUIRE(file_obj != nullptr);
    REQUIRE(file_obj->getName() == "test.txt");

    auto* reg_file = dynamic_cast<IRegularFile*>(file_obj);
    REQUIRE(reg_file != nullptr);
}

TEST_CASE("touch - create multiple files") {
    FsForTestCmdlets fs;
    fs.initialize();
    fs.registerBasicCmdlets();

    CmdletRequest req1;
    req1.args.push_back("file1.txt");
    req1.args.push_back("/");
    fs.getDispatcher()->executeCmdlet("touch", req1);

    CmdletRequest req2;
    req2.args.push_back("file2.txt");
    req2.args.push_back("/");
    fs.getDispatcher()->executeCmdlet("touch", req2);

    auto* file1 = fs.getKernelService()->resolvePath("/file1.txt");
    auto* file2 = fs.getKernelService()->resolvePath("/file2.txt");

    REQUIRE(file1 != nullptr);
    REQUIRE(file2 != nullptr);
    REQUIRE(file1->getName() == "file1.txt");
    REQUIRE(file2->getName() == "file2.txt");
}

TEST_CASE("write - write to file") {
    FsForTestCmdlets fs;
    fs.initialize();
    fs.registerBasicCmdlets();

    CmdletRequest touch_req;
    touch_req.args.push_back("test.txt");
    touch_req.args.push_back("/");
    fs.getDispatcher()->executeCmdlet("touch", touch_req);

    CmdletRequest write_req;
    write_req.args.push_back("Hello, World!");
    write_req.args.push_back("/test.txt");

    auto result = fs.getDispatcher()->executeCmdlet("write", write_req);
    REQUIRE(result != nullptr);

    auto dto = fs.file_app->readFile("/test.txt");
    REQUIRE(dto.data == "Hello, World!");
}

TEST_CASE("write - append to file") {
    FsForTestCmdlets fs;
    fs.initialize();
    fs.registerBasicCmdlets();

    CmdletRequest touch_req;
    touch_req.args.push_back("test.txt");
    touch_req.args.push_back("/");
    fs.getDispatcher()->executeCmdlet("touch", touch_req);

    CmdletRequest write_req1;
    write_req1.args.push_back("Hello");
    write_req1.args.push_back("/test.txt");
    fs.getDispatcher()->executeCmdlet("write", write_req1);

    CmdletRequest write_req2;
    write_req2.flags.push_back("a");
    write_req2.args.push_back(", World!");
    write_req2.args.push_back("/test.txt");
    fs.getDispatcher()->executeCmdlet("write", write_req2);

    auto dto = fs.file_app->readFile("/test.txt");
    REQUIRE(dto.data == "Hello, World!");
}

TEST_CASE("cat - read file content") {
    FsForTestCmdlets fs;
    fs.initialize();
    fs.registerBasicCmdlets();

    CmdletRequest touch_req;
    touch_req.args.push_back("test.txt");
    touch_req.args.push_back("/");
    fs.getDispatcher()->executeCmdlet("touch", touch_req);

    CmdletRequest write_req;
    write_req.args.push_back("Test content");
    write_req.args.push_back("/test.txt");
    fs.getDispatcher()->executeCmdlet("write", write_req);

    CmdletRequest cat_req;
    cat_req.args.push_back("/test.txt");

    auto result = fs.getDispatcher()->executeCmdlet("cat", cat_req);
    REQUIRE(result != nullptr);
}

TEST_CASE("mkdir - create directory") {
    FsForTestCmdlets fs;
    fs.initialize();
    fs.registerBasicCmdlets();

    CmdletRequest req;
    req.args.push_back("testdir");
    req.args.push_back("/");

    auto result = fs.getDispatcher()->executeCmdlet("mkdir", req);
    REQUIRE(result != nullptr);

    auto* dir_obj = fs.getKernelService()->resolvePath("/testdir");
    REQUIRE(dir_obj != nullptr);
    REQUIRE(dir_obj->getName() == "testdir");

    auto* dir = dynamic_cast<IDirectory*>(dir_obj);
    REQUIRE(dir != nullptr);
}

TEST_CASE("mkdir - create nested directories") {
    FsForTestCmdlets fs;
    fs.initialize();
    fs.registerBasicCmdlets();

    CmdletRequest req1;
    req1.args.push_back("parent");
    req1.args.push_back("/");
    fs.getDispatcher()->executeCmdlet("mkdir", req1);

    CmdletRequest req2;
    req2.args.push_back("child");
    req2.args.push_back("/parent");
    fs.getDispatcher()->executeCmdlet("mkdir", req2);

    auto* parent = fs.getKernelService()->resolvePath("/parent");
    auto* child = fs.getKernelService()->resolvePath("/parent/child");

    REQUIRE(parent != nullptr);
    REQUIRE(child != nullptr);

    auto* parent_dir = dynamic_cast<IDirectory*>(parent);
    REQUIRE(parent_dir != nullptr);
    REQUIRE(parent_dir->getDirectoryStructure()->contains("child"));
}

TEST_CASE("ls - list directory contents") {
    FsForTestCmdlets fs;
    fs.initialize();
    fs.registerBasicCmdlets();

    CmdletRequest touch_req1;
    touch_req1.args.push_back("file1.txt");
    touch_req1.args.push_back("/");
    touch_req1.flags.push_back("a");
    REQUIRE(touch_req1.args.size() == 2);
    fs.getDispatcher()->executeCmdlet("touch", touch_req1);

    CmdletRequest touch_req2;
    touch_req2.args.push_back("file2.txt");
    touch_req2.args.push_back("/");
    fs.getDispatcher()->executeCmdlet("touch", touch_req2);

    CmdletRequest mkdir_req;
    mkdir_req.args.push_back("testdir");
    mkdir_req.args.push_back("/");
    fs.getDispatcher()->executeCmdlet("mkdir", mkdir_req);

    CmdletRequest ls_req;
    ls_req.args.push_back("/");

    auto result = fs.getDispatcher()->executeCmdlet("ls", ls_req);
    REQUIRE(result != nullptr);

    auto* root = dynamic_cast<IDirectory*>(fs.getKernelService()->resolvePath("/"));
    REQUIRE(root != nullptr);
    REQUIRE(root->getDirectoryStructure()->contains("file1.txt"));
    REQUIRE(root->getDirectoryStructure()->contains("file2.txt"));
    REQUIRE(root->getDirectoryStructure()->contains("testdir"));
}

TEST_CASE("ls - list specific directory") {
    FsForTestCmdlets fs;
    fs.initialize();
    fs.registerBasicCmdlets();

    CmdletRequest mkdir_req;
    mkdir_req.args.push_back("mydir");
    mkdir_req.args.push_back("/");
    fs.getDispatcher()->executeCmdlet("mkdir", mkdir_req);

    CmdletRequest touch_req;
    touch_req.args.push_back("file.txt");
    touch_req.args.push_back("/mydir");
    fs.getDispatcher()->executeCmdlet("touch", touch_req);

    CmdletRequest ls_req;
    ls_req.args.push_back("/mydir");

    auto result = fs.getDispatcher()->executeCmdlet("ls", ls_req);
    REQUIRE(result != nullptr);

    auto* dir = dynamic_cast<IDirectory*>(fs.getKernelService()->resolvePath("/mydir"));
    REQUIRE(dir != nullptr);
    REQUIRE(dir->getDirectoryStructure()->contains("file.txt"));
}

TEST_CASE("cp - copy directory n file") {
    FsForTestCmdlets fs;
    fs.initialize();
    fs.registerBasicCmdlets();

    CmdletRequest mkdir_req1;
    mkdir_req1.args.push_back("sourcedir");
    mkdir_req1.args.push_back("/");
    fs.getDispatcher()->executeCmdlet("mkdir", mkdir_req1);

    CmdletRequest mkdir_req2;
    mkdir_req2.args.push_back("destdir");
    mkdir_req2.args.push_back("/");
    fs.getDispatcher()->executeCmdlet("mkdir", mkdir_req2);

    CmdletRequest touch_req;
    touch_req.args.push_back("file.txt");
    touch_req.args.push_back("/");
    fs.getDispatcher()->executeCmdlet("touch", touch_req);

    CmdletRequest cp_req_dir;
    cp_req_dir.args.push_back("/sourcedir");
    cp_req_dir.args.push_back("/destdir");

    auto result = fs.getDispatcher()->executeCmdlet("cp", cp_req_dir);
    REQUIRE(result != nullptr);

    CmdletRequest cp_req_file;
    cp_req_file.args.push_back("file.txt");
    cp_req_file.args.push_back("/destdir");

    auto result_file = fs.getDispatcher()->executeCmdlet("cp", cp_req_file);
    REQUIRE(result_file != nullptr);

    auto* source = fs.getKernelService()->resolvePath("/sourcedir");
    auto* dest = fs.getKernelService()->resolvePath("/destdir");

    REQUIRE(source != nullptr);
    REQUIRE(dest != nullptr);

    auto* dest_dir = dynamic_cast<IDirectory*>(dest);
    REQUIRE(dest_dir != nullptr);
    REQUIRE(dest_dir->getDirectoryStructure()->contains("sourcedir"));
    REQUIRE(dest_dir->getDirectoryStructure()->contains("file.txt"));
}

TEST_CASE("mv - move directory n file") {
    FsForTestCmdlets fs;
    fs.initialize();
    fs.registerBasicCmdlets();

    CmdletRequest mkdir_req;
    mkdir_req.args.push_back("sourcedir");
    mkdir_req.args.push_back("/");
    fs.getDispatcher()->executeCmdlet("mkdir", mkdir_req);

    CmdletRequest mkdir_req2;
    mkdir_req2.args.push_back("destdir");
    mkdir_req2.args.push_back("/");
    fs.getDispatcher()->executeCmdlet("mkdir", mkdir_req2);

    CmdletRequest touch_req;
    touch_req.args.push_back("file.txt");
    touch_req.args.push_back("/sourcedir");
    fs.getDispatcher()->executeCmdlet("touch", touch_req);

    CmdletRequest mv_req;
    mv_req.args.push_back("/sourcedir");
    mv_req.args.push_back("/destdir");

    auto result = fs.getDispatcher()->executeCmdlet("mv", mv_req);
    REQUIRE(result != nullptr);

    auto* dest = fs.getKernelService()->resolvePath("/destdir");
    REQUIRE(dest != nullptr);

    REQUIRE_THROWS_AS(
        fs.getKernelService()->resolvePath("/sourcedir"),
        std::runtime_error
    );

    auto* file = fs.getKernelService()->resolvePath("/destdir/sourcedir/file.txt");
    REQUIRE(file != nullptr);
}

TEST_CASE("rm - remove file") {
    FsForTestCmdlets fs;
    fs.initialize();
    fs.registerBasicCmdlets();

    CmdletRequest touch_req;
    touch_req.args.push_back("test.txt");
    touch_req.args.push_back("/");
    fs.getDispatcher()->executeCmdlet("touch", touch_req);

    CmdletRequest rm_req;
    rm_req.args.push_back("/test.txt");

    auto result = fs.getDispatcher()->executeCmdlet("rm", rm_req);
    REQUIRE(result != nullptr);

    REQUIRE_THROWS_AS(
        fs.getKernelService()->resolvePath("/test.txt"),
        std::runtime_error
    );
}

TEST_CASE("rm - remove directory") {
    FsForTestCmdlets fs;
    fs.initialize();
    fs.registerBasicCmdlets();

    CmdletRequest mkdir_req;
    mkdir_req.args.push_back("testdir");
    mkdir_req.args.push_back("/");
    fs.getDispatcher()->executeCmdlet("mkdir", mkdir_req);

    CmdletRequest rm_req;
    rm_req.args.push_back("/testdir");

    auto result = fs.getDispatcher()->executeCmdlet("rm", rm_req);
    REQUIRE(result != nullptr);

    REQUIRE_THROWS_AS(
        fs.getKernelService()->resolvePath("/testdir"),
        std::runtime_error
    );
}

TEST_CASE("stat - get file statistics") {
    FsForTestCmdlets fs;
    fs.initialize();
    fs.registerBasicCmdlets();

    CmdletRequest touch_req;
    touch_req.args.push_back("test.txt");
    touch_req.args.push_back("/");
    fs.getDispatcher()->executeCmdlet("touch", touch_req);

    CmdletRequest write_req;
    write_req.args.push_back("Test data");
    write_req.args.push_back("/test.txt");
    fs.getDispatcher()->executeCmdlet("write", write_req);

    CmdletRequest stat_req;
    stat_req.args.push_back("/test.txt");

    auto result = fs.getDispatcher()->executeCmdlet("stat", stat_req);
    REQUIRE(result != nullptr);
}

TEST_CASE("stat - get directory statistics") {
    FsForTestCmdlets fs;
    fs.initialize();
    fs.registerBasicCmdlets();

    CmdletRequest mkdir_req;
    mkdir_req.args.push_back("testdir");
    mkdir_req.args.push_back("/");
    fs.getDispatcher()->executeCmdlet("mkdir", mkdir_req);

    CmdletRequest stat_req;
    stat_req.args.push_back("/testdir");

    auto result = fs.getDispatcher()->executeCmdlet("stat", stat_req);
    REQUIRE(result != nullptr);
}

TEST_CASE("Integration - create, write, read, delete") {
    FsForTestCmdlets fs;
    fs.initialize();
    fs.registerBasicCmdlets();

    CmdletRequest touch_req;
    touch_req.args.push_back("integration.txt");
    touch_req.args.push_back("/");
    fs.getDispatcher()->executeCmdlet("touch", touch_req);

    CmdletRequest write_req;
    write_req.args.push_back("Integration test");
    write_req.args.push_back("/integration.txt");
    fs.getDispatcher()->executeCmdlet("write", write_req);

    CmdletRequest cat_req;
    cat_req.args.push_back("/integration.txt");
    fs.getDispatcher()->executeCmdlet("cat", cat_req);

    auto dto = fs.file_app->readFile("/integration.txt");
    REQUIRE(dto.data == "Integration test");

    CmdletRequest rm_req;
    rm_req.args.push_back("/integration.txt");
    fs.getDispatcher()->executeCmdlet("rm", rm_req);

    REQUIRE_THROWS_AS(
        fs.getKernelService()->resolvePath("/integration.txt"),
        std::runtime_error
    );
}