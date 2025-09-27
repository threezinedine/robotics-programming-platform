#pragma once
#include "core/core.h"

namespace rpp
{
    class Drawable
    {
    public:
        virtual ~Drawable() = default;

    public:
        virtual void Draw() const = 0;
    };
};