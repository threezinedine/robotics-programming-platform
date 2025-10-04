#include "core/filesystem.h"
#include <fstream>
#include "core/assertions.h"
#include <direct.h>

/**
 * @note not using the FileSystem interface because it can be messed up with the testing environment
 */

namespace rpp
{
    Scope<Storage<FileSystem::FileEntry>> FileSystem::s_fileEntries = nullptr;
    String FileSystem::s_temporaryPathRoot = "";

    void FileSystem::Initialize(const String &temporaryPathRoot)
    {
        if (temporaryPathRoot.Length() > 0) /// testing environment
        {
            char buffer[1024];
            RPP_ASSERT(getcwd(buffer, sizeof(buffer)) != nullptr);
            String cwd(buffer);
            s_temporaryPathRoot = cwd + "/" + temporaryPathRoot;

            // create the temporary directory if it does not exist
            _mkdir(s_temporaryPathRoot.CStr());
        }

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

        // clean up the temporary directory if in testing environment
        if (s_temporaryPathRoot.Length() > 0)
        {
            // Note: This is a simple implementation and may not handle all cases (e.g., nested directories).
            // For a robust solution, consider using a library like Boost.Filesystem or std::filesystem (C++17).
            _rmdir(s_temporaryPathRoot.CStr());
            s_temporaryPathRoot = "";
        }
    }

    String FileSystem::getPhysicalPath(const String &path)
    {
        if (s_temporaryPathRoot.Length() == 0)
        {
            return path;
        }

        // In testing environment, prepend the temporary path root
#if RPP_PLATFORM_WINDOWS
        // TODO: Need a better way to handle drive letters in paths, maybe use Regex?

        String physicalPath = s_temporaryPathRoot + "/" + path;
        physicalPath.Replace("C:/", "c/");
        physicalPath.Replace("D:/", "d/");
        physicalPath.Replace("E:/", "e/");
        physicalPath.Replace("F:/", "f/");
        physicalPath.Replace("G:/", "g/");
        physicalPath.Replace("H:/", "h/");
        physicalPath.Replace("I:/", "i/");
        physicalPath.Replace("\\", "/", TRUE);
        return physicalPath;
#else
#error "FileSystem is only implemented for Windows platform."
#endif
    }

    b8 FileSystem::IsPhysicalPathExists(const String &path)
    {
        std::ifstream file(path.CStr());
        return file.good();
    }

    void FileSystem::CreatePhysicalDirectory(const String &path, b8 isRecursive)
    {
        Array<String> parts;
        SplitPath(parts, path);
    }

    FileHandle FileSystem::OpenFile(const String &filePath, FileMode mode)
    {
        RPP_ASSERT(s_fileEntries != nullptr);

        FileHandle fileHandle = s_fileEntries->Create();
        FileEntry *pFileEntry = s_fileEntries->Get(fileHandle);
        RPP_ASSERT(pFileEntry != nullptr);

        pFileEntry->id = fileHandle;
        pFileEntry->name = getPhysicalPath(filePath);
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

    String FileSystem::CWD()
    {
        char buffer[1024];
        if (getcwd(buffer, sizeof(buffer)) != nullptr)
        {
            return String(buffer);
        }
        return String();
    }

    void FileSystem::SplitPath(Array<String> &outParts, const String &path)
    {
        outParts.Clear();

        String finalPath = path;
        finalPath = finalPath.Replace("\\", "/", TRUE); // replace all backslashes with forward slashes

        finalPath.Split(outParts, "/");
        u32 partsCount = outParts.Size();
        u32 index = 0;

        while (index < outParts.Size())
        {
            if (outParts[index].Length() == 0)
            {
                outParts.Erase(index);
            }
            else
            {
                if (outParts[index].EndsWith(":"))
                {
                    outParts[index] = outParts[index].ToLowerCase();
                    outParts[index] = outParts[index].SubString(0, outParts[index].Length() - 1);
                }
                index++;
            }
        }
    }
} // namespace rpp
