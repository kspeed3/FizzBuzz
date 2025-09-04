#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>

#include "FizzBuzz.hpp"

template<class T>
class test
{
public:
    test(const std::string& output, const T& runtime) : output(output), runtime(runtime) { }

public:
    const std::string output;
    const T runtime;

public:
    friend std::ostream& operator<<(std::ostream& out, const test& object) { return out << object.runtime.count(); }
    bool operator!=(const test& object) { return output != object.output; }
};

void controlFizzBuzz(int n)
{
    for (int i = 1; i <= n; ++i)
    {
        if (i % 15 == 0)
            std::cout << "FizzBuzz\n";
        else if (i % 3 == 0)
            std::cout << "Fizz\n";
        else if (i % 5 == 0)
            std::cout << "Buzz\n";
        else
            std::cout << i << "\n";
    }
}

test<std::chrono::microseconds> getOutput(std::function<void(int n)> getFizzBuzz, int n)
{
    // Redirect cout to a stringstream
    std::stringstream output;
    std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());

    const auto start = std::chrono::high_resolution_clock::now();
    getFizzBuzz(n);
    const auto end = std::chrono::high_resolution_clock::now();
    const auto durationMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Restore original cout
    std::cout.rdbuf(oldCout);

    return test<std::chrono::microseconds>(output.str(), durationMicroseconds);
}

int main()
{
    const int NUMBER = 10000000;

    const std::function<void(int n)> user = FizzBuzz;
    auto output = getOutput(user, NUMBER);

    const std::function<void(int n)> control = controlFizzBuzz;
    auto match = getOutput(control, NUMBER);

    std::cout << "\n\n"
              << "--------------------------------------------\n\n";

    if (output != match)
    {
        std::cout << "\tFizzBuzz test failed :(\n\n"
                  << "\tOutput does not match expected\n\n"
                  << "--------------------------------------------\n"
                  << "\n";
        return 1;
    }

    std::cout << "\tFizzBuzz test passed :)\n\n"
              << "--------------------------------------------\n"
              << "\n";

    std::cout << "\n\n"
              << "--------------------------------------------\n\n"
              << "\tPERFORMANCE INFO:\n\n"
              << "Program execution time: " << output << " Mircroseconds" << "\n"
              << "Common answer execution time: " << match << " Microseconds" << "\n\n"
              << "--------------------------------------------\n";

    return 0;
}
