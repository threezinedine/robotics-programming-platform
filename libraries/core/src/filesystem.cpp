#include "core/filesystem.h"

namespace rpp
{
    void FileSystem::Initialize(b8 testingEnviroment)
    {
        RPP_UNUSED(testingEnviroment);
        RPP_ASSERT(s_fileEntries == nullptr);

        s_fileEntries = CreateScope<Storage<FileEntry>>();
    }

    void FileSystem::Shutdown()
    {
        RPP_ASSERT(s_fileEntries != nullptr);

        s_fileEntries.reset();
    }

    FileHandle FileSystem::OpenFile(const String &filePath)
    {
        RPP_ASSERT(s_fileEntries != nullptr);

        FileHandle fileHandle = s_fileEntries->Create();
        FileEntry *fileEntry = s_fileEntries->Get(fileHandle);
        RPP_ASSERT(fileEntry != nullptr);

        fileEntry->id = fileHandle;
        fileEntry->name = filePath;

        return fileHandle;
    }

    b8 FileSystem::IsFileOpen(FileHandle file)
    {
        RPP_ASSERT(s_fileEntries != nullptr);
        FileEntry *fileEntry = s_fileEntries->Get(file);
        RPP_ASSERT(fileEntry != nullptr);

        return fileEntry != nullptr;
    }

    String FileSystem::Read(FileHandle file)
    {
        return "";
    }

    void FileSystem::CloseFile(FileHandle file)
    {
        RPP_ASSERT(s_fileEntries != nullptr);
        s_fileEntries->Free(file);
    }
} // namespace rpp
