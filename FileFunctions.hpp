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

// namespace fs = std::filesystem;

// // Function to calculate the hash of a file
// std::size_t calculateFileHash(const std::string& filePath)
// {
//     std::ifstream file(filePath, std::ios::binary);
//     if (!file.is_open())
//     {
//         return 0; // Unable to open file, return a dummy hash
//     }

//     // Use a hash function (e.g., std::hash) to calculate the hash of the file content
//     std::hash<std::string> hasher;
//     std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
//     return hasher(content);
// }

// // Function to process a directory recursively
// void processDirectory(const fs::path& dirPath)
// {
//     // Store file hashes in a set to detect duplicates
//     std::unordered_set<std::size_t> fileHashes;

//     // Concatenate content of all logs in the directory
//     std::string concatenatedContent;

//     for (const auto& entry : fs::directory_iterator(dirPath))
//     {
//         if (fs::is_directory(entry))
//         {
//             // Recursively process subdirectories
//             processDirectory(entry.path());
//         }
//         else if (fs::is_regular_file(entry))
//         {
//             // Check for duplicate files based on content hash
//             std::size_t hash = calculateFileHash(entry.path().string());
//             if (fileHashes.find(hash) == fileHashes.end())
//             {
//                 // File is not a duplicate, process it
//                 fileHashes.insert(hash);

//                 // Append content to the concatenated string
//                 std::ifstream file(entry.path());
//                 concatenatedContent +=
//                     std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
//                 file.close();
//             }
//         }
//     }

//     // Write concatenated content to a single file in the directory
//     fs::path outputFile = dirPath / "concatenated_log.txt";
//     std::ofstream output(outputFile);
//     output << concatenatedContent;
//     output.close();

//     std::cout << "Processed directory: " << dirPath << std::endl;
// }

// int main(int argc, char* argv[])
// {
//     if (argc != 2)
//     {
//         std::cerr << "Usage: " << argv[0] << " <starting_directory>" << std::endl;
//         return 1;
//     }

//     fs::path startPath(argv[1]);

//     if (!fs::exists(startPath) || !fs::is_directory(startPath))
//     {
//         std::cerr << "Invalid starting directory." << std::endl;
//         return 1;
//     }

//     processDirectory(startPath);

//     std::cout << "Processing complete." << std::endl;
//     return 0;
// }