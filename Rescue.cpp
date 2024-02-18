#include <array>
#include <iostream>
#include <filesystem>
#include <string>


int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cerr<<"Error: No directory path provided"<<std::endl;
        return 1;
    }

    if(argv[1] == nullptr)
    {
        std::cerr<<"Error: No directory path provided"<<std::endl;
        return 1;
    }

    std::string mainDirPathStr{argv[1]};
    
    // {
    //     std::cerr<<"Error: No directory path provided"<<std::endl;
    //     return 1;
    // }
    // std::filesystem::path mainDirPath{mainDirPathStr};

    
    // for( auto& singleFile : std::filesystem::directory_iterator{mainDirPath})
    // {
    //     std::fstream file (singleFile.path(),std::ios::in);

    //     std::string firstLine;
    //     std::getline(file,firstLine);

    //     if(firstLine.empty())
    //     {
    //         continue;
    //     }
    //     file.close();
    //     if( ( firstLine[0] >= 'a' && firstLine[0] <= 'z' )  || ( firstLine[0] >= 'A' && firstLine[0] <= 'Z' )||( firstLine[0] >= '0' && firstLine[0] <= '9' ) )
    //     {
    //         std::string subdirName(1,firstLine[0]);
    //         std::filesystem::path subPath = mainDirPath;
    //         subPath.append(subdirName);
    //         if(!std::filesystem::is_directory(subdirName))
    //         {
                
    //             std::filesystem::create_directory(subPath);
    //         }
    //         std::filesystem::path newName = subPath;
    //         newName.append(singleFile.path().filename().string());
    //         std::filesystem::rename(singleFile,newName);
    //     }
    //     else
    //     {
    //         continue;
    //     }
    // }

    return 0;
}