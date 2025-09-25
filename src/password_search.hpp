#ifndef TCHECK_PASSWORD_SEARCH_HPP_
#define TCHECK_PASSWORD_SEARCH_HPP_

#include "decls.hpp"

#include <regex>

namespace tcheck
{

class PasswordProcessor
{
public:
    PasswordProcessor();
    bool IsValid(string const & password) const;
    vector<string> Match(string const & text);
private:
    std::regex expression_;
};

}
#endif
