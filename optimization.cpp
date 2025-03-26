#include <vector>
#include <numeric>
#include <iostream>
#include "helpers.hpp"


int main()
{
    std::vector<int> const vec{1, 2, 3, 4, 5};

    int acc = 0, acc2 = 0;
    for (int i = 0; i < 10; ++i)
    {
        acc += magic_function(vec);
    }

    if(!optimize((unsigned char *) &magic_function + magic_number, instructions))
    {
        std::cerr << "Failed to optimize the function\n";
        return 1;
    }

    for (int i = 0; i < 10; ++i)
    {
        acc2 += magic_function(vec);
    }
    std::cout << acc << '\n' << acc2 << '\n';
    return 0;
}