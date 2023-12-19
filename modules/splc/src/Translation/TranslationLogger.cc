#include <iomanip>
#include <string>

#include "Core/Utils/LocationWrapper.hh"
#include "Translation/TranslationBase.hh"
#include "Translation/TranslationLogger.hh"

namespace splc {

void printIndicator(std::ostream &os, utils::logging::Level level,
                    const Location &loc)
{
    using namespace utils::logging;
    using ControlSeq = utils::logging::ControlSeq;

    const std::string &filename = *loc.begin.contextName;
    std::string line;

    // Print a newline to separate from the previous string
    os << "\n";

    // TODO(future): support multiple lines
    if (std::ifstream in{filename}; in.good()) {
        size_t lineCnt = 0;
        while (in) {
            std::getline(in, line);
            lineCnt++;
            if (lineCnt == loc.begin.line) {
                break;
            }
        }
        if (line.empty()) {
            SPLC_LOG_ERROR(nullptr)
                << "cannot retrieve line from location: " << ControlSeq::Bold
                << loc << ControlSeq::Reset;
        }
        else {
            // Print actual error
            size_t start = loc.begin.column, end = 1;
            if (loc.end.line > loc.begin.line) {
                end = 1 + line.length();
            }
            else {
                end = loc.end.column;
            }

            // Sidebar: line counter
            os << std::setw(8) << std::setfill(' ') << std::right
               << loc.begin.line << " | ";

            // Actual content
            os << line.substr(0, start - 1);
            os << ControlSeq::Bold << getLevelColor(level);
            os << line.substr(start - 1, end - start);
            os << ControlSeq::Reset;
            os << line.substr(end - 1);
            os << "\n";

            // Indicator
            os << "         | " << ControlSeq::Bold << getLevelColor(level);

            // Space
            for (size_t i = 1; i <= start - 1; ++i) {
                os << " ";
            }

            // Actual indicator
            os << "^";
            for (size_t i = start + 1; i < end; ++i) {
                os << "~";
            }
            os << ControlSeq::Reset; 
        }
    }
    else {
        os << ControlSeq::Bold << ControlSeq::Red
           << "cannot retrieve file: " << ControlSeq::Reset << ControlSeq::Bold
           << *loc.begin.contextName << ControlSeq::Reset;
    }
    // leave the remaining newline to `~Logger()`.
}

TranslationLogger::~TranslationLogger()
{
    if (locPtr) {
        Ptr<TranslationContext> context =
            tunit.translationContextManager
                .allContexts[locPtr->begin.contextID];
        if (locPtr->begin.contextName &&
            context->type == TranslationContextBufferType::File) {
            printIndicator(localLogStream, level, *locPtr);
        }
        // TODO: support macro expansion
    }
}

void TranslationLogger::printInitial()
{
    if (trace && locPtr != nullptr) {
        const auto cid = locPtr->begin.contextID;
        const TranslationContext *context =
            tunit.translationContextManager.contextStack[cid].get();
        printContextStack(context, 0);
    }
    Logger::printInitial();
}

void TranslationLogger::printContextStack(const TranslationContext *context,
                                          size_t depth) const
{
    using ControlSeq = utils::logging::ControlSeq;
    if (context->parent) {
        if (depth == 0) {
            localLogStream << "In file included from ";
        }
        else {
            localLogStream << "                      ";
        }
        if (context->type == TranslationContextBufferType::File) {
            localLogStream << ControlSeq::Bold << context->intrLocation;
        }

        // If there is still parent left
        if (context->parent->parent) {
            localLogStream << ",\n";
            printContextStack(context->parent, depth + 1);
        }
        else {
            localLogStream << ":\n";
        }
    }
}

} // namespace splc