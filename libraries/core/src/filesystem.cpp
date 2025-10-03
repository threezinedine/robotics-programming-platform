#include "core/filesystem.h"
#include <fstream>
#include "core/assertions.h"

namespace rpp
{
    Scope<Storage<FileSystem::FileEntry>> FileSystem::s_fileEntries = nullptr;

    void FileSystem::Initialize(b8 testingEnviroment)
    {
        RPP_UNUSED(testingEnviroment);
        RPP_ASSERT(s_fileEntries == nullptr);

        auto DeallocateFileEntry = [](FileEntry *pFileEntry)
        {
            RPP_ASSERT(pFileEntry != nullptr);

            if (pFileEntry->pFileHandle != nullptr)
            {
                RPP_DELETE(static_cast<std::ifstream *>(pFileEntry->pFileHandle));
            }

            RPP_DELETE(pFileEntry);
        };

        s_fileEntries = CreateScope<Storage<FileEntry>>(DeallocateFileEntry);
    }

    void FileSystem::Shutdown()
    {
        RPP_ASSERT(s_fileEntries != nullptr);

        s_fileEntries.reset();
    }

    FileHandle FileSystem::OpenFile(const String &filePath, FileMode mode)
    {
        RPP_ASSERT(s_fileEntries != nullptr);

        FileHandle fileHandle = s_fileEntries->Create();
        FileEntry *pFileEntry = s_fileEntries->Get(fileHandle);
        RPP_ASSERT(pFileEntry != nullptr);

        pFileEntry->id = fileHandle;
        pFileEntry->name = filePath;
        pFileEntry->mode = mode;

        std::ios_base::openmode openMode;

        switch (mode)
        {
        case FileMode::READ:
            openMode = std::ios::in;
            break;
        case FileMode::WRITE:
            openMode = std::ios::out | std::ios::trunc;
            break;
        case FileMode::APPEND:
            openMode = std::ios::out | std::ios::app;
            break;
        case FileMode::READ_WRITE:
            openMode = std::ios::in | std::ios::out;
            break;
        default:
            RPP_UNREACHABLE();
        }

        pFileEntry->pFileHandle = new std::ifstream(filePath.CStr(), openMode);

        if (!static_cast<std::ifstream *>(pFileEntry->pFileHandle)->is_open())
        {
            pFileEntry->pFileHandle = nullptr;
        }

        return fileHandle;
    }

    b8 FileSystem::IsFileOpen(FileHandle file)
    {
        RPP_ASSERT(s_fileEntries != nullptr);
        FileEntry *pFileEntry = s_fileEntries->Get(file);
        RPP_ASSERT(pFileEntry != nullptr);

        return pFileEntry->pFileHandle != nullptr;
    }

    String FileSystem::Read(FileHandle file)
    {
        RPP_ASSERT(s_fileEntries != nullptr);
        FileEntry *pFileEntry = s_fileEntries->Get(file);
        RPP_ASSERT(pFileEntry != nullptr);
        RPP_ASSERT(pFileEntry->pFileHandle != nullptr);
        RPP_ASSERT(pFileEntry->mode == FileMode::READ || pFileEntry->mode == FileMode::READ_WRITE);

        String content;
        std::ifstream *pFileStream = static_cast<std::ifstream *>(pFileEntry->pFileHandle);
        std::string line;
        while (std::getline(*pFileStream, line))
        {
            content += line.c_str();
            content += "\n";
        }

        return content;
    }

    void FileSystem::Write(FileHandle file, const String &data)
    {
        RPP_ASSERT(s_fileEntries != nullptr);
        FileEntry *pFileEntry = s_fileEntries->Get(file);
        RPP_ASSERT(pFileEntry != nullptr);

        RPP_UNUSED(data);
    }

    void FileSystem::CloseFile(FileHandle file)
    {
        RPP_ASSERT(s_fileEntries != nullptr);

        s_fileEntries->Free(file);
    }
} // namespace rpp
