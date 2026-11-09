#include "Tokenizer.h"
#include "Parser.h"

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <fstream>
#include <optional>

namespace fs = std::filesystem;

std::optional<std::string> ReadFile(const fs::path& path) {
    std::ifstream file(path, std::ios::binary);
    if(!file) {
        std::cout << "Error reading header " << path << std::endl;
        return std::nullopt;
    }

    std::error_code err;
    const size_t size = fs::file_size(path, err);
    if(err) {
        std::cout << "Error getting header " << path << " file size" << std::endl;
        return std::nullopt;
    }

    std::string contents(size, '\0');
    file.read(contents.data(), static_cast<std::streamsize>(size));
    if(!file) {
        std::cout << "Error reading header " << path << " file contents" << std::endl;
        return std::nullopt;
    }

    return contents;
}


int main(int argc, char* argv[]) {
    if(argc == 1) {
        std::cout << "\nError: Source code path argument required.\n\n";
        return 1;
    }

    fs::path sourcePath = argv[1];

    std::cout << "\nScanning source path " << sourcePath << " for headers...\n\n";

    std::vector<fs::path> headers = {};

    for(const auto& entry : fs::recursive_directory_iterator(sourcePath)) {
        if(entry.is_regular_file() && entry.path().extension() == ".h") {
            headers.push_back(entry.path());
        }
    }

    std::cout << "Found " << headers.size() << " headers...\n\n";

    for(const auto& path : headers) {
        auto contents = ReadFile(path);

        if(!contents.has_value()) {
            std::cout << "Error: Header " << path << " has no contents.\n\n";
            return 1;
        }

        std::vector<Token> tokens = Tokenize(path, contents.value());
        if(tokens.size() > 0) {
            std::cout << path << std::endl;
        }
        std::vector<ParsedType> types = Parse(path, tokens);
    }

    return 0;
}