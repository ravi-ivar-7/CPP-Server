#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unistd.h> // For sysconf(_SC_NPROCESSORS_ONLN)

std::string getCpuInfo()
{
    try
    {
        std::ifstream cpuInfoFile("/proc/cpuinfo");
        if (!cpuInfoFile.is_open())
            throw std::runtime_error("Failed to open CPU-INFO file");

        std::stringstream buffer;
        buffer << cpuInfoFile.rdbuf();
        cpuInfoFile.close();
        return buffer.str();
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error in getCpuInfo(): " << ex.what() << std::endl;
        throw;
    }
}

std::string getMemoryInfo()
{
    try
    {
        std::ifstream memInfoFile("/proc/meminfo");
        if (!memInfoFile.is_open())
            throw std::runtime_error("Failed to open MEMORY-INFO file");

        std::stringstream buffer;
        buffer << memInfoFile.rdbuf();
        memInfoFile.close();
        return buffer.str();
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error in getMemoryInfo(): " << ex.what() << std::endl;
        throw;
    }
}

long getUpTime()
{
    try
    {
        std::ifstream uptimeFile("/proc/uptime");
        if (!uptimeFile.is_open())
            throw std::runtime_error("Failed to open uptime file");

        double uptime;
        uptimeFile >> uptime;
        uptimeFile.close();
        return static_cast<long>(uptime);
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error in getUpTime(): " << ex.what() << std::endl;
        throw;
    }
}

int getCoresNum()
{
    try
    {
        int cores = sysconf(_SC_NPROCESSORS_ONLN);
        if (cores == -1)
            throw std::runtime_error("Failed to get CPU-CORES");

        return cores;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error in getCoresNum(): " << ex.what() << std::endl;
        throw;
    }
}
