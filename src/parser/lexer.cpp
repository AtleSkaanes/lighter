#include "lexer.hpp"

#include "ast.hpp"
#include "utils/result.hpp"

#include <cctype>
#include <cstdio>
#include <fstream>
#include <string>

using namespace parser;

/// Helper function to cast `int` return type to `char` when using peek function
[[clang::always_inline]]
static inline char peek(std::basic_ifstream<char>& stream)
{
    return static_cast<char>(stream.peek());
}
/// Helper function to cast `int` return type to `char` when using get function
[[clang::always_inline]]
static inline char get(std::basic_ifstream<char>& stream)
{
    return static_cast<char>(stream.get());
}

Lexer::Lexer(const char* path)
        : filestream{std::ifstream{path}}, identifier{std::string{""}}, op{ast::Operator::Add},
          prevTokPos(filestream.tellg())
{
}

void Lexer::SkipWhitespace()
{
    prevTokPos = filestream.tellg();
    while (isspace(peek(filestream)))
        get(filestream);
}

// Needs to return Error
bool Lexer::NextIdentifier()
{
    SkipWhitespace();
    char nextChar = peek(filestream);

    // If the next identifier is a special character
    if (!isalpha(nextChar) || nextChar == '_')
        return false;

    identifier = get(filestream);
    nextChar = peek(filestream);
    while (isalnum(nextChar) || nextChar == '_')
    {
        identifier += get(filestream);
        nextChar = peek(filestream);
    }

    return true;
}

// Needs to return error
bool Lexer::NextNumber()
{
    using namespace types;
    const auto IsValidNumberChar = [](const char c) -> bool { return isdigit(c) || c == '.'; };

    if (!IsValidNumberChar(peek(filestream)))
        return false;

    std::string numberStr{};
    numberStr = get(filestream);

    type = numberStr == "." ? Type::Flow : Type::Rune;
    while (IsValidNumberChar(peek(filestream)))
    {
        const char currChar = get(filestream);
        // First `.` character found
        if (currChar == '.' && type == Type::Rune)
            type = Type::Flow;

        // Return error: too many `.` characters in number
        else [[unlikely]] if (currChar == '.' && type == Type::Flow)
            return false;


        numberStr += currChar;
    }

    // Return error: Invalid symbol in number
    if (!isspace(peek(filestream)))
        return false;

    // If number begins with a `.` insert a 0 in front
    if (numberStr.front() == '.')
        numberStr.insert(numberStr.begin(), 1, '0');

    // If number ends with a `.` insert a 0 in the back
    if (numberStr.back() == '.')
        numberStr += '0';

    // Note: Technically this means that `.` is a valid number
    // since `.` => `0.0`
    // I'm keeping it

    // Add more type detection in the future (With specific types like R8 and F64)
    if (type == Type::Rune)
        number = stoi(numberStr);
    else if (type == Type::Flow)
        number = stof(numberStr);

    return true;
}

// Needs to return error
/* bool Lexer::NextOperator() */
/* { */
/*     SkipWhitespace(); */
/*     char nextChar = peek(filestream); */
/* } */

// Needs to return error
utils::Result<Lexer::TokenData, std::string> Lexer::NextToken()
{
    SkipWhitespace();
    /**/
    /* // Identifiers like keywords and variable / function names */
    /* if (NextIdentifier()) */
    /* { */
    /*     if (identifier == "cast") */
    /*         return TokenType::CallFunc; */
    /**/
    /*     if (identifier == "create") */
    /*     { */
    /*         if (!NextIdentifier()) */
    /*             std::cerr << "Invalid character after 'create' keyword: \"" << filestream.peek()
     */
    /*                       << '\"'; */
    /**/
    /*         if (identifier == "spell") */
    /*             return TokenType::CreateFunc; */
    /**/
    /*         // We want a seperate call to get the Identifier */
    /*         // hence we roll back the filestream to the `create` keyword */
    /*         filestream.seekg(prevTokPos); */
    /**/
    /*         return TokenType::CreateVar; */
    /*     } */
    /**/
    /*     return TokenType::Identifier; */
    /* } */
    /**/
    /* // Integers and floats */
    /* if (NextNumber()) */
    /*     return TokenType::Constant; */
}

ast::Operator Lexer::GetOperator() {}
std::string Lexer::GetIdentifier() {}
