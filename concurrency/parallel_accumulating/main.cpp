#include <thread>
#include <iostream>
#include <cstdint>
#include <numeric> /* accumulate */
#include <functional> /* ref */
#include <array>
#include <vector>

template<class Iterator, class T>
void accumulate(Iterator beg, Iterator end, T &result)
{
    result = std::accumulate(beg, end, 0);
}

template<class Iterator, class T>
void parallel_accumulate(Iterator beg, Iterator end, T &result)
{
    auto const length = std::distance(beg, end);
    if (length <= 0) return;
    std::size_t const min_per_thread = 16;
    std::size_t const hardware_threads = std::thread::hardware_concurrency();
    std::size_t const max_threads = // minimum one thread:
            (length - 1) / min_per_thread + 1;
    std::size_t const num_threads = // minimum one thread:
            std::min(hardware_threads != 0 ? hardware_threads : 1, max_threads);
    std::size_t const block_size = length / num_threads;

    std::vector<T> results{num_threads};
    std::vector<std::thread> threads{num_threads};

    Iterator
            block_beg{beg},
            block_end{beg};
    std::size_t i{0};
    for (; i < num_threads - 1; ++i) {
        std::advance(block_end, block_size);
        threads[i] = std::thread(
                accumulate<Iterator, T>,
                block_beg,
                block_end,
                std::ref(results[i]));
        block_beg = block_end;
    }

    threads[i] = std::thread(
            accumulate<Iterator, T>,
            block_beg,
            end,
            std::ref(results[i]));

    for (auto &thread : threads) {
        thread.join();
    }

    result = std::accumulate(results.cbegin(), results.cend(), 0);
}

int main()
{
    // initialize array:
    constexpr std::size_t array_size{1000000}; // 10^6
    typedef uint8_t array_t;
    std::array<array_t, array_size> array{};

    // initialize array's data:
    for (auto &elem : array) {
        elem = rand() % std::numeric_limits<array_t>::max();
    }

    uint64_t result1{0};
    auto start = std::chrono::high_resolution_clock::now();
    accumulate(array.cbegin(), array.cend(), result1);
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Not parallel accumulating took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << "us "
              << "with result " << result1 << ".\n";

    uint64_t result2{0};
    start = std::chrono::high_resolution_clock::now();
    parallel_accumulate(array.cbegin(), array.cend(), result2);
    end = std::chrono::high_resolution_clock::now();

    std::cout << "Parallel accumulating took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << "us "
              << "with result " << result2 << ".\n";
    return 0;
}