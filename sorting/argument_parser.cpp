#include "argument_parser.h"

ParameterProcessor &ParameterProcessor::addSupported(char flag, std::string description,
                                                     bool isDependant, std::string dependencyDescription) {
    flags.push_back({flag, false, isDependant, std::move(dependencyDescription), std::move(description)});
    return *this;
}

bool ParameterProcessor::isPresent(char flag) {
    for (const Argument& arg : flags){
        if(arg.flag == flag)
            return arg.present;
    }
    return false;
}

void ParameterProcessor::parse() { _parse(); }

void ParameterProcessor::setPermissive(bool isPermissive) {
    permissive = isPermissive;
}

void ParameterProcessor::showHelpOnException(bool showHelpOnException) {
    autoHelp = showHelpOnException;
}

void ParameterProcessor::_parse() {
    size_t dependantCount = 0; bool showHelp = false;
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if(strcmp(argv[i], "--help") == 0){ showUsage(); exit(0); break;}
            int j = 1;
            while ('\0' != argv[i][j]) {
                /* Linear search */
                size_t k = 0;
                while (k < flags.size() && flags[k].flag != argv[i][j]) k++;
                if(k < flags.size()){
                    flags[k].present = true;
                    dependantCount += flags[k].isDependant;
                }else {
                    if(autoHelp) showHelp = true;
                    if(!permissive){
                        showUsage("Invalid option!");
                        throw UNSUPPORTED_FLAG;
                    }
                }
                ++j;
            }
        } else {
            non_flag.push_back(argv[i]);
        }
    }

    if (non_flag.size() < dependantCount){
        if(autoHelp && !showHelp) {showUsage("Missing [OTHER] argument.");}
        if(!permissive) throw MISSING_DEPENDENCY;
    }

    if(showHelp) showUsage("Invalid option!");
}

std::vector<char *> &ParameterProcessor::getAdditionalParams() {
    return non_flag;
}

void ParameterProcessor::showUsage(const std::string &msg) {
    std::cout<<msg<<"\n";
    std::cout<<"Usage: "<<argv[0]<<" [OPTION]... [OTHER]...";
    std::printf("\n%10s\t%10s", "[OPTION]", "[OTHER]");
    for (const Argument& arg : flags){
        std::printf("\n    -%1c\t%20s\t\t%10s",
                    arg.flag,
                    arg.dependencyDescription.c_str(),
                    arg.description.c_str());
    }
    std::cout<<"\n";
}
