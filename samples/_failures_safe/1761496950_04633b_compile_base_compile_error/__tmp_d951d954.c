//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100
#define MAX_TEXT_LEN 1024

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    int longest_word_length;
    int shortest_word_length;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

int is_valid_char(char c) {
    return isalpha(c) || c == ' ' || c == '\t' || c == '\n' || c == '\0';
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int length) {
    if (length <= 0 || length >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
        return 0;
    }

    for (int i = 0; i < analyzer->word_count; i++) {
        if (strncmp(analyzer->words[i].word, word, length) == 0 && 
            analyzer->words[i].word[length] == '\0') {
            analyzer->words[i].count++;
            return 1;
        }
    }

    if (analyzer->word_count < MAX_WORDS) {
        strncpy(analyzer->words[analyzer->word_count].word, word, length);
        analyzer->words[analyzer->word_count].word[length] = '\0';
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = length;
        analyzer->words[analyzer->word_count].is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
        analyzer->word_count++;
        return 1;
    }
    return 0;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    char buffer[MAX_TEXT_LEN];
    size_t buf_len = strlen(text);
    
    if (buf_len >= MAX_TEXT_LEN - 1) {
        buf_len = MAX_TEXT_LEN - 1;
    }
    
    strncpy(buffer, text, buf_len);
    buffer[buf_len] = '\0';

    char *ptr = buffer;
    char word[MAX_WORD_LEN];
    int word_len = 0;

    while (*ptr != '\0') {
        if (!is_valid_char(*ptr)) {
            ptr++;
            continue;
        }

        if (isalpha((unsigned char)*ptr)) {
            if (word_len < MAX_WORD_LEN - 1) {
                word[word_len++] = *ptr;
            }
        } else if (word_len > 0) {
            word[word_len] = '\0';
            if (add_word(analyzer, word, word_len)) {
                analyzer->total_words++;
                if (word_len > analyzer->longest_word_length) {
                    analyzer->longest_word_length = word_len;
                }
                if (word_len < analyzer->shortest_word_length) {
                    analyzer->shortest_word_length = word_len;
                }
            }
            word_len = 0;
        }
        ptr++;
    }

    if (word_len > 0) {
        word[word_len] = '\0';
        if (add_word(analyzer, word, word_len)) {
            analyzer->total_words++;
            if (word_len > analyzer->longest_word_length) {
                analyzer->longest_word_length = word_len;
            }
            if (word_len < analyzer->shortest_word_length) {
                analyzer->shortest_word_length = word_len;
            }
        }
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("'%s': %d occurrence(s), length: %d, %s\n",
               analyzer->words[i].word,
               analyzer->words[i].count,
               analyzer->words[i].length,
               analyzer->words[i].is_capitalized ? "capitalized" : "lowercase");
    }
}

int main() {
    struct TextAnalyzer analyzer;
    char input_text[MAX_TEXT_LEN];
    
    init_analyzer(&analyzer);
    
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {