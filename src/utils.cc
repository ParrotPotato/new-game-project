#include "utils.hh"

#include <cstdio>
#include <cstdlib>
#include <string>

using std::string;

char * read_and_load_file_in_buffer(const char * file_path){
    FILE *file = fopen(file_path, "r");
    if (file == nullptr)
    {
        perror("Error opening file");
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *fileContents = (char *)malloc(fileSize + 1); // +1 for null terminator
    if (!fileContents)
    {
        perror("Error allocating memory");
        fclose(file);
        return nullptr;
    }

    size_t bytesRead = fread(fileContents, 1, fileSize, file);
    if (bytesRead != fileSize)
    {
        perror("Error reading file");
        free(fileContents);
        fclose(file);
        return nullptr;
    }

    fileContents[fileSize] = '\0';
    fclose(file);

    return fileContents;
}
