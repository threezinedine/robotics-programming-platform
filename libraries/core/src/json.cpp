#include "core/json.h"
#include "nlohmann/json.hpp"
#include <string>

namespace rpp
{
    typedef nlohmann::json JSON;

    Json::Json()
        : m_data(RPP_NEW(JSON, JSON::parse("{}")))
    {
    }

    Json::Json(const String &jsonString)
    {
        m_data = RPP_NEW(JSON, JSON::parse(jsonString.CStr()));
    }

    Json::Json(const Json &other)
    {
        JSON *otherJson = static_cast<JSON *>(other.m_data);
        m_data = RPP_NEW(JSON, JSON::parse(otherJson->dump()));
    }

    Json::Json(Json &&other) noexcept
        : m_data(other.m_data)
    {
        // Leave the moved-from object in a valid state
        other.m_data = nullptr;
    }

    Json::~Json()
    {
        if (m_data != nullptr)
        {
            // delete static_cast<JSON *>(m_data);
            RPP_DELETE(m_data);
            m_data = nullptr;
        }
    }

    String Json::ToString() const
    {
        return static_cast<JSON *>(m_data)->dump(4).c_str();
    }

    b8 Json::Empty() const
    {
        return static_cast<JSON *>(m_data)->empty();
    }

    b8 Json::IsArray() const
    {
        return static_cast<JSON *>(m_data)->is_array();
    }

    u32 Json::Size() const
    {
        JSON *json = static_cast<JSON *>(m_data);
        if (!json->is_array())
        {
            return 0;
        }
        return static_cast<u32>(json->size());
    }

#define JSON_GET_SET_IMPLEMENT(type, isTypeMethod, getMethod, setMethod)   \
    template <>                                                            \
    type Json::Get<type>(const String &key, const type defaultValue) const \
    {                                                                      \
        JSON *json = static_cast<JSON *>(m_data);                          \
        if (!(*json).contains(key.CStr()))                                 \
        {                                                                  \
            return defaultValue;                                           \
        }                                                                  \
                                                                           \
        if (!(*json)[key.CStr()].isTypeMethod())                           \
        {                                                                  \
            return defaultValue;                                           \
        }                                                                  \
                                                                           \
        return (*json)[key.CStr()].getMethod;                              \
    }                                                                      \
    template <>                                                            \
    type Json::Get<type>(i32 index, const type defaultValue) const         \
    {                                                                      \
        JSON *json = static_cast<JSON *>(m_data);                          \
        if (!(*json).is_array() || index < 0 || index >= (*json).size())   \
        {                                                                  \
            return defaultValue;                                           \
        }                                                                  \
                                                                           \
        return (*json)[index].getMethod;                                   \
    }                                                                      \
                                                                           \
    template <>                                                            \
    void Json::Set<type>(const String &key, const type &value)             \
    {                                                                      \
        JSON *json = static_cast<JSON *>(m_data);                          \
        (*json)[key.CStr()] = setMethod;                                   \
    }                                                                      \
    template <>                                                            \
    void Json::Set<type>(i32 index, const type &value)                     \
    {                                                                      \
        JSON *json = static_cast<JSON *>(m_data);                          \
        if (!(*json).is_array() || index < 0 || index >= (*json).size())   \
        {                                                                  \
            return;                                                        \
        }                                                                  \
        (*json)[index] = setMethod;                                        \
    }                                                                      \
    template <>                                                            \
    void Json::Append<type>(const type &value)                             \
    {                                                                      \
        JSON *json = static_cast<JSON *>(m_data);                          \
        if (!(*json).is_array())                                           \
        {                                                                  \
            return;                                                        \
        }                                                                  \
        (*json).push_back(setMethod);                                      \
    }

    JSON_GET_SET_IMPLEMENT(String, is_string, get<std::string>().c_str(), value.CStr());

    JSON_GET_SET_IMPLEMENT(u32, is_number, get<u32>(), value);
    JSON_GET_SET_IMPLEMENT(u16, is_number, get<u16>(), value);
    JSON_GET_SET_IMPLEMENT(u8, is_number, get<u8>(), value);

    JSON_GET_SET_IMPLEMENT(i32, is_number, get<i32>(), value);
    JSON_GET_SET_IMPLEMENT(i16, is_number, get<i16>(), value);
    JSON_GET_SET_IMPLEMENT(i8, is_number, get<i8>(), value);

    JSON_GET_SET_IMPLEMENT(b8, is_boolean, get<bool>(), value);

    JSON_GET_SET_IMPLEMENT(f32, is_number, get<f32>(), value);
    JSON_GET_SET_IMPLEMENT(f64, is_number, get<f64>(), value);

    template <>
    Json Json::Get<Json>(const String &key, const Json defaultValue) const
    {
        JSON *json = static_cast<JSON *>(m_data);
        if (!(*json).contains(key.CStr()))
        {
            return defaultValue;
        }
        if (!(*json)[key.CStr()].is_object() && !(*json)[key.CStr()].is_array())
        {
            return defaultValue;
        }
        return Json((*json)[key.CStr()].get<JSON>().dump().c_str());
    }

    template <>
    void Json::Set<Json>(const String &key, const Json &value)
    {
        JSON *json = static_cast<JSON *>(m_data);
        (*json)[key.CStr()] = JSON::parse(value.ToString().CStr());
    }

    template <>
    void Json::Append<Json>(const Json &value)
    {
        JSON *json = static_cast<JSON *>(m_data);
        if (!(*json).is_array())
        {
            return;
        }
        (*json).push_back(JSON::parse(value.ToString().CStr()));
    }
} // namespace rpp
