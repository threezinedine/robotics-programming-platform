#pragma once
#include "core/core.h"

namespace rpp
{
    /**
     * Used for defining the properties of the command line arguments. The properties
     * can be used to configure the ArgParser instance.
     *
     * @note add default value for some non-frequently used properties.
     */
    struct ArgumentData
    {
        String name; ///< Name of the argument (e.g., "--help" or "-h").
    };

    /**
     * General tool for extracting the commandline arguments which are passed to the executable into
     * a more manageable format. The interface of this class is mailnly inspired by
     * the argparse module in Python.
     */
    class ArgParser
    {
    public:
        ArgParser();
        ~ArgParser();

    public:
        /**
         * Adds a new argument to the parser with the specified properties.
         * @param argData The properties of the argument to be added.
         * @note This method should be called before calling the Parse method.
         */
        void AddArgument(const ArgumentData &argData);

        /**
         * Main method of the ArgParser class. Parses the commandline arguments and returns
         * a Json object containing the parsed arguments.
         * @param argc The number of commandline arguments.
         * @param argv The array of commandline arguments.
         * @return A Json object containing the parsed arguments.
         *
         * @note all the configuration options like argument names, types, default values, etc must be
         * configured before calling this method.
         */
        Json Parse(int argc, const char **argv);

    private:
        Array<ArgumentData> m_argumentDefinitions; ///< Stores the definitions of the arguments to be parsed.
    };
} // namespace rpp
