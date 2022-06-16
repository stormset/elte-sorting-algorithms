#ifndef ARGUMENT_PARSER_H
#define ARGUMENT_PARSER_H

#include <iostream>
#include <vector>
#include <cstring>

// Gyakorlás Obj. elvű proramozásra
// A parse ötlete innen származik: http://gsd.web.elte.hu/lectures/imper/imper-lecture-11/

/*
 * A simple class that parses the flags in the command line arguments (array of C-type strings).
 * Only a single char is supported like: -l or -laA == -l -a -A
 * Exception: --help
 */

class ParameterProcessor {
public:
    enum ParseException {
        UNSUPPORTED_FLAG, MISSING_DEPENDENCY
    };

    ParameterProcessor(int argc, char **argv) {
        this->argc = argc;
        this->argv = argv;
    }

    ParameterProcessor &addSupported(char flag, std::string description = "",
                                     bool isDependant = false, std::string dependencyDescription = "");

    bool isPresent(char flag);

    void parse();

    std::vector<char *>& getAdditionalParams();

    void setPermissive(bool isPermissive);

    void showHelpOnException(bool showHelpOnException);

private:
    struct Argument {
        char flag;
        bool present;
        bool isDependant; /* E.g. needs a file to be passed when present */
        std::string dependencyDescription;
        std::string description;
    };

    int argc;
    char **argv;
    bool permissive = false; /* won't throw exception */
    bool autoHelp = true; /* Shows usage when an error occurs */
    std::vector<Argument> flags; /* For holding parsed flags */
    std::vector<char *> non_flag; /* For holding pointers to non flag type parameters (like filename)*/

    void _parse();

    void showUsage(const std::string &msg = "");
};

#endif //ARGUMENT_PARSER_H
