#pragma once

#include "ast.hpp"
#include "types.hpp"
#include "utils/result.hpp"

#include <fstream>
#include <variant>


namespace parser
{
using numberVariant = std::variant<int8_t, int16_t, int32_t, int64_t, float, double, long double>;

class Lexer
{
public:
    struct TokenData
    {
    };

private:
    std::ifstream filestream;
    std::string identifier;
    numberVariant number;
    Type type;
    ast::Operator op;
    std::ifstream::pos_type prevTokPos;

public:
    Lexer(const char* path);

    utils::Result<TokenData, std::string> NextToken();
    ast::Operator GetOperator();
    std::string GetIdentifier();
    Type GetType();
    numberVariant GetNumber();


private:
    void SkipWhitespace();
    bool NextIdentifier();
    bool NextNumber();
    /* bool NextOperator(); */
};

} // namespace parser
