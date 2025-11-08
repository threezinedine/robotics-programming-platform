#pragma once
#include "core/core.h"

#define STRUCTURE_SAVE_LOAD_DEFINE(className)                     \
public:                                                           \
    className();                                                  \
    className(const className##Description &desc);                \
    className(const className &other);                            \
    ~className();                                                 \
                                                                  \
public:                                                           \
    static className *Create();                                   \
    static className *Create(const className##Description &desc); \
    static className *Create(const String &filePath);             \
    void Save(const String &filePath) const;                      \
                                                                  \
public:                                                           \
    className##Description ToDescription() const;

/**
 * Need da define `className()`, `className(const className##Description &desc)`,
 * `className(const className &other)`, `~className()`, `ToDescription() const` manually.
 *
 */
#define STRUCTURE_SAVE_LOAD_IMPLEMENT(className)                                    \
    className *className::Create()                                                  \
    {                                                                               \
        return RPP_NEW(className);                                                  \
    }                                                                               \
                                                                                    \
    className *className::Create(const className##Description &desc)                \
    {                                                                               \
        return RPP_NEW(className, desc);                                            \
    }                                                                               \
                                                                                    \
    className *className::Create(const String &filePath)                            \
    {                                                                               \
        RPP_ASSERT(FileSystem::PathExists(filePath));                               \
                                                                                    \
        FileHandle file = FileSystem::OpenFile(filePath, FILE_MODE_READ);           \
        String fileContent = FileSystem::Read(file);                                \
        FileSystem::CloseFile(file);                                                \
                                                                                    \
        return RPP_NEW(className, FromString<className##Description>(fileContent)); \
    }                                                                               \
                                                                                    \
    void className::Save(const String &filePath) const                              \
    {                                                                               \
        FileHandle file = FileSystem::OpenFile(filePath, FILE_MODE_WRITE);          \
        FileSystem::Write(file, ToString(this->ToDescription()));                   \
        FileSystem::CloseFile(file);                                                \
    }
