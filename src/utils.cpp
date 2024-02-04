#include "utils.h"

#include <cstdio>
#include <cstdlib>
#include <string>

#include <iostream>
#include <optional>

using std::cerr;
using std::endl;
using std::string;

namespace utils {
    std::optional<string> load_file_buffer(const char * file_path){
        FILE *file = fopen(file_path, "r");
        if (file == nullptr)
        {
            perror("Error opening file");
            return std::nullopt;
        }

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        char *fileContents = (char *)malloc(fileSize + 1); // +1 for null terminator
        if (!fileContents)
        {
            perror("Error allocating memory");
            fclose(file);
            return std::nullopt;
        }

        size_t bytesRead = fread(fileContents, 1, fileSize, file);
        if (bytesRead != fileSize)
        {
            perror("Error reading file");
            free(fileContents);
            fclose(file);
            return std::nullopt;
        }

        fileContents[fileSize] = '\0';
        fclose(file);

        std::string result(fileContents);;
        free(fileContents);

        return result;
    }
}
