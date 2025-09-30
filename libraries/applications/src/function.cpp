#include "applications/structures/function.h"

namespace rpp
{
    Function::Function()
        : m_name("UnnamedFunction")
    {
    }

    Function::Function(const FunctionDescription &desc)
        : m_name(desc.name)
    {
    }

    Function::Function(const Function &other)
        : m_name(other.m_name)
    {
    }

    Function::~Function()
    {
    }

    Function Function::CreateFunction(const FunctionDescription &desc)
    {
        return Function(desc);
    }
} // namespace rpp
