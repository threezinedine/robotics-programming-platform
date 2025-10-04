#include "core/string.h"
#include <cstring>
#include <stdexcept>

namespace rpp
{
    String::String()
    {
        m_data = RPP_NEW(char[1]{'\0'});
    }

    String::String(const char *str)
    {
        if (str)
        {
            size_t len = std::strlen(str);
            m_data = RPP_NEW(char[len + 1]);
            std::strcpy(m_data, str);
        }
        else
        {
            m_data = RPP_NEW(char[1]{'\0'});
        }
    }

    String::String(const String &other)
    {
        size_t len = std::strlen(other.m_data);
        m_data = RPP_NEW(char[len + 1]);
        std::strcpy(m_data, other.m_data);
    }

    String::~String()
    {
        if (m_data != nullptr)
        {
            delete[] m_data;
            m_data = nullptr;
        }
    }

    u32 String::Length() const
    {
        return static_cast<u32>(std::strlen(m_data));
    }

    const char *String::CStr() const
    {
        return m_data;
    }

    char String::operator[](u32 index) const
    {
        if (index >= Length())
        {
            throw std::out_of_range("Index out of range");
        }
        return m_data[index];
    }

    void String::operator=(const String &other)
    {
        if (this != &other)
        {
            delete[] m_data;
            size_t len = std::strlen(other.m_data);
            m_data = RPP_NEW(char[len + 1]);
            std::strcpy(m_data, other.m_data);
        }
    }

    String String::operator+(const String &other)
    {
        return Concat(other);
    }

    void String::operator+=(const String &other)
    {
        String result = Concat(other);
        *this = result;
    }

    i32 String::Find(const String &substr, u32 startIndex) const
    {
        if (startIndex >= Length())
        {
            return -1;
        }

        const char *found = std::strstr(m_data + startIndex, substr.m_data);
        if (found)
        {
            return static_cast<i32>(found - m_data);
        }
        return -1;
    }

    String String::SubString(u32 startIndex, i32 length) const
    {
        if (startIndex >= Length())
        {
            return String();
        }

        u32 finalLength = 0;

        if (length == -1 || u32(length) + startIndex > Length())
        {
            finalLength = Length() - startIndex;
        }
        else
        {
            finalLength = static_cast<u32>(length);
        }

        char *subStr = RPP_NEW(char[finalLength + 1]);
        std::strncpy(subStr, m_data + startIndex, finalLength);
        subStr[finalLength] = '\0';

        String result(subStr);
        delete[] subStr;
        return result;
    }

    String String::Replace(const String &oldSubstr, const String &newSubstr, b8 replaceAll)
    {
        String result;
        i32 startIndex = 0;
        u32 oldLength = oldSubstr.Length();

        while (TRUE)
        {
            i32 foundIndex = Find(oldSubstr, startIndex);

            if (foundIndex == -1)
            {
                result += SubString(startIndex);
                break;
            }

            result += SubString(startIndex, foundIndex - startIndex);
            result += newSubstr;

            startIndex = foundIndex + oldSubstr.Length();

            if (!replaceAll)
            {
                result += SubString(startIndex);
                break; // Replace only the first occurrence
            }
        }

        return result;
    }

    String String::Concat(const String &other) const
    {
        size_t len1 = std::strlen(m_data);
        size_t len2 = std::strlen(other.m_data);
        char *newData = RPP_NEW(char[len1 + len2 + 1]);
        std::strcpy(newData, m_data);
        std::strcat(newData, other.m_data);
        newData[len1 + len2] = '\0';
        String result(newData);
        delete[] newData;
        return result;
    }

    void String::Split(Array<String> &outParts, const String &delimiter) const
    {
        u32 start = 0;
        u32 delimLength = delimiter.Length();

        while (start <= Length())
        {
            if (start == Length())
            {
                outParts.Push(String());
                break;
            }

            i32 delimIndex = Find(delimiter, start);
            if (delimIndex == -1)
            {
                outParts.Push(SubString(start));
                break;
            }
            outParts.Push(SubString(start, delimIndex - start));
            start = delimIndex + delimLength;
        }
    }

    b8 String::EndsWith(const String &suffix) const
    {
        // TODO: Next can be used with regex for more complex patterns
        u32 strLength = Length();
        u32 suffixLength = suffix.Length();

        if (suffixLength > strLength)
        {
            return FALSE;
        }

        return std::strcmp(m_data + strLength - suffixLength, suffix.m_data) == 0;
    }

    String String::ToLowerCase() const
    {
        size_t len = std::strlen(m_data);
        char *lowerData = RPP_NEW(char[len + 1]);
        for (size_t i = 0; i < len; i++)
        {
            lowerData[i] = static_cast<char>(std::tolower(m_data[i]));
        }
        lowerData[len] = '\0';
        String result(lowerData);
        delete[] lowerData;
        return result;
    }

    String String::Join(const Array<String> &parts, const String &delimiter)
    {
        if (parts.Size() == 0)
        {
            return String();
        }

        String result = parts[0];
        for (u32 i = 1; i < parts.Size(); i++)
        {
            result += delimiter;
            result += parts[i];
        }
        return result;
    }
} // namespace rpp
