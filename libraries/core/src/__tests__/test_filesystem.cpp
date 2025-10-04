#include "test_common.h"

/**
 * Due to the testing purpose, the interface of the FileSystem can only be tested in the testing enviroment.
 */

class FileSystemTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        rpp::FileSystem::Initialize("temp");
    }

    void TearDown() override
    {
        rpp::FileSystem::Shutdown(); ///< must have to clean up the "temp" folder for next test, can disable if you want to inspect the files
    }
};

TEST_F(FileSystemTest, CreateAndReadAFile)
{
    String cwd = rpp::FileSystem::CWD();
    String filePath = cwd + "/test_file.txt";

    FileHandle file = FileSystem::OpenFile(filePath, FileMode::WRITE);
    ASSERT_NE(file, INVALID_ID);
    ASSERT_TRUE(FileSystem::IsFileOpen(file));

    String data = "Hello, World!";
    FileSystem::Write(file, data);
    FileSystem::CloseFile(file);

    file = FileSystem::OpenFile(filePath, FileMode::READ);
    ASSERT_NE(file, INVALID_ID);
    String content = FileSystem::Read(file);
    FileSystem::CloseFile(file);

    ASSERT_STREQ(content.CStr(), data.CStr());
}

TEST_F(FileSystemTest, SplitPathWithoutHardDisk)
{
    Array<String> parts;
    rpp::FileSystem::SplitPath(parts, "folder/subfolder/file.txt");

    ASSERT_EQ(parts.Size(), 3);
    ASSERT_STREQ(parts[0].CStr(), "folder");
    ASSERT_STREQ(parts[1].CStr(), "subfolder");
    ASSERT_STREQ(parts[2].CStr(), "file.txt");
}

TEST_F(FileSystemTest, SplitPathWithHardDisk)
{
    Array<String> parts;
    rpp::FileSystem::SplitPath(parts, "C:/folder/subfolder/file.txt");

    ASSERT_EQ(parts.Size(), 4);
    ASSERT_STREQ(parts[0].CStr(), "c");
    ASSERT_STREQ(parts[1].CStr(), "folder");
    ASSERT_STREQ(parts[2].CStr(), "subfolder");
    ASSERT_STREQ(parts[3].CStr(), "file.txt");
}

TEST_F(FileSystemTest, PathWithReverseSlash)
{
    Array<String> parts;
    rpp::FileSystem::SplitPath(parts, "C:\\folder\\subfolder\\file.txt");

    ASSERT_EQ(parts.Size(), 4);
    ASSERT_STREQ(parts[0].CStr(), "c");
    ASSERT_STREQ(parts[1].CStr(), "folder");
    ASSERT_STREQ(parts[2].CStr(), "subfolder");
    ASSERT_STREQ(parts[3].CStr(), "file.txt");
}

TEST_F(FileSystemTest, SplitPathWithMissingSlash)
{
    Array<String> parts;
    rpp::FileSystem::SplitPath(parts, "C:\\folder\\subfolder/file.txt");

    ASSERT_EQ(parts.Size(), 4);
    ASSERT_STREQ(parts[0].CStr(), "c");
    ASSERT_STREQ(parts[1].CStr(), "folder");
    ASSERT_STREQ(parts[2].CStr(), "subfolder");
    ASSERT_STREQ(parts[3].CStr(), "file.txt");
}

TEST_F(FileSystemTest, SplitPathWithLeadingAndTrailingSlash)
{
    Array<String> parts;
    rpp::FileSystem::SplitPath(parts, "/folder/subfolder/file/");

    ASSERT_EQ(parts.Size(), 3);
    ASSERT_STREQ(parts[0].CStr(), "folder");
    ASSERT_STREQ(parts[1].CStr(), "subfolder");
    ASSERT_STREQ(parts[2].CStr(), "file");
}

TEST_F(FileSystemTest, SpaceFolderName)
{
    Array<String> parts;
    rpp::FileSystem::SplitPath(parts, "C:/My Documents/Work Folder/file name.txt");

    ASSERT_EQ(parts.Size(), 4);
    ASSERT_STREQ(parts[0].CStr(), "c");
    ASSERT_STREQ(parts[1].CStr(), "My Documents");
    ASSERT_STREQ(parts[2].CStr(), "Work Folder");
    ASSERT_STREQ(parts[3].CStr(), "file name.txt");
}