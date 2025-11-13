#include "applications/language/blocks/function_block.h"

namespace rpp
{
    FunctionBlock::FunctionBlock(const FunctionDescription &desc)
        : Block()
    {
        RPP_PROFILE_SCOPE();
        RPP_UNUSED(desc);
    }

    FunctionBlock::~FunctionBlock()
    {
        RPP_PROFILE_SCOPE();
    }

    void FunctionBlock::DrawImpl()
    {
        RPP_PROFILE_SCOPE();
    }

    FunctionDescription FunctionBlock::ToDescription()
    {
        RPP_PROFILE_SCOPE();
        FunctionDescription desc;
        return desc;
    }
} // namespace rpp
