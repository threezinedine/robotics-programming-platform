#pragma once
#include "block.h"
#include "modules/modules.h"

namespace rpp
{
    /**
     * The super basic block, each function will be placed in a file as a function block.
     */
    class FunctionBlock : public Block
    {
    public:
        FunctionBlock(const FunctionDescription &desc);
        ~FunctionBlock();

    public:
        /**
         * Needed if we wanna serialize and save the function block for later reload.
         */
        FunctionDescription ToDescription();

    protected:
        virtual void DrawImpl() override;
    };
} // namespace rpp
