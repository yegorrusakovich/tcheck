#include "decls.hpp"
#include "global_error_handler.hpp"
#include "matchers.hpp"

#include <iostream>
#include <memory_resource>
#include <string>

void MainLoop();
void SetUp();

int main(int, char **)
{
    try {
        SetUp();
        MainLoop();
    } catch (...) {
        GlobalErrorHandler();
    }
    return 0;
}

void MainLoop()
{
    using namespace tcheck;

    auto pool = std::pmr::unsynchronized_pool_resource{};
    auto alloc = std::pmr::polymorphic_allocator<char>{&pool};
    auto input = string{alloc};

    auto processor = PasswordMatcher{};

    while(std::getline(std::cin, input))
    {
        if (processor.IsValid(input))
            std::cout << input << " is correct\n";
        else
            std::cout << input << " is NOT correct\n";
    }
}

void SetUp()
{
    std::cin.exceptions(std::ios::badbit | std::ios::failbit);
    std::cout.exceptions(std::ios::badbit | std::ios::failbit);
}
