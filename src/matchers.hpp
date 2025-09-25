#include "pattern_matcher.hpp"

namespace tcheck
{

struct PasswordMatcher : PatternMatcher
{
    PasswordMatcher();
};

struct UriMatcher : PatternMatcher
{
    UriMatcher();
};

}
