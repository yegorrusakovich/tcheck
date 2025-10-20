#ifndef TCHECK_PASSWORD_SEARCH_HPP_
#define TCHECK_PASSWORD_SEARCH_HPP_

#include <regex>

namespace tcheck {

class PatternMatcher {
 public:
  explicit PatternMatcher(std::string_view regexp);
  virtual ~PatternMatcher() = default;

  /// no string allocations, text must outlive calls and result usage
  virtual bool IsValid(std::string_view text) const;
  virtual std::vector<std::string_view> Match(std::string_view text) const;
  virtual std::vector<std::string_view> Search(std::string_view text) const;
  virtual std::string GetUserErrorDescription() const;

 private:
  std::regex expression_;
};

}  // namespace tcheck
#endif
