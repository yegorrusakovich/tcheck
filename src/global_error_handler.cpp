#include "global_error_handler.hpp"

#include <cassert>

void GlobalErrorHandler()
{
    try {
        throw;
    } catch (...) {
        assert(false); // TODO RYS implement
        throw;
    }
}
