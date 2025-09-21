#pragma once
#include "platforms/platforms.h"
#include <map>

namespace rpp
{
    // TODO: Implement a custom map class if needed
    template <typename K, typename V>
    using Map = std::map<K, V>;
} // namespace rpp
