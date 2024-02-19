#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

namespace utils
{
template <typename T>
struct semaphore_lock_guard
{
    T& m_;
    semaphore_lock_guard(T& m)
        : m_(m)
    {
        m_.acquire();
    }
    ~semaphore_lock_guard()
    {
        m_.release();
    }
};

constexpr std::string_view packFlag{"--pack"};
constexpr std::string_view unpackFlag{"--unpack"};

inline void printBadUsage() noexcept
{
    std::cerr << "Usage: ./CXXRescue [--pack | --unpack] <directory | log file>" << std::endl;
}

inline bool checkDirectory(const std::filesystem::path& dirPath) noexcept
{
    if (!std::filesystem::exists(dirPath) || !std::filesystem::is_directory(dirPath))
    {
        std::cerr << "Invalid directory.\n";
        printBadUsage();
        return false;
    }
    return true;
}

inline bool checkFile(const std::filesystem::path& filePath) noexcept
{
    if (!std::filesystem::exists(filePath) || !std::filesystem::is_regular_file(filePath))
    {
        std::cerr << "Invalid file.\n";
        printBadUsage();
        return false;
    }
    return true;
}
} // namespace utils