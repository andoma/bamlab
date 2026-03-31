#include "core/expr.h"

#include <cassert>
#include <cmath>
#include <cstdio>

#define TEST(name) static void name()
#define RUN(name) do { printf("  %-40s", #name); name(); printf("OK\n"); } while(0)

static void assert_near(double a, double b, double eps = 1e-9)
{
    if (std::fabs(a - b) >= eps) {
        fprintf(stderr, "assert_near failed: %f != %f (eps=%g)\n", a, b, eps);
        abort();
    }
}

TEST(test_simple_add)
{
    auto r = core::eval_expr("2 + 3");
    assert(r.has_value());
    assert_near(*r, 5.0);
}

TEST(test_precedence)
{
    auto r = core::eval_expr("2 + 3 * 4");
    assert(r.has_value());
    assert_near(*r, 14.0);
}

TEST(test_parentheses)
{
    auto r = core::eval_expr("(2 + 3) * 4");
    assert(r.has_value());
    assert_near(*r, 20.0);
}

TEST(test_division)
{
    auto r = core::eval_expr("10 / 4");
    assert(r.has_value());
    assert_near(*r, 2.5);
}

TEST(test_division_by_zero)
{
    auto r = core::eval_expr("1 / 0");
    assert(!r.has_value());
}

TEST(test_nested_parens)
{
    auto r = core::eval_expr("((1 + 2) * (3 + 4))");
    assert(r.has_value());
    assert_near(*r, 21.0);
}

TEST(test_floating_point)
{
    auto r = core::eval_expr("3.14 * 2");
    assert(r.has_value());
    assert_near(*r, 6.28);
}

TEST(test_negative)
{
    auto r = core::eval_expr("-5 + 3");
    assert(r.has_value());
    assert_near(*r, -2.0);
}

TEST(test_whitespace)
{
    auto r = core::eval_expr("  1   +   2  ");
    assert(r.has_value());
    assert_near(*r, 3.0);
}

TEST(test_invalid_expr)
{
    auto r = core::eval_expr("abc");
    assert(!r.has_value());
}

TEST(test_trailing_junk)
{
    auto r = core::eval_expr("1 + 2 abc");
    assert(!r.has_value());
}

TEST(test_complex)
{
    auto r = core::eval_expr("(10 - 2) * 3 + 4 / 2");
    assert(r.has_value());
    assert_near(*r, 26.0);
}

int main()
{
    printf("test_expr:\n");
    RUN(test_simple_add);
    RUN(test_precedence);
    RUN(test_parentheses);
    RUN(test_division);
    RUN(test_division_by_zero);
    RUN(test_nested_parens);
    RUN(test_floating_point);
    RUN(test_negative);
    RUN(test_whitespace);
    RUN(test_invalid_expr);
    RUN(test_trailing_junk);
    RUN(test_complex);
    printf("All tests passed.\n");
    return 0;
}
