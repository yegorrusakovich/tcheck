#include <mtch/pattern_matcher.hpp>
#include <algorithm>
#include <vector>

namespace tcheck
{

PatternMatcher::PatternMatcher(std::string_view regexp)
    // TODO RYS compile time regexp validation?
    : expression_{regexp.begin(), regexp.end()}
{}

bool PatternMatcher::IsValid(string const & text) const
{
    std::smatch m;
    return std::regex_match(text, m, expression_);
}

vector<string> PatternMatcher::Match(string const & text) const
{
    std::smatch m;
    if (std::regex_match(text, m, expression_) && !m.empty())
    {
        std::vector<string> res;
        res.reserve(m.size());
        std::ranges::transform(m, std::back_inserter(res), &std::ssub_match::str);
        return res;
    }
    return {};
}

std::string PatternMatcher::GetUserErrorDescription() const
{
    return {};
}

}
