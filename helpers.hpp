#pragma once

#include <unistd.h>
#include <sys/mman.h>
#include <cstring>
#include <span>
#include <vector>
#include <numeric>
#include <cstdint>

static constexpr ptrdiff_t magic_number = 78;
std::vector<unsigned char> const instructions{0x48, 0x8b, 0x5d, 0xf8, 0xc9, 0xc3};


int magic_function(std::vector<int> const & vec)
{
    static int counter = 0;
    auto const sum = std::accumulate(std::begin(vec), std::end(vec), 0);
    return (counter++ < 5)? sum + counter : sum;
}

bool make_page_writable(unsigned char * addr) {
    int page_size = getpagesize();
    addr -= (unsigned long)addr % page_size;

    return mprotect(addr, page_size, PROT_READ | PROT_WRITE | PROT_EXEC) == 0;
}

bool make_page_read_and_execute(unsigned char * addr) {
    int page_size = getpagesize();
    addr -= (unsigned long)addr % page_size;

    return mprotect(addr, page_size, PROT_READ | PROT_EXEC) == 0;
}


bool hotpatch(unsigned char * target, unsigned char * replacement)
{
    if (!make_page_writable(target))
    {
        return false;
    }
    uint32_t rel = replacement - target - 5;
    std::vector<unsigned char> instructions{0xe9, (unsigned char)rel, (unsigned char)(rel >> 8),
                                                  (unsigned char)(rel >> 16), (unsigned char)(rel >> 24)};

    std::memcpy(target, instructions.data(), instructions.size());
    if (!make_page_read_and_execute(target))
    {
        return false;
    }
    return true;
}

bool optimize(unsigned char * addr, std::span<unsigned char const> instructions)
{
    if (!make_page_writable(addr))
    {
        return false;
    }

    std::memcpy(addr, instructions.data(), instructions.size());

    if(!make_page_read_and_execute(addr))
    {
        return false;
    }

    return true;

}