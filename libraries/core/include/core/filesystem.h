#pragma once
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

    /**
     * All possible modes for opening a file.
     */
    enum class FileMode : u8
    {
        READ,      ///< Open the file for reading.
        WRITE,     ///< Open the file for writing (overwrites existing content).
        APPEND,    ///< Open the file for appending (adds to the end of the file).
        READ_WRITE ///< Open the file for both reading and writing.
    };

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
    class FileSystem
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
            FileMode mode;     ///< The mode in which the file was opened (if applicable).
        };

    public:
        /**
         * Initializes the file system module.
         * This function sets up necessary resources and configurations for file operations.
         * @param testingEnvironment If TRUE, initializes the module in a testing environment. Default is FALSE.
         */
        static void Initialize(b8 testingEnvironment = FALSE);

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
        static FileHandle OpenFile(const String &filePath, FileMode mode = FileMode::READ);

        /**
         * @brief Check if current file id is opened or not.
         * @param file The handle of the file to check.
         * @return True if the file is open, false otherwise.
         */
        static b8 IsFileOpen(FileHandle file);

        /**
         * @brief Reads the entire content of an open file into a string.
         * @param file The handle of the file to read from.
         * @return A String containing the content of the file.
         */
        static String Read(FileHandle file);

        /**
         * @brief Writes data to an open file.
         * @param file The handle of the file to write to.
         * @param data The data to write to the file. The data can be appended to the existing content of the file or overwrite it.
         * (default is to overwrite the existing content)
         */
        static void Write(FileHandle file, const String &data);

        /**
         * Closes an open file identified by the given file handle.
         * @param file The handle of the file to close.
         */
        static void CloseFile(FileHandle file);

    private:
        static Scope<Storage<FileEntry>> s_fileEntries; ///< Storage for file entries.
    };
} // namespace rpp
