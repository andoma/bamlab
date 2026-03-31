#include "core/expr.h"

#include <cstdio>
#include <iostream>
#include <string>

// Main for calculator
int main(int argc, char *argv[])
{
    if (argc > 1) {
        // Evaluate arguments as expressions
        for (int i = 1; i < argc; i++) {
            auto result = core::eval_expr(argv[i]);
            if (result)
                printf("%s = %g\n", argv[i], *result);
            else
                fprintf(stderr, "error: %s (at position %d)\n",
                        result.error().message.c_str(),
                        result.error().position);
        }
        return 0;
    }

    // Interactive REPL
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty() || line == "quit" || line == "exit")
            break;

        auto result = core::eval_expr(line);
        if (result)
            printf("= %g\n", *result);
        else
            fprintf(stderr, "error: %s (at position %d)\n",
                    result.error().message.c_str(),
                    result.error().position);
    }

    return 0;
}
