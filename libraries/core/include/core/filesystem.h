#pragma once
#include "platforms/platforms.h"
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
         * @return The id of the file if it exists, or INVALID_ID if it does not.
         */
        static FileHandle OpenFile(const String &filePath);

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
         * Closes an open file identified by the given file handle.
         * @param file The handle of the file to close.
         */
        static void CloseFile(FileHandle file);
    };
} // namespace rpp
