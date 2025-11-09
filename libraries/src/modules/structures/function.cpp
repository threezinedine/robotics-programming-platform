#include "modules/structures/function.h"

namespace rpp
{
    Function::Function()
        : m_name("UnnamedFunction")
    {
        RPP_PROFILE_SCOPE();
    }

    Function::Function(const FunctionDescription &desc)
        : m_name(desc.name)
    {
        RPP_PROFILE_SCOPE();
    }

    Function::Function(const Function &other)
        : m_name(other.m_name)
    {
        RPP_PROFILE_SCOPE();
    }

    Function::~Function()
    {
        RPP_PROFILE_SCOPE();
    }

    Function Function::CreateFunction(const FunctionDescription &desc)
    {
        RPP_PROFILE_SCOPE();
        return Function(desc);
    }
} // namespace rpp
