#include <future>
#include <iostream>

// Task with variadic parameters:
template <class ...Args>
bool task(Args... args)
{
    std::cout << "[task]: Task started." << std::endl;
    std::cout << "[task]: Executing time 1 seconds." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "[task]: Task complete." << std::endl;
//    throw std::exception(); // just for test exception's throwing
    return true;
}

// Specialization of task for more than one parameters:
template<class T, class ...Args>
bool task(T t, Args... args)
{
    const auto count = sizeof...(Args) + 1;
    std::cout << "[task]: Task with " << count << " parameters started." << std::endl;
    std::cout << "[task]: Executing time 3 seconds." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(count * 2));
    std::cout << "[task]: Task complete." << std::endl;
    return true;
}

bool test()
{
    //some parameters:
    int i = 0, j = 0;
    std::string str{"qwerty"};
    auto tuple = std::make_tuple(i, j, str);

    bool success = false;
    try {
        auto future1 = std::async(std::launch::async, task<>);
        auto future2 = std::async(std::launch::async, task<int>, i);
        auto future3 = std::async(std::launch::async, task<decltype(tuple)>, std::move(tuple));
        auto future4 = std::async(std::launch::async, task<int, int, std::string>, i, j, str);
        success = future1.get()
                & future2.get()
                & future3.get()
                & future4.get();
    } catch (std::exception const &err) {
        std::cerr << err.what() << std::endl;
        return success;
    }
    return success;
}

int main()
{
    try {
        auto future = std::async(test);
        std::cout << "Test has been started. Waiting.\n";

        std::future_status status;
        do {
            status = future.wait_for(std::chrono::seconds(1));
            switch (status) {
                case std::future_status::timeout:
                    std::cout << "Work in process.\n";
                    break;
                case std::future_status::deferred:
                    std::cout << "Work is deferred.\n";
                    throw std::exception{};
                case std::future_status::ready:
                    std::cout << "Work is ready!\n";
                    std::cout << std::boolalpha << "Test completed. [" << future.get() << "]\n";
                    break;
            }
        } while (status != std::future_status::ready);

    } catch (std::exception const &err) {
        std::cerr << err.what() << std::endl;
        return 1;
    }

    return 0;
}