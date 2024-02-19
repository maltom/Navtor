#include "FileFunctions.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <mutex>
#include <optional>
#include <semaphore>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <cryptopp/channels.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
// Can use different hash/checksum functions for optimal performance/collision risk
// #include <cryptopp/sha.h>

#include "Utils.h"
namespace fileFunctions
{
std::optional<std::string> calculateFileHash(const std::filesystem::path& filePath)
{
    std::string fileHash{};

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
    {
        return std::nullopt;
    }

    using namespace CryptoPP;

    Weak::MD5 hashFunc;

    HashFilter filter(hashFunc, new HexEncoder(new StringSink(fileHash)));

    ChannelSwitch channelSwitch;
    channelSwitch.AddDefaultRoute(filter);

    FileSource(file, true, new Redirector(channelSwitch));

    return fileHash;
}

void processSingleFile(const std::filesystem::path& filePath,
                       std::unordered_multimap<std::string, std::string>& fileAdresses,
                       std::unordered_set<std::string>& uniqueFileHashes,
                       std::mutex& fileHashesMutex,
                       std::counting_semaphore<>& semaphore)
{
    utils::semaphore_lock_guard lockSem(semaphore);
    const auto fileHash = fileFunctions::calculateFileHash(filePath);
    if (fileHash.has_value())
    {
        std::lock_guard<std::mutex> lockMut(fileHashesMutex);
        uniqueFileHashes.insert(fileHash.value());
        fileAdresses.insert({fileHash.value(), filePath.string()});
    }
}

bool concatenateLogs(const std::filesystem::path& targetPath,
                     std::unordered_multimap<std::string, std::string>& fileAdresses,
                     std::unordered_set<std::string>& uniqueFileHashes)
{
    const auto logPath = std::filesystem::current_path() / "CXXRescue.rescued";

    std::ofstream output(logPath, std::ios::binary | std::ios::ate | std::ios::trunc);
    if (!output.is_open())
    {
        std::cerr << "Unable to open log file for writing.\n";
        return false;
    }
    output << fileAdresses.size() << "\n";
    output << uniqueFileHashes.size() << "\n";

    for (const auto& hash : uniqueFileHashes)
    {
        const auto pathsForHash = fileAdresses.equal_range(hash);
        const auto siblingFileCount = std::distance(pathsForHash.first, pathsForHash.second);
        output << hash << "\n" << siblingFileCount << "\n";

        std::for_each(pathsForHash.first,
                      pathsForHash.second,
                      [&output, &targetPath](const auto& paths)
                      {
                          const auto relativePath = std::filesystem::relative(paths.second, targetPath);
                          output << relativePath.string() << "\n";
                      });

        const auto pathToAnySiblingFile = fileAdresses.find(hash)->second;
        const auto siblingFileSize = std::filesystem::file_size(pathToAnySiblingFile);
        std::ifstream fileToBeRescued(pathToAnySiblingFile, std::ios::binary);

        if (!fileToBeRescued.is_open())
        {
            std::cerr << "Unable to open file for reading: " << pathToAnySiblingFile << "\n";
            return false;
        }
        output << siblingFileSize << "\n";
        output << fileToBeRescued.rdbuf();
    }
    output.close();
    return true;
}

bool unpackRescuedFile(const std::filesystem::path& filePath)
{
    std::ifstream rescuedFile(filePath, std::ios::binary);
    if (!rescuedFile.is_open())
    {
        std::cerr << "Unable to open rescued file for reading.\n";
        return false;
    }

    const auto unpackDirPath = std::filesystem::current_path() / "CXXRescueUnpacked";
    auto rescuedFilesCount = 0;

    if (std::filesystem::exists(unpackDirPath))
    {
        std::filesystem::remove_all(unpackDirPath);
    }
    std::filesystem::create_directory(unpackDirPath);

    std::string line{};
    std::getline(rescuedFile, line);
    const auto targetFileCount{std::stoi(line)};

    std::getline(rescuedFile, line);
    const auto uniqueFileCount{std::stoi(line)};

    for (auto iUniques = 0; iUniques < uniqueFileCount; ++iUniques)
    {
        // skip hash
        std::getline(rescuedFile, line);

        std::getline(rescuedFile, line);
        const auto siblingFileCount{std::stoi(line)};
        std::vector<std::filesystem::path> siblingFilesPaths{};
        siblingFilesPaths.reserve(siblingFileCount);

        for (auto iSiblings = 0; iSiblings < siblingFileCount; ++iSiblings)
        {
            std::getline(rescuedFile, line);
            siblingFilesPaths.emplace_back(unpackDirPath / line.c_str());
        }

        std::getline(rescuedFile, line);
        const auto siblingFileSize = std::stoll(line);

        // Write first sibling file
        const auto firstSiblingPath = siblingFilesPaths.front();
        std::filesystem::create_directories(firstSiblingPath.parent_path());

        std::ofstream siblingFile(firstSiblingPath, std::ios::binary);
        if (!siblingFile.is_open())
        {
            std::cerr << "Unable to open file for writing: " << firstSiblingPath << "\n";
            return false;
        }
        std::vector<char> buffer(siblingFileSize);
        rescuedFile.read(buffer.data(), siblingFileSize);
        siblingFile.write(buffer.data(), siblingFileSize);

        ++rescuedFilesCount;
        siblingFile.close();
        // copy files if there are more than one sibling
        for (auto iSiblings = 1; iSiblings < siblingFileCount; ++iSiblings)
        {
            if (const auto parentPath = siblingFilesPaths[iSiblings].parent_path();
                !std::filesystem::exists(parentPath))
            {
                std::filesystem::create_directories(parentPath);
            }
            std::filesystem::copy_file(firstSiblingPath, siblingFilesPaths[iSiblings]);

            ++rescuedFilesCount;
        }
    }
    return rescuedFilesCount == targetFileCount;
}

} // namespace fileFunctions
