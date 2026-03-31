#pragma once

#include <string>
#include <expected>

namespace core {

struct ExprError {
    std::string message;
    int position = 0;
};

// Evaluate a simple arithmetic expression (+ - * / parentheses).
// Supports integer and floating-point literals.
std::expected<double, ExprError> eval_expr(const std::string &expr);

} // namespace core
