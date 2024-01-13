#include "Core/Utils/CommandLineParser.hh"
#include "Core/Utils/Logging.hh"
#include <string_view>

using namespace std::string_view_literals;

namespace splc::utils {

std::string toupper(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::toupper(c); } // correct
    );
    return s;
}

void CommandLineParser::printHelp() const noexcept
{
    std::cout << "usage: " << progname << " ";
    std::cout << "[-h] [--h] ";

    for (auto &ent : regArgMap) {
        std::cout << "[--" << ent.first;
        if (ent.second & ArgOption::WithOption) {
            std::cout << " " << toupper(ent.first);
        }
        std::cout << "]";
    }
    for (auto &name : dirArgName) {
        std::cout << name << " ";
    }
    std::cout << std::endl;
}

void CommandLineParser::parseArgs(const int argc,
                                  const char *const argv[]) noexcept
{
    int i = 0;

    // Skip itself, assume user has passed the original argc/argv
    if (argc >= 1) {
        i = 1;
    }
    for (; i < argc; ++i) {
        std::string_view arg{argv[i]};
        std::string_view argName;
        std::string_view argOpt = ""sv;
        auto pos = arg.find('=');

        if (arg.starts_with("--")) {
            if (arg.substr(2) == "help"sv) {
                setHelpParsed();
                printHelp();
                continue;
            }
            if (pos != std::string_view::npos)
                argName = arg.substr(2, pos - 2);
            else
                argName = arg.substr(2);
        }
        else if (arg.starts_with("-")) {
            // single-dash argument

            // Process help first
            if (arg.substr(1) == "h"sv) {
                setHelpParsed();
                printHelp();
                continue;
            }

            // If not a directly assigned argument, we use the entire string
            // otherwise pos is supplied and name can be fetched

            if (pos != std::string_view::npos)
                argName = arg.substr(1, pos - 1);
            else
                argName = arg.substr(1, pos);
        }
        else {
            // a direct argument
            dirArg.push_back(std::string{arg});
            continue;
        }

        // check if this argument exists and requires a option
        auto it = regArgMap.find(argName);
        if (it == regArgMap.end()) {
            SPLC_LOG_ERROR(nullptr, false)
                << "unrecognized argument: " << arg.substr(0, pos);
            continue;
        }

        bool requireOpt = it->second == ArgOption::WithOption;

        // find the corresponding argOpt, if any
        if (pos != std::string_view::npos) {
            argOpt = arg.substr(pos + 1);

            if (!requireOpt) {
                SPLC_LOG_ERROR(nullptr, false)
                    << "argument " << arg.substr(0, pos)
                    << " does not require any option. Given: " << argOpt;
                continue;
            }
        }
        else if (requireOpt) {
            if (i >= argc) {
                SPLC_LOG_ERROR(nullptr, false)
                    << "argument " << argName
                    << " requires an option but none has been given";
                continue;
            }
            else
                argOpt = {argv[++i]};
        }

        argMap[std::string{argName}].push_back(std::string{argOpt});
    }
}

} // namespace splc::utils