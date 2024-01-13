#ifndef __SPLC_CORE_UTILS_COMMANDLINEPARSER_HH__
#define __SPLC_CORE_UTILS_COMMANDLINEPARSER_HH__ 1

#include <map>
#include <memory>
#include <optional>
#include <vector>

namespace splc::utils {

class CommandLineParser {
  public:
    enum ArgOption : unsigned {
        NoOption = 0,
        WithOption = 1,
    };

  public:
    CommandLineParser(std::string_view progname_) noexcept : progname{progname_}
    {
    }

    void reset() noexcept
    {
        regArgMap.clear();
        argMap.clear();
        dirArg.clear();
    }

    void printHelp() const noexcept;

    void addPositionalArg(std::string_view name, unsigned opt) noexcept
    {
        regArgMap[std::string{name}] = opt;
    }

    void addSeqDirArgName(std::string_view name) noexcept
    {
        dirArgName.push_back(std::string{name});
    }

    void parseArgs(const int argc, const char *const argv[]) noexcept;

    std::optional<std::vector<std::string_view>>
    operator[](std::string_view key) const noexcept;

    std::string_view operator[](size_t idx) const noexcept
    {
        return dirArg[idx];
    }

    bool isArgPresent(std::string_view key) const noexcept
    {
        return argMap.find(key) != argMap.end();
    }

    template <class T = std::string_view>
        requires std::is_constructible_v<T, const std::string &>
    std::optional<std::vector<T>> get(std::string_view key) const noexcept;

    template <class T, class Functor>
        requires std::is_invocable_r_v<T, Functor, const std::string &>
    std::optional<std::vector<T>> get(std::string_view key,
                                      Functor &&f) const noexcept;

    template <class T = std::string_view>
        requires std::is_constructible_v<T, const std::string &>
    T get(size_t idx) const noexcept
    {
        return T{dirArg[idx]};
    }

    template <class T, class Functor>
        requires std::is_invocable_r_v<T, Functor, const std::string &>
    T get(size_t idx, Functor &&f) const noexcept;

    auto &getProgramName() const noexcept { return progname; }

    bool isHelpParsed() const noexcept { return parsedHelp; }

    auto &getRegisteredArgList() const noexcept { return regArgMap; }

    auto &getArgMap() const noexcept { return argMap; }

    size_t getDirectArgSize() const noexcept { return dirArg.size(); }

    auto &getDirectArgVec() const noexcept { return dirArg; }

  private:
    void setHelpParsed() noexcept { parsedHelp = true; }

  private:
    std::string progname;    ///< Program name
    bool parsedHelp = false; ///< True if help has been parsed in argv
    std::map<std::string, unsigned, std::less<>>
        regArgMap; ///< Map of registered arguments
    std::map<std::string, std::vector<std::string>, std::less<>> argMap;
    std::vector<std::string> dirArg;     ///< List of direct arguments parsed
    std::vector<std::string> dirArgName; ///< list of name of direct arguments

}; // class CommandLineParser

inline std::optional<std::vector<std::string_view>>
CommandLineParser::operator[](std::string_view key) const noexcept
{
    auto it = argMap.find(key);
    if (it != argMap.end()) {
        std::vector<std::string_view> ret;
        ret.reserve(it->second.size());
        std::transform(it->second.begin(), it->second.end(),
                       std::back_inserter(ret),
                       [](const auto &p) { return std::string_view{p}; });
        return ret;
    }
    else
        return std::nullopt;
}

template <class T>
    requires std::is_constructible_v<T, const std::string &>
inline std::optional<std::vector<T>>
CommandLineParser::get(std::string_view key) const noexcept
{
    auto it = argMap.find(key);
    if (it != argMap.end()) {
        std::vector<T> ret;
        ret.reserve(it->second.size());
        std::transform(it->second.begin(), it->second.end(),
                       std::back_inserter(ret),
                       [](const auto &p) { return T{p}; });
        return ret;
    }
    else
        return std::nullopt;
}

template <class T, class Functor>
    requires std::is_invocable_r_v<T, Functor, const std::string &>
std::optional<std::vector<T>> CommandLineParser::get(std::string_view key,
                                                     Functor &&f) const noexcept
{
    auto it = argMap.find(key);
    if (it != argMap.end()) {
        std::vector<T> ret;
        ret.reserve(it->second.size());
        std::transform(it->second.begin(), it->second.end(),
                       std::back_inserter(ret),
                       [=](const auto &p) { return f(p); });
        return ret;
    }
    else
        return std::nullopt;
}

template <class T, class Functor>
    requires std::is_invocable_r_v<T, Functor, const std::string &>
T CommandLineParser::get(size_t idx, Functor &&f) const noexcept
{
    return std::move(f(dirArg[idx]));
}

} // namespace splc::utils

#endif // __SPLC_CORE_UTILS_COMMANDLINEPARSER_HH__
