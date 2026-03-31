#include "core/expr.h"

#include <cctype>
#include <cstdlib>

namespace core {

namespace {

struct Parser {
    const char *input;
    int pos = 0;

    char peek() const { return input[pos]; }
    char advance() { return input[pos++]; }

    void skip_ws()
    {
        while (std::isspace(static_cast<unsigned char>(peek())))
            advance();
    }

    std::expected<double, ExprError> parse_number()
    {
        skip_ws();
        int start = pos;
        bool has_dot = false;

        if (peek() == '-' || peek() == '+')
            advance();

        if (!std::isdigit(static_cast<unsigned char>(peek())) && peek() != '.')
            return std::unexpected(ExprError{"expected number", pos});

        while (std::isdigit(static_cast<unsigned char>(peek())))
            advance();

        if (peek() == '.') {
            has_dot = true;
            advance();
            while (std::isdigit(static_cast<unsigned char>(peek())))
                advance();
        }

        (void)has_dot;
        std::string num_str(input + start, input + pos);
        char *end = nullptr;
        double val = std::strtod(num_str.c_str(), &end);
        if (end == num_str.c_str())
            return std::unexpected(ExprError{"invalid number", start});
        return val;
    }

    std::expected<double, ExprError> parse_primary()
    {
        skip_ws();
        if (peek() == '(') {
            advance();
            auto val = parse_additive();
            if (!val)
                return val;
            skip_ws();
            if (peek() != ')')
                return std::unexpected(ExprError{"expected ')'", pos});
            advance();
            return val;
        }
        return parse_number();
    }

    std::expected<double, ExprError> parse_multiplicative()
    {
        auto left = parse_primary();
        if (!left)
            return left;

        for (;;) {
            skip_ws();
            char op = peek();
            if (op != '*' && op != '/')
                break;
            advance();
            auto right = parse_primary();
            if (!right)
                return right;
            if (op == '*')
                *left *= *right;
            else {
                if (*right == 0)
                    return std::unexpected(ExprError{"division by zero", pos});
                *left /= *right;
            }
        }
        return left;
    }

    std::expected<double, ExprError> parse_additive()
    {
        auto left = parse_multiplicative();
        if (!left)
            return left;

        for (;;) {
            skip_ws();
            char op = peek();
            if (op != '+' && op != '-')
                break;
            advance();
            auto right = parse_multiplicative();
            if (!right)
                return right;
            if (op == '+')
                *left += *righty;
            else
                *left -= *right;
        }
        return left;
    }
};

} // anonymous namespace

std::expected<double, ExprError>
eval_expr(const std::string &expr)
{
    Parser p{expr.c_str()};
    auto result = p.parse_additive();
    if (!result)
        return result;
    p.skip_ws();
    if (p.peek() != '\0')
        return std::unexpected(
            ExprError{"unexpected character after expression", p.pos});
    return result;
}

} // namespace core
