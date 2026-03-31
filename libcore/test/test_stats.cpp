#include "core/stats.h"

#include <cassert>
#include <cstdio>

#define TEST(name) static void name()
#define RUN(name) do { printf("  %-40s", #name); name(); printf("OK\n"); } while(0)

TEST(test_to_lower)
{
    assert(core::to_lower("Hello World") == "hello world");
    assert(core::to_lower("ABC123") == "abc123");
    assert(core::to_lower("") == "");
}

TEST(test_split_words)
{
    auto words = core::split_words("Hello, world! This is a test.");
    assert(words.size() == 6);
    assert(words[0] == "Hello");
    assert(words[1] == "world");
    assert(words[5] == "test");
}

TEST(test_split_words_empty)
{
    auto words = core::split_words("");
    assert(words.empty());
}

TEST(test_split_words_contractions)
{
    auto words = core::split_words("it's a don't");
    assert(words.size() == 3);
    assert(words[0] == "it's");
    assert(words[2] == "don't");
}

TEST(test_analyze_text)
{
    auto stats = core::analyze_text("Hello world\nHello again\n");
    assert(stats.lines == 3);
    assert(stats.words == 4);
    assert(stats.word_freq["hello"] == 2);
    assert(stats.word_freq["world"] == 1);
    assert(stats.word_freq["again"] == 1);
}

TEST(test_analyze_empty)
{
    auto stats = core::analyze_text("");
    assert(stats.lines == 0);
    assert(stats.words == 0);
    assert(stats.chars == 0);
}

TEST(test_analyze_single_line)
{
    auto stats = core::analyze_text("one two three");
    assert(stats.lines == 1);
    assert(stats.words == 3);
}

int main()
{
    printf("test_stats:\n");
    RUN(test_to_lower);
    RUN(test_split_words);
    RUN(test_split_words_empty);
    RUN(test_split_words_contractions);
    RUN(test_analyze_text);
    RUN(test_analyze_empty);
    RUN(test_analyze_single_line);
    printf("All tests passed.\n");
    return 0;
}
