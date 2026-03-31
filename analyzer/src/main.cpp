#include "core/stats.h"

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char *argv[])
{
    bool show_freq = false;
    int top_n = 10;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--freq")
            show_freq = true;
        else if (arg == "--top" && i + 1 < argc)
            top_n = std::stoi(argv[++i]);
        else if (arg == "--help") {
            printf("Usage: analyzer [--freq] [--top N]\n"
                   "Reads stdin and prints text statistics.\n");
            return 0;
        }
    }

    // Read all of stdin
    std::ostringstream buf;
    buf << std::cin.rdbuf();
    std::string text = buf.str();

    auto stats = core::analyze_text(text);

    printf("Lines:      %d\n", stats.lines);
    printf("Words:      %d\n", stats.words);
    printf("Characters: %d\n", stats.chars);
    printf("Unique:     %d\n", static_cast<int>(stats.word_freq.size()));

    if (show_freq && !stats.word_freq.empty()) {
        // Sort by frequency descending
        std::vector<std::pair<std::string, int>> sorted(
            stats.word_freq.begin(), stats.word_freq.end());
        std::sort(sorted.begin(), sorted.end(),
                  [](const auto &a, const auto &b) {
                      return a.second > b.second;
                  });

        printf("\nTop %d words:\n", top_n);
        int count = 0;
        for (auto &[word, freq] : sorted) {
            printf("  %4d  %s\n", freq, word.c_str());
            if (++count >= top_n)
                break;
        }
    }

    return 0;
}
