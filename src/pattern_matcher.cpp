#include "pattern_matcher.hpp"

namespace tcheck
{

PatternMatcher::PatternMatcher(std::string_view regexp)
    // TODO RYS compile time regexp validation?
    : expression_{regexp.begin(), regexp.end()}
{}

bool PatternMatcher::IsValid(string const & password) const
{
    std::smatch m;
    return std::regex_match(password, m, expression_);
}

vector<string> PatternMatcher::Match(string const &)
{
    return {};
}

}
