#include "password_search.hpp"
#include <regex>

namespace tcheck
{

PasswordProcessor::PasswordProcessor()
    : expression_{
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
        // TODO RYS find better solution for forbidding "...!af!..." in password, 
        // only when no other special symbols given
        R"((?=.*(?:\!.*[@#$%^&*?]|@.*[!#$%^&*?]|#.*[!@$%^&*?]|\$.*[!@#%^&*?]|%.*[!@#$^&*?]|\^.*[!@#$%&*?]|&.*[!@#$%^*?]|\*.*[!@#$%^&?]|\?.*[!@#$%^&*])))"
        // at least 8 symbols from list
        R"([A-Za-z\d!@#$%^&*?]{8,}$)"
    }
{}

bool PasswordProcessor::IsValid(string const & password) const
{
    std::smatch m;
    return std::regex_match(password, m, expression_);
}

vector<string> PasswordProcessor::Match(string const &)
{
    return {};
}

}
