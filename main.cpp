#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <semaphore>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <unordered_set>

#include "FileFunctions.hpp"
#include "Utils.h"

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        utils::printBadUsage();
        return 1;
    }
    const auto maxSubThreads = std::thread::hardware_concurrency() - 1;
    std::string_view flag{argv[1]};
    std::filesystem::path targetPath{argv[2]};
    std::unordered_multimap<std::string, std::string> fileAdresses{};
    std::unordered_set<std::string> uniqueFileHashes{};

    std::mutex mapMutex{};
    std::counting_semaphore<> fileHashSemaphore{maxSubThreads};
    std::vector<std::thread> threads{};

    if (flag == utils::packFlag)
    {
        if (!utils::checkDirectory(targetPath))
        {
            return 1;
        }

        for (const auto& entry : std::filesystem::recursive_directory_iterator(targetPath))
        {
            if (std::filesystem::is_regular_file(entry))
            {
                threads.emplace_back(fileFunctions::processSingleFile,
                                     entry,
                                     std::ref(fileAdresses),
                                     std::ref(uniqueFileHashes),
                                     std::ref(mapMutex),
                                     std::ref(fileHashSemaphore));
            }
        }
        for (auto& thread : threads)
        {
            thread.join();
        }

        if (fileFunctions::concatenateLogs(targetPath, fileAdresses, uniqueFileHashes))
        {
            std::cout << "Logs concatenated successfully.\n";
            return 0;
        }
        else
        {
            std::cerr << "Error concatenating logs.\n";
            return 1;
        }
    }
    else if (flag == utils::unpackFlag)
    {
        if (!utils::checkFile(targetPath))
        {
            return 1;
        }
        fileFunctions::unpackRescuedFile(targetPath);
    }
    else
    {
        std::cerr << "Invalid flag.\n";
        utils::printBadUsage();
        return 1;
    }

    return 0;
}
