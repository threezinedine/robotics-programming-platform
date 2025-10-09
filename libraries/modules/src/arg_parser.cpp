#include "modules/arg_parser.h"

namespace rpp
{
    ArgParser::ArgParser()
        : m_argumentDefinitions()
    {
    }

    ArgParser::~ArgParser()
    {
    }

    void ArgParser::AddArgument(const ArgumentData &argData)
    {
        m_argumentDefinitions.Push(argData);
    }

    Json ArgParser::Parse(int argc, char **argv)
    {
        RPP_ASSERT_MSG(argc > 0 && argv != nullptr, "Invalid command line arguments.");

        Json result;

        if (m_argumentDefinitions.Size() == 0)
        {
            return result;
        }

        ArgumentData &firstArgDef = m_argumentDefinitions[0];
        if (argc != 2)
        {
            throw std::runtime_error("Invalid number of arguments. Expected exactly one argument.");
        }

        result.Set(firstArgDef.name, String(argv[1]));

        return result;
    }
} // namespace rpp
