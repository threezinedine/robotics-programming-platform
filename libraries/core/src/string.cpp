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
        delete[] m_data;
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
} // namespace rpp
