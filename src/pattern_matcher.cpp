#include <algorithm>
#include <mtch/pattern_matcher.hpp>
#include <regex>
#include <vector>

namespace tcheck {

PatternMatcher::PatternMatcher(std::string_view regexp)
    // TODO RYS compile time regexp validation?
    : expression_{regexp.begin(), regexp.end()} {}

bool PatternMatcher::IsValid(std::string_view text) const {
  std::cmatch m;
  return std::regex_match(text.cbegin(), text.cend(), m, expression_);
}

std::vector<std::string_view> PatternMatcher::Match(
    std::string_view text) const {
  std::cmatch m;
  if (std::regex_match(text.cbegin(), text.cend(), m, expression_) &&
      !m.empty()) {
    std::vector<std::string_view> res;
    res.reserve(m.size());
    std::ranges::transform(m, std::back_inserter(res), &std::csub_match::str);
    return res;
  }
  return {};
}

std::vector<std::string_view> PatternMatcher::Search(
    std::string_view text) const {
  return Match(text);
}

std::string PatternMatcher::GetUserErrorDescription() const { return {}; }

}  // namespace tcheck
