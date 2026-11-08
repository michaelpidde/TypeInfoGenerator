#pragma once

#include "Tokenizer.h"

#include <filesystem>
#include <string>
#include <vector>

struct ParsedField {
    std::string type;
    std::string name;
    bool isPointer;
    // This is a string because sometimes the array extent is defined by a constant (i.e. MAX_TILES)
    std::string arrayExtent;
    int line;
};

struct ParsedEnumValue {
    std::string name;
};

struct ParsedType {
    enum class Kind {
        Struct,
        Enum,
    } kind;

    std::string name;
    std::vector<ParsedField> fields;
    std::vector<ParsedEnumValue> values;
    // If it's an enum class, scoped = true
    bool scoped;
    fs::path file;
    int line;
};

std::vector<ParsedType> Parse(std::vector<Token> tokens);