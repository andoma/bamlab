#include "core/stats.h"

#include <algorithm>
#include <cctype>
#include <sstream>

namespace core {

std::string
to_lower(const std::string &s)
{
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::vector<std::string>
split_words(const std::string &text)
{
    std::vector<std::string> words;
    std::string word;

    for (char c : text) {
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '\'') {
            word += c;
        } else if (!word.empty()) {
            words.push_back(word);
            word.clear();
        }
    }
    if (!word.empty())
        words.push_back(word);
    return words;
}

TextStats
analyze_text(const std::string &text)
{
    TextStats stats;
    stats.chars = static_cast<int>(text.size());
    stats.lines = text.empty() ? 0 : 1;

    for (char c : text) {
        if (c == '\n')
            stats.lines++;
    }

    auto words = split_words(text);
    stats.words = static_cast<int>(words.size());

    for (auto &w : words)
        stats.word_freq[to_lower(w)]++;

    return stats;
}

} // namespace core
