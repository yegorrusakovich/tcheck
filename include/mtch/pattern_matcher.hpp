#ifndef TCHECK_PASSWORD_SEARCH_HPP_
#define TCHECK_PASSWORD_SEARCH_HPP_


#include <regex>

namespace tcheck
{
// TODO RYS move to config
template<typename T>
using vector = std::vector<T>;

using string = std::string;

class PatternMatcher
{
public:
    explicit PatternMatcher(std::string_view regexp);
    virtual ~PatternMatcher() = default;

    virtual bool IsValid(string const & text) const;
    // TODO RYS add search (for searching in text)
    virtual vector<string> Match(string const & text) const;
    virtual std::string GetUserErrorDescription() const;
private:
    std::regex expression_;
};

}
#endif
