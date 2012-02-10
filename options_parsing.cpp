// options_parsing.cpp
#include <iostream>

#include "options_parsing.h"
#include "globals.h"

const char* Options::HELP     = "help";
const char* Options::TASKFILE = "taskfile";

Options::Options(int argc, char* argv[])
    : m_argc(argc)
    , m_argv(argv)
    , m_optionsMap(new OptionsMap)
{
}

Options::~Options()
{
    delete m_optionsMap;
}

OptionsMap* Options::getOptions()
{
    return m_optionsMap;
}

int Options::parse()
{
    namespace po = boost::program_options;
    using std::string;

    string description = string(PROGRAM_NAME) + " " + string(VERSION)
                                + " ver, allowed options"
    ;

    po::options_description desc(description);

    desc.add_options()
        (HELP, "produce help message")
        (TASKFILE, po::value<std::string>(),
         "set file to read & write tracked tasks")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(m_argc, m_argv, desc), vm);
    po::notify(vm);

    if (vm.count(HELP)) {
        std::cout << desc << "\n";
        return 1;
    }

    if (vm.count(TASKFILE)) {
        OptionValue file = vm[TASKFILE].as< string >();
        (*m_optionsMap)[TASKFILE] = file;
    } else {
        OptionValue defaultFile = "";
        (*m_optionsMap)[TASKFILE] = defaultFile;
    }

    return 0;
}

