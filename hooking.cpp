#include <iostream>
#include "helpers.hpp"


__attribute__ ((ms_hook_prologue))
int to_be_hooked(int a, int b)
{
    return a + b;
}


int hook(int a, int b)
{
    return a * b;
}

int main()
{
    unsigned char * hooked_function_address = (unsigned char *) &to_be_hooked;
    unsigned char * hook_address = (unsigned char *) &hook;

    std::cout << "Before hooking " << to_be_hooked(1,3) << '\n';

    if (!hotpatch(hooked_function_address, hook_address))
    {
        std::cerr << "Failed to hotpatch the function!\n";
        return 1;
    }

    std::cout << "After hooking " << to_be_hooked(1,3) << '\n';

    return 0;
}