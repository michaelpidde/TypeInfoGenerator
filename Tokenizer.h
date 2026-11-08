#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;

enum class TokenKind {
    Identifier,
    Number,
    String,
    Punctuation,
    Comment,
    End
};

struct Token {
    TokenKind kind;
    std::string_view text;
    int line;
};

std::vector<Token> Tokenize(const fs::path path, const std::string& src);

inline const char* TokenKindName(TokenKind kind) {
    switch(kind) {
        case TokenKind::Identifier:  return "Identifier ";
        case TokenKind::Number:      return "Number     ";
        case TokenKind::String:      return "String     ";
        case TokenKind::Punctuation: return "Punctuation";
        case TokenKind::Comment:     return "Comment    ";
        case TokenKind::End:         return "End        ";
        default:                     return "Unknown    ";
    }
}