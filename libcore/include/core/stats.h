#pragma once

#include <string>
#include <vector>
#include <map>

namespace core {

struct TextStats {
    int lines = 0;
    int words = 0;
    int chars = 0;
    std::map<std::string, int> word_freq;
};

TextStats analyze_text(const std::string &text);

std::vector<std::string> split_words(const std::string &text);

std::string to_lower(const std::string &s);

} // namespace core
