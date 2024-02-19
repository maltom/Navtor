#include <filesystem>
#include <fstream>
#include <mutex>
#include <optional>
#include <semaphore>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace fileFunctions
{
std::optional<std::string> calculateFileHash(const std::filesystem::path& filePath);

void processSingleFile(const std::filesystem::path& filePath,
                       std::unordered_multimap<std::string, std::string>& fileAdresses,
                       std::unordered_set<std::string>& uniqueFileHashes,
                       std::mutex& fileHashesMutex,
                       std::counting_semaphore<>& semaphore);

bool concatenateLogs(const std::filesystem::path& targetPath,
                     std::unordered_multimap<std::string, std::string>& fileAdresses,
                     std::unordered_set<std::string>& uniqueFileHashes);

bool unpackRescuedFile(const std::filesystem::path& filePath);

} // namespace fileFunctions
