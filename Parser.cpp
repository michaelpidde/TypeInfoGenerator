#include "Parser.h"

#include <iostream>
#include <filesystem>

std::vector<ParsedType> Parse(const fs::path& path, std::vector<Token>& tokens) {
    std::vector<ParsedType> types;

    for(auto it = tokens.begin(); it != tokens.end(); ++it) {
        std::cout << TokenKindName(it->kind) << "(" << it->line << "): " << it->text << "\n";
    }

    for(size_t i = 0; i < tokens.size(); ++i) {
        if(tokens[i].kind != TokenKind::Comment || !HasAnnotation(tokens[i].text, "reflect")) {
            continue;
        }

        // Not doing anything with annotations right now if they exist, so just move past any Comment tokens
        while(tokens[i + 1].kind == TokenKind::Comment) {
            ++i;
        }

        // Increment past last comment
        ++i;

        Cursor cursor = {
            tokens,
            i
        };

        ParsedType structType;
        structType.file = path;
        structType.line = tokens[i].line;
        if(ParseStruct(cursor, structType)) {
            types.push_back(structType);
        }

        i = cursor.position;
    }

    return types;
}

bool ParseStruct(Cursor& cursor, ParsedType& out) {
    if(cursor.IsIdentifier("struct")) {
        cursor.Advance();

        out.kind = ParsedType::Kind::Struct;

        if(cursor.Peek().kind != TokenKind::Identifier) {
            // Edge case - this should be the name of the struct, not something else (would fail compiling the header anyhow)
            return false;
        }

        out.name = cursor.Peek().text;

        cursor.Advance();

        if(cursor.Peek().kind != TokenKind::Punctuation || cursor.Peek().text != "{") {
            // Edge case - this should open the struct (would fail compiling the header anyhow)
            return false;
        }

        cursor.Advance();

        while(true) {
            if(cursor.Peek().kind == TokenKind::Comment) {
                cursor.Advance();
                continue;
            }

            // TODO: This logic changes with a nested enum member
            if(cursor.IsPunctuation("}")) {
                break;
            }

            // Edge case (would fail compiling the header anyhow)
            if(cursor.Peek().kind == TokenKind::End) {
                std::cout << "Error: Unexpected end of file while parsing struct\n";
                return false;
            }

            ParsedField field;
            if(ParseField(cursor, field)) {
                out.fields.push_back(field);
            } else {
                // TODO: This shows the starting line, not the particular full field in the struct
                std::cout << "Error: " << out.file << "(" << out.line << "): Could not parse field...";
            }

            // Iterate over properties...
            // Normal case:
            //   - Identifier (type)
            //   - Identifier (name)
            //   - (Punctuation (=) & (Number | String)) | Punctuation (;)
            // Special case (enum/enum class):
            //   - (Identifier (enum) & optional Identifier (class))
            //   - Identifier (name)
            //   - Parse the rest of it...
        }

        // Advance past last "}"
        // Should end on ";" token when returning

        PrintType(out);
    } else {
        std::cout << "Warning: @reflect must be followed by a struct or enum.\n";
        return false;
    }

    return true;
}

// bool ParseEnum(Cursor& cursor, ParsedType& out) {
//     return true;
// }

bool ParseField(Cursor& cursor, ParsedField& out) {
    return true;
}

void PrintType(const ParsedType& type) {
    std::cout << type.file << "(" << type.line << ")\n";
    if(type.kind == ParsedType::Kind::Struct) std::cout << "Type: Struct\n";
    if(type.kind == ParsedType::Kind::Enum && type.scoped) std::cout << "Type: Enum Class\n";
    if(type.kind == ParsedType::Kind::Enum && !type.scoped) std::cout << "Type: Enum\n";
    std::cout << "Name: " << type.name << "\n";
}

std::vector<Annotation> ParseAnnotations(std::string_view comment) {
    std::vector<Annotation> result;
    size_t i = 0;
    
    while((i = comment.find('@', i)) != std::string_view::npos) {
        ++i;
        size_t start = i;
        
        // after @, go while it's an alphanum or underscore
        while(i < comment.size() && IsIdentityChar(comment[i])) {
            ++i;
        }

        Annotation annotation {
            comment.substr(start, i - start), {}
        };

        // This is for future state - collect any arguments from annotation (e.g. "1, 50" in @range(1, 50))
        if(i < comment.size() && comment[i] == '(') {
            size_t close = comment.find(')', i);
            if(close != std::string_view::npos) {
                // substr starts after "(", ends before closing ")"
                annotation.args = comment.substr(i + 1, close - i - 1);
                i = close + 1;
            }
        }
        result.push_back(annotation);
    }

    return result;
}

bool HasAnnotation(std::string_view comment, std::string_view name) {
    for(const Annotation& a : ParseAnnotations(comment)) {
        if(a.name == name) {
            return true;
        }
    }
    return false;
}