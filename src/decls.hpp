#ifndef TCHECK_DECLS_HPP_
#define TCHECK_DECLS_HPP_

#include <vector>

namespace tcheck
{
template<typename T>
using vector = std::pmr::vector<T>;

using string = std::pmr::string;
}
#endif
