#include "core/filesystem.h"
#include <fstream>
#include <filesystem>
#include "core/assertions.h"
#include <direct.h>

/**
 * @note not using the FileSystem interface because it can be messed up with the testing environment
 *
 * TODO: Later, add mechanism for skipping the testing environment for RELEASE build.
 */

namespace rpp
{
    Scope<Storage<FileSystem::FileEntry>> FileSystem::s_fileEntries = nullptr;
    String FileSystem::s_temporaryPathRoot = "";
    String FileSystem::s_cwd = "";
    String FileSystem::s_convertedCWD = "";

    void FileSystem::Initialize(const String &temporaryPathRoot)
    {
        if (temporaryPathRoot.Length() > 0) /// testing environment
        {
            char buffer[1024];
            RPP_ASSERT(getcwd(buffer, sizeof(buffer)) != nullptr);
            String cwd(buffer);
            s_temporaryPathRoot = temporaryPathRoot;
            s_cwd = cwd;

            // TODO: Need a better way to handle drive letters in paths, maybe use Regex? And avoid hardcoding for all drive letters and duplicated code.
            s_convertedCWD = s_cwd;
            s_convertedCWD = s_convertedCWD.Replace("\\", "/", TRUE);
            s_convertedCWD = s_convertedCWD.Replace("C:/", "c/");
            s_convertedCWD = s_convertedCWD.Replace("D:/", "d/");
            s_convertedCWD = s_convertedCWD.Replace("E:/", "e/");
            s_convertedCWD = s_convertedCWD.Replace("F:/", "f/");
            s_convertedCWD = s_convertedCWD.Replace("G:/", "g/");
            s_convertedCWD = s_convertedCWD.Replace("H:/", "h/");
            s_convertedCWD = s_convertedCWD.Replace("I:/", "i/");

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
            std::filesystem::remove_all(s_temporaryPathRoot.CStr());
            //_rmdir(s_temporaryPathRoot.CStr());
            s_temporaryPathRoot = "";
        }

        s_convertedCWD.~String();
        s_cwd.~String();
        s_temporaryPathRoot.~String();
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

        String physicalPath = path;
        physicalPath = physicalPath.Replace("\\", "/", TRUE);
        physicalPath = physicalPath.Replace("C:/", "c/");
        physicalPath = physicalPath.Replace("D:/", "d/");
        physicalPath = physicalPath.Replace("E:/", "e/");
        physicalPath = physicalPath.Replace("F:/", "f/");
        physicalPath = physicalPath.Replace("G:/", "g/");
        physicalPath = physicalPath.Replace("H:/", "h/");
        physicalPath = physicalPath.Replace("I:/", "i/");

        if (physicalPath.SubString(0, s_convertedCWD.Length()) != s_convertedCWD)
        {
            physicalPath = s_temporaryPathRoot + "/" + s_convertedCWD + "/" + physicalPath;
        }
        else
        {
            physicalPath = s_temporaryPathRoot + "/" + physicalPath;
        }
        return physicalPath;
#else
#error "FileSystem is only implemented for Windows platform."
#endif
    }

    b8 FileSystem::IsPhysicalPathExists(const String &path)
    {
        return std::filesystem::exists(path.CStr()) && std::filesystem::is_directory(path.CStr());
    }

    void FileSystem::CreatePhysicalDirectory(const String &path)
    {
        Array<String> parts;
        SplitPath(parts, path);
        u32 partsCount = parts.Size();

        String currentFolder;

        for (u32 partIndex = 0; partIndex < partsCount; ++partIndex)
        {
            currentFolder = partIndex == 0 ? parts[partIndex] : currentFolder + "/" + parts[partIndex];

            if (IsPhysicalPathExists(currentFolder))
            {
                continue;
            }

            _mkdir(currentFolder.CStr());
        }
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

        String physicalPath = getPhysicalPath(filePath);

        if (mode == FileMode::WRITE || mode == FileMode::APPEND || mode == FileMode::READ_WRITE)
        {
            // ensure the directory exists

            // TODO: Another interface for creating directory?
            Array<String> pathParts;
            SplitPath(pathParts, physicalPath);

            String directoryPath = String::Join(pathParts, "/").SubString(0, physicalPath.Length() - pathParts[pathParts.Size() - 1].Length() - 1);
            CreatePhysicalDirectory(directoryPath);
        }

        std::ios_base::openmode openMode;

        switch (mode)
        {
        case FileMode::READ:
        {
            pFileEntry->pFileHandle = new std::ifstream(getPhysicalPath(filePath).CStr(), std::ios::in);
            openMode = std::ios::in;
            if (!static_cast<std::ifstream *>(pFileEntry->pFileHandle)->is_open())
            {
                pFileEntry->pFileHandle = nullptr;
            }

            break;
        }
        case FileMode::WRITE:
        {
            openMode = std::ios::out | std::ios::trunc;
            pFileEntry->pFileHandle = new std::ofstream(getPhysicalPath(filePath).CStr(), openMode);
            if (!static_cast<std::ofstream *>(pFileEntry->pFileHandle)->is_open())
            {
                pFileEntry->pFileHandle = nullptr;
            }
            break;
        }
        case FileMode::APPEND:
        {
            openMode = std::ios::out | std::ios::app;
            pFileEntry->pFileHandle = new std::ofstream(getPhysicalPath(filePath).CStr(), openMode);
            if (!static_cast<std::ofstream *>(pFileEntry->pFileHandle)->is_open())
            {
                pFileEntry->pFileHandle = nullptr;
            }
            break;
        }
        case FileMode::READ_WRITE:
        {
            openMode = std::ios::in | std::ios::out;
            pFileEntry->pFileHandle = new std::fstream(getPhysicalPath(filePath).CStr(), openMode);
            if (!static_cast<std::fstream *>(pFileEntry->pFileHandle)->is_open())
            {
                pFileEntry->pFileHandle = nullptr;
            }
            break;
        }
        default:
            RPP_UNREACHABLE();
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
            if (content.Length() > 0)
            {
                content += "\n";
            }
            content += line.c_str();
        }

        return content;
    }

    void FileSystem::Write(FileHandle file, const String &data)
    {
        RPP_ASSERT(s_fileEntries != nullptr);
        FileEntry *pFileEntry = s_fileEntries->Get(file);
        RPP_ASSERT(pFileEntry != nullptr);
        RPP_ASSERT(pFileEntry->pFileHandle != nullptr);
        RPP_ASSERT(pFileEntry->mode == FileMode::WRITE || pFileEntry->mode == FileMode::APPEND || pFileEntry->mode == FileMode::READ_WRITE);

        switch (pFileEntry->mode)
        {
        case FileMode::WRITE:
        case FileMode::APPEND:
        {
            std::ofstream *pFileStream = static_cast<std::ofstream *>(pFileEntry->pFileHandle);
            (*pFileStream) << data.CStr();
            pFileStream->flush();
            break;
        }
        default:
            RPP_UNREACHABLE();
        };

        RPP_UNUSED(data);
    }

    void FileSystem::CloseFile(FileHandle file)
    {
        RPP_ASSERT(s_fileEntries != nullptr);

        s_fileEntries->Free(file);
    }

    String FileSystem::CWD()
    {
        return s_cwd;
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
