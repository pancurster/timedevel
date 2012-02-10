#ifndef OPTIONS_PARSING_H
#define OPTIONS_PARSING_H

#include <map>
#include <string>

#include <boost/variant.hpp>
#include <boost/program_options.hpp>

typedef boost::variant <std::string, int> OptionValue;
typedef std::map <std::string, OptionValue> OptionsMap;

class Options {
    public:
        Options(int argc, char* argv[]);
        ~Options();

        /// Start parsing options
        ///@ret 
        /// * 1 need quit
        /// * 0 no need to quit
        int parse();

        /// Return map with options
        OptionsMap* getOptions();

        // OPTIONS
        static const char* HELP;
        static const char* TASKFILE;

    private:

        int m_argc;
        char** m_argv;
        OptionsMap* m_optionsMap;
};

#endif // OPTIONS_PARSING_H
