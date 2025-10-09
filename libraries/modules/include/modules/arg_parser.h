#pragma once
#include "core/core.h"

namespace rpp
{
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
         * Main method of the ArgParser class. Parses the commandline arguments and returns
         * a Json object containing the parsed arguments.
         * @param argc The number of commandline arguments.
         * @param argv The array of commandline arguments.
         * @return A Json object containing the parsed arguments.
         *
         * @note all the configuration options like argument names, types, default values, etc must be
         * configured before calling this method.
         */
        Json Parse(int argc, char **argv);
    };
} // namespace rpp
