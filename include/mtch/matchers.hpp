#include <mtch/pattern_matcher.hpp>

namespace tcheck
{

struct PasswordMatcher : PatternMatcher
{
    PasswordMatcher();
};

/// @{
/// Utility matchers
struct Ipv4Matcher : PatternMatcher
{
    Ipv4Matcher();
};
struct HostNameMatcher : PatternMatcher
{
    HostNameMatcher();
};
/// @}


/// @brief uri validator and matcher which can precisely tell what part of 
/// uri can not be matched
/// @details base expression_ used for main matcher (that match by 
/// scheme, host, path...), others for detailed parts validation
/// @attention single threaded
struct UriMatcher : PatternMatcher
{
    UriMatcher();
    bool IsValid(string const & text) const override;
private:
    Ipv4Matcher ipv4_matcher_;
    HostNameMatcher host_matcher_;
};

}
