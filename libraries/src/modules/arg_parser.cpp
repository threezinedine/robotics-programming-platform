#include "modules/arg_parser.h"

namespace rpp
{
    ArgParser::ArgParser()
        : m_argumentDefinitions()
    {
        RPP_PROFILE_SCOPE();
    }

    ArgParser::~ArgParser()
    {
        RPP_PROFILE_SCOPE();
    }

    void ArgParser::AddArgument(const ArgumentData &argData)
    {
        RPP_PROFILE_SCOPE();
        m_argumentDefinitions.Push(argData);
    }

    Json ArgParser::Parse(int argc, const char **argv)
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT_MSG(argc > 0 && argv != nullptr, "Invalid command line arguments.");

        Json result;

        if (m_argumentDefinitions.Size() == 0)
        {
            return result;
        }

        if (m_argumentDefinitions.Size() == 1)
        {
            ArgumentData &firstArgDef = m_argumentDefinitions[0];
            if (argc != 2)
            {
                throw std::runtime_error("Invalid number of arguments. Expected exactly one argument.");
            }

            result.Set(firstArgDef.name, String(argv[1]));
            return result;
        }

        RPP_ASSERT(m_argumentDefinitions.Size() == 2);

        if (argc != 3)
        {
            throw std::runtime_error("Invalid number of arguments. Expected exactly two arguments.");
        }
        result.Set(m_argumentDefinitions[0].name, String(argv[1]));
        result.Set(m_argumentDefinitions[1].name, String(argv[2]));

        return result;
    }
} // namespace rpp
