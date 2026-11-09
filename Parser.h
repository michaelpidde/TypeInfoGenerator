#pragma once

#include "Tokenizer.h"

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct Annotation {
    std::string_view name;
    std::string_view args;
};

struct ParsedField {
    std::string type;
    std::string name;
    bool isPointer = false;
    // This is a string because sometimes the array extent is defined by a constant (i.e. MAX_TILES)
    std::string arrayExtent;
    int line = -1;
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
    bool scoped = false;
    fs::path file;
    int line = -1;
};

struct Cursor {
    const std::vector<Token>& tokens;
    size_t position = 0;

    // const after parameter declaration means it's a const member function, i.e. won't modify the object (struct) it's called on
    // It also ensures that if the Cursor instance is a const, we can call these member functions on it without a compiler error
    const Token& Peek(size_t amount = 0) const {
        return tokens[std::min(position + amount, tokens.size() - 1)];
    }

    bool IsIdentifier(std::string_view text) const {
        return Peek().kind == TokenKind::Identifier && Peek().text == text;
    }

    bool IsPunctuation(std::string_view text) const {
        return Peek().kind == TokenKind::Punctuation && Peek().text == text;
    }

    void Advance() {
        if(Peek().kind != TokenKind::End) {
            ++position;
        }
    }
};

std::vector<ParsedType> Parse(const fs::path& path, std::vector<Token>& tokens);
bool ParseStruct(Cursor& cursor, ParsedType& out);
bool ParseEnum(Cursor& cursor, ParsedType& out);
bool ParseField(Cursor& cursor, ParsedField& out);
bool HasAnnotation(std::string_view comment, std::string_view name);
void PrintType(const ParsedType& type);