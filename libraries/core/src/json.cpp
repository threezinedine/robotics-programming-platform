#include "core/json.h"
#include "nlohmann/json.hpp"

namespace rpp
{
    typedef nlohmann::json JSON;

    Json::Json(const String &jsonString)
    {
        m_data = new JSON(JSON::parse(jsonString.CStr()));
    }

    Json::~Json()
    {
        delete static_cast<JSON *>(m_data);
    }

    String Json::ToString() const
    {
        return static_cast<JSON *>(m_data)->dump(4).c_str();
    }
} // namespace rpp
