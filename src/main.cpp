#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>

#include "FizzBuzz.hpp"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "An integer argument is required to run this program!\n"
                  << "Example: \"" << argv[0] << " 100\"\n";
        return 1;
    }

    int n;
    try
    {
        n = std::stoi(argv[1]);
    }
    catch (const std::invalid_argument& ia)
    {
        std::cerr << "Error: Invalid argument. Not a number: " << argv[1] << "\n";
        return 1;
    }
    catch (const std::out_of_range& oor)
    {
        std::cerr << "Error: Argument out of range: " << argv[1] << "\n";
        return 1;
    }

    const auto start = std::chrono::high_resolution_clock::now();
    FizzBuzz(n);
    const auto end = std::chrono::high_resolution_clock::now();
    const auto durationMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Program execution time: " << durationMicroseconds.count() << " Microseconds" << "\n";

    return 0;
}