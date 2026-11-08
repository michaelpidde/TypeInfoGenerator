#include "Tokenizer.h"

#include <cctype>
#include <iostream>

static bool IsIdentityChar(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

std::vector<Token> Tokenize(const fs::path path, const std::string& src) {
    std::vector<Token> tokens;
    int line = 1;
    size_t i = 0;

    while(i < src.size()) {
        char c = src[i];

        if(c == '\n') {
            ++line;
            ++i;
            continue;
        }

        if(isspace((unsigned char)c)) {
            ++i;
            continue;
        }

        // Annotations begin after comment slashes
        if(c == '/' && src[i+1] == '/') {
            size_t start = i;
            while(i < src.size() && src[i] != '\n') {
                ++i;
            }
            tokens.push_back({
                TokenKind::Comment,
                {&src[start], i - start},
                line
            });
            continue;
        }

        if(c == '#') {
            while(i < src.size() && src[i] != '\n') {
                ++i;
            }
            continue;
        }

        if(c == '"' || c == '\'') {
            const char quote = c;
            size_t start = i++;
            while(i < src.size() && src[i] != quote) {
                if(src[i] == '\\') {
                    ++i;
                } else if(src[i] == '\n') {
                    ++line;
                }
                ++i;
            }
            if(src[i] == quote) {
                // Increment past closing quote
                ++i;
            }
            tokens.push_back({
                TokenKind::String,
                {&src[start], i - start},
                line
            });
            continue;
        }

        if(std::isalpha((unsigned char)c) || c == '_') {
            size_t start = i;
            while(i < src.size() && IsIdentityChar(src[i])) {
                ++i;
            }
            tokens.push_back({
                TokenKind::Identifier,
                {&src[start], i - start},
                line
            });
            continue;
        }

        if(std::isdigit((unsigned char)c)) {
            size_t start = i;
            while(i < src.size() && (std::isdigit((unsigned char)src[i]) || src[i] == '.')) {
                ++i;
            }
            tokens.push_back({
                TokenKind::Number,
                {&src[start], i - start},
                line
            });
            continue;
        }

        switch(c) {
            case '{':
            case '}':
            case ';':
            case '*':
            case '&':
            case '<':
            case '>':
            case ',':
            case '[':
            case ']':
            case '(':
            case ')':
            case '=':
            case ':':
            case '-':
                tokens.push_back({
                    TokenKind::Punctuation,
                    {&src[i], 1},
                    line
                });
                ++i;
                continue;
            default:
                std::cout << "Warning: Header " << path << "(" << line << "): unexpected character " << src[i] << "\n\n";
        }

        ++i;
    }

    tokens.push_back({
        TokenKind::End,
        {},
        line
    });

    return tokens;
}