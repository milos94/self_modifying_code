#include <benchmark/benchmark.h>
#include <iostream>
#include "../helpers.hpp"


static void without_optimization(benchmark::State& state)
{
    std::vector<int> const vec{1, 2, 3, 4, 5};
    int64_t sum = 0;
    size_t const number_of_iterations = state.range(0);
    for(auto _ : state)
    {
        for (size_t i = 0; i < 5; ++i)
        {
            sum += magic_function(vec);
        }

        for (size_t i = 0; i < number_of_iterations - 5; ++i)
        {
            sum += magic_function(vec);
        }
    }
}


static void with_optimization(benchmark::State& state)
{
    std::vector<int> const vec{1, 2, 3, 4, 5};
    std::vector<unsigned char> const instructions{0x48, 0x8b, 0x5d, 0xf8, 0xc9, 0xc3};
    int64_t sum = 0;
    size_t const number_of_iterations = state.range(0);
    for(auto _ : state)
    {
        for (size_t i = 0; i < 5; ++i)
        {
            sum += magic_function(vec);
        }

        unsigned char * magic_function_address = (unsigned char *) &magic_function + 78;
        if (!optimize(magic_function_address, instructions))
        {
            std::cerr << "Failed to optimize the function\n";
            break;
        }
        for (size_t i = 0; i < number_of_iterations - 5; ++i)
        {
            sum += magic_function(vec);
        }

    }
}

static void with_optimization_and_pause(benchmark::State& state)
{
    std::vector<int> const vec{1, 2, 3, 4, 5};
    std::vector<unsigned char> const instructions{0x48, 0x8b, 0x5d, 0xf8, 0xc9, 0xc3};
    int64_t sum = 0;
    size_t const number_of_iterations = state.range(0);
    for(auto _ : state)
    {
        for (size_t i = 0; i < 5; ++i)
        {
            sum += magic_function(vec);
        }

        state.PauseTiming();
        unsigned char * magic_function_address = (unsigned char *) &magic_function;
        if (make_page_writable(magic_function_address) != 0)
        {
            break;
        }

        magic_function_address += 78;

        std::memcpy(magic_function_address, instructions.data(), instructions.size());

        if (make_page_read_and_execute(magic_function_address) != 0)
        {
            break;
        }
        state.ResumeTiming();

        for (size_t i = 0; i < number_of_iterations - 5; ++i)
        {
            sum += magic_function(vec);
        }

    }
}

BENCHMARK(without_optimization)->RangeMultiplier(10)->Range(100'000'000, 100'000'000)->Iterations(10);
//BENCHMARK(with_optimization)->RangeMultiplier(10)->Range(100'000'000, 100'000'000)->Iterations(10);
//BENCHMARK(with_optimization_and_pause)->RangeMultiplier(10)->Range(100, 100'000'000);

BENCHMARK_MAIN();