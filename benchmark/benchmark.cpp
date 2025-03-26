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
    int64_t sum = 0;
    size_t const number_of_iterations = state.range(0);
    for(auto _ : state)
    {
        for (size_t i = 0; i < 5; ++i)
        {
            sum += magic_function(vec);
        }

        if (!optimize((unsigned char *) &magic_function + magic_number, instructions))
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
    int64_t sum = 0;
    size_t const number_of_iterations = state.range(0);
    for(auto _ : state)
    {
        for (size_t i = 0; i < 5; ++i)
        {
            sum += magic_function(vec);
        }

        state.PauseTiming();
        if (!optimize((unsigned char *) &magic_function + magic_number, instructions))
        {
            std::cerr << "Failed to optimize the function\n";
            break;
        }
        state.ResumeTiming();

        for (size_t i = 0; i < number_of_iterations - 5; ++i)
        {
            sum += magic_function(vec);
        }

    }
}

BENCHMARK(without_optimization)->DenseRange(1'000'000, 1'000'000, 100)->Iterations(100);
BENCHMARK(with_optimization)->DenseRange(1'000'000, 1'000'000, 100)->Iterations(100);
BENCHMARK(with_optimization_and_pause)->DenseRange(100, 1'000'000, 100)->Iterations(100);

BENCHMARK_MAIN();