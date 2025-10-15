#pragma once
#include "common.h"
#include "platforms/platforms.h"
#include "containers/storage.h"
#include "string.h"

namespace rpp
{
    /**
     * Used to represent a file handle in the file system module. The handle is an
     * abstract identifier for an open file, allowing for operations such as reading,
     * writing, and closing the file.
     */
    typedef u32 FileHandle;

#define FILE_MODE_READ u32(0x00)       ///< Open the file for reading.
#define FILE_MODE_WRITE u32(0x01)      ///< Open the file for writing (overwrites existing content).
#define FILE_MODE_APPEND u32(0x02)     ///< Open the file for appending (adds to the end of the file).
#define FILE_MODE_READ_WRITE u32(0x03) ///< Open the file for both reading and writing.

    /**
     * The file system module provides functionalities for file and directory operations.
     * It includes functions for reading and writing files, checking file existence,
     * and manipulating file paths. The side features of the module include:
     * - Cross-platform compatibility for file operations.
     * - Support for various file formats.
     * - Error handling and logging for file operations.
     * - Testing environment setup for file system operations.
     * @note This module is designed to be used in conjunction with other core modules.
     */
    class RPP_E2E_BINDING FileSystem
    {
    private:
        /**
         * Represents an entry in the file system, which could be a file or a directory.
         * This structure holds metadata about the entry, such as its name, path,
         * size, and type (file or directory).
         */
        struct FileEntry
        {
            FileHandle id;     ///< The unique identifier for the file or directory.
            String name;       ///< The name of the file or directory.
            void *pFileHandle; ///< Pointer to the underlying file handle (platform-specific).
            u8 mode;           ///< The mode in which the file was opened (if applicable).
        };

    private:
        /**
         * used internally to convert a given path to the actual physical path on the filesystem (for testing environment)
         *
         * @note in non-testing environment, this function will return the path as-is.
         */
        static String getPhysicalPath(const String &path);

        /**
         * @brief Checks if a physical file/folder exists on the filesystem.
         * @param path The physical path to check. The path is the ABSOLUTE path (physical not the logical path).
         * @return TRUE if the path exists, FALSE otherwise.
         */
        static b8 IsPhysicalPathExists(const String &path);

        /**
         * @brief Creates a physical directory on the filesystem.
         * @param path The physical path where the directory should be created (the ABSOLUTE path)
         * @note The function will create any necessary parent directories as well.
         * @note The directory is the physical path, not the logical path.
         */
        static void CreatePhysicalDirectory(const String &path);

    public:
        /**
         * Initializes the file system module.
         * This function sets up necessary resources and configurations for file operations.
         * @param temporaryPathRoot The root path for temporary files. Default is an empty string. If provided,
         * the file system will operate in a testing environment, using this path as the base for all file operations.
         * If left empty, the file system will operate in the normal environment, using the actual file system of the OS.
         * The path `must` be a relative path from the current working directory of the application (because use for testing only).
         * @note In testing environment, all file operations will be confined to the specified temporary path,
         */
        static void Initialize(const String &temporaryPathRoot = "");

        /**
         * Shuts down the file system module.
         * This function releases resources and performs cleanup operations.
         */
        static void Shutdown();

    public:
        /**
         * @brief Checks if a file exists at the specified path.
         * @param filePath The path to the file to check.
         * @param mode The mode in which to open the file. Default is READ. With READ mode,
         * the function checks if the file can be opened for reading. With WRITE or APPEND mode,
         * the function checks if the file can be opened for writing.
         * @return The id of the file if it exists, or INVALID_ID if it does not.
         */
        static FileHandle OpenFile(const String &filePath, u32 mode = FILE_MODE_READ) RPP_E2E_BINDING;

        /**
         * @brief Check if current file id is opened or not.
         * @param file The handle of the file to check.
         * @return True if the file is open, false otherwise.
         */
        static b8 IsFileOpen(FileHandle file) RPP_E2E_BINDING;

        /**
         * @brief Reads the entire content of an open file into a string.
         * @param file The handle of the file to read from.
         * @return A String containing the content of the file.
         */
        static String Read(FileHandle file) RPP_E2E_BINDING;

        /**
         * @brief Writes data to an open file.
         * @param file The handle of the file to write to.
         * @param data The data to write to the file. The data can be appended to the existing content of the file or overwrite it.
         * (default is to overwrite the existing content)
         */
        static void Write(FileHandle file, const String &data) RPP_E2E_BINDING;

        /**
         * Closes an open file identified by the given file handle.
         * @param file The handle of the file to close.
         */
        static void CloseFile(FileHandle file) RPP_E2E_BINDING;

        /**
         * @brief Checks if a file/folder exists at the specified path.
         * @param path The path to the file to check.
         * @return TRUE if the file exists, FALSE otherwise.
         */
        static b8 PathExists(const String &path);

        /**
         * @brief Check if a directory exists at the specified path.
         * @param path The path to the directory to check.
         * @return TRUE if the directory exists, FALSE otherwise.
         */
        static b8 IsDirectory(const String &path);

        /**
         * @brief Creates a directory at the specified path, including any necessary parent directories.
         * @param path The path where the directory should be created (the ABSOLUTE path).
         */
        static void CreateDirectory(const String &path);

        /// Path utils
    public:
        /**
         * Get the current working directory (CWD) of the application. In the `testingEnvironment`, the CWD is set to the `temp`
         * directory inside the project root and be deleted when the application exits.
         *
         * @note in `testingEnvironment`, all C:\ or D:\ inside the path will be keep as normal folder names, but the code inside the
         * module will treat them as the `temp` root folder.
         *
         * @example
         * ```cpp
         * FileSystem::Initialize(TRUE); // Initialize in testing environment
         *
         * String cwd = FileSystem::CWD(); // "C://WorkDir/test/" but the actual folder is "C://<path-to-project-temp>/c/WorkDir/test"
         * ```
         */
        static String CWD();

        /**
         * Splits a file path into its components (directories and file name).
         * @param outParts The array to store the resulting substrings.
         * @param path The file path to split.
         */
        static void SplitPath(Array<String> &outParts, const String &path);

    private:
        static String s_temporaryPathRoot; ///< Root path for temporary files in testing environment.
        static String s_cwd;               ///< Current working directory of the application, only being used in testing environment.
        static String s_convertedCWD;      ///< Converted CWD for testing environment (with drive letters replaced).

        static Scope<Storage<FileEntry>> s_fileEntries; ///< Storage for file entries.
    };
} // namespace rpp
