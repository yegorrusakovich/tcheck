#include <mtch/matchers.hpp>

namespace tcheck {

PasswordMatcher::PasswordMatcher()
    : PatternMatcher{
          // no identicall symbols in a row
          R"(^(?!.*(.)\1))"
          // at least one lowercase
          "(?=.*[a-z])"
          // at least one uppercase
          "(?=.*[A-Z])"
          // at least one digit
          R"((?=.*\d))"
          // at least 2 special symbols
          "(?=.*[!@#$%^&*?].*[!@#$%^&*?])"
          // TODO RYS find better solution for forbidding "...!af!..." in
          // password,
          // only when no other special symbols given
          R"((?=.*(?:\!.*[@#$%^&*?]|@.*[!#$%^&*?]|#.*[!@$%^&*?]|\$.*[!@#%^&*?]|%.*[!@#$^&*?]|\^.*[!@#$%&*?]|&.*[!@#$%^*?]|\*.*[!@#$%^&?]|\?.*[!@#$%^&*])))"
          // at least 8 symbols from list
          R"([A-Za-z\d!@#$%^&*?]{8,}$)"} {}

UriMatcher::UriMatcher()
    : PatternMatcher{// supported schemes
                     "^((https?|ftp|ssh|smb|sftp))"
                     // scheme delimeter
                     "://"
                     // userinfo part (optional)
                     "([^/@]*@)?"
                     // host
                     "([^/:]+)"
                     // port, (optional, not capturing)
                     R"((?::(\d*))?)"
                     // path
                     "([^?#]*)"
                     // params (optional, not capturing?)
                     R"((?:\?([^#]*))?)"
                     // fragment (optional, not capturing?)
                     "(?:#(.*))?$"

      } {}

// TODO RYS support ipv6 in host part
// TODO RYS host name no longer than 255 chars
// (https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1:~:text=no%20more%20than-,255,-characters%20in%20length)
// There is no specific scheme dependent uri rules that worse to be set on match
// level
bool UriMatcher::IsValid(string const& text) const {
  auto res = PatternMatcher::Match(text);
  if (res.empty()) return false;

  // 4 group is host
  if (!ipv4_matcher_.IsValid(res[4]) || !host_matcher_.IsValid(res[4]))
    return false;

  return true;
}

Ipv4Matcher::Ipv4Matcher()
    : PatternMatcher{R"(^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).)"
                     R"((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).)"
                     R"((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).)"
                     R"((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$)"} {}

HostNameMatcher::HostNameMatcher()
    : PatternMatcher{// TODO RYS fix that
                     "^[a-zA-Z0-9]([a-zA-Z0-9-]*[a-zA-Z0-9])?(\\.[a-zA-Z0-9](["
                     "a-zA-Z0-9-]*[a-zA-Z0-9])?)*$"} {}

}  // namespace tcheck
