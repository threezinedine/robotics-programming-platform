#include "core/string.h"
#include <cstring>
#include <stdexcept>

namespace rpp
{
    String::String()
    {
        m_data = new char[1]{'\0'};
    }

    String::String(const char *str)
    {
        if (str)
        {
            size_t len = std::strlen(str);
            m_data = new char[len + 1];
            std::strcpy(m_data, str);
        }
        else
        {
            m_data = new char[1]{'\0'};
        }
    }

    String::String(const String &other)
    {
        size_t len = std::strlen(other.m_data);
        m_data = new char[len + 1];
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
            m_data = new char[len + 1];
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

    String String::Concat(const String &other) const
    {
        size_t len1 = std::strlen(m_data);
        size_t len2 = std::strlen(other.m_data);
        char *newData = new char[len1 + len2 + 1];
        std::strcpy(newData, m_data);
        std::strcat(newData, other.m_data);
        newData[len1 + len2] = '\0';
        String result(newData);
        delete[] newData;
        return result;
    }
} // namespace rpp
