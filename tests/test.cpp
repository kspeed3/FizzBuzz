#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cassert>
#include <stdexcept>

// Function to execute a command and get its output
std::string exec(const char* cmd)
{
    char buffer[128];
    std::string result = "";
    FILE* pipe;
    #ifdef _WIN32
        pipe = _popen(cmd, "r");
    #else
        pipe = popen(cmd, "r");
    #endif
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    try
    {
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) 
        {
            result += buffer;
        }
    }
    catch (...)
    {
        #ifdef _WIN32
            _pclose(pipe);
        #else
            pclose(pipe);
        #endif
        throw;
    }
    #ifdef _WIN32
        _pclose(pipe);
    #else
        pclose(pipe);
    #endif
    return result;
}

void FizzBuzz(int n)
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

int main()
{
    const int NUMBER = 10000000;

    #ifdef _WIN32
        std::string command = "build\\Project.exe ";
    #else
        std::string command = "build/Project ";
    #endif
    command += std::to_string(NUMBER);

    std::string output = exec(command.c_str());
    std::string time;
    const size_t pos = output.find("Program");
    if (pos != std::string::npos)
    {
        time = output.substr(pos);
        output.erase(pos);
    }

    // Redirect cout to a stringstream
    std::stringstream match;
    std::streambuf* oldCout = std::cout.rdbuf(match.rdbuf());
    
    const auto start = std::chrono::high_resolution_clock::now();
    FizzBuzz(NUMBER);
    const auto end = std::chrono::high_resolution_clock::now();
    const auto durationMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Restore original cout
    std::cout.rdbuf(oldCout);

    std::cout << "\n\n"
              << "-----------------------------------\n\n";

    if (output != match.str())
    {
        std::cout << "\tFizzBuzz test failed\n\n"
                  << "\tOutput does not match expected\n\n"
                  << "-----------------------------------\n"
                  << "\n";
        return 1;
    }

    std::cout << "\tFizzBuzz test passed\n\n"
              << "-----------------------------------\n"
              << "\n";

    std::cout << "\n\n"
              << "-----------------------------------\n\n"
              << "\tPERFORMANCE INFO:\n\n"
              << time
              << "Common answer execution time: " << durationMicroseconds.count() << " Microseconds" << "\n\n"
              << "-----------------------------------\n";

    return 0;
}
