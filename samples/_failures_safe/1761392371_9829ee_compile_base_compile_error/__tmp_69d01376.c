//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000
#define MAX_TEXT_LEN 10000

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_chars;
    int total_words;
    char processed_text[MAX_TEXT_LEN];
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
    analyzer->total_words = 0;
    memset(analyzer->processed_text, 0, MAX_TEXT_LEN);
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(analyzer->words[i].word, 0, MAX_WORD_LEN);
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int len, int capitalized) {
    if (len <= 0 || len >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
        return 0;
    }

    char temp[MAX_WORD_LEN];
    if (len >= MAX_WORD_LEN) {
        return 0;
    }
    strncpy(temp, word, len);
    temp[len] = '\0';

    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, temp) == 0) {
            analyzer->words[i].count++;
            return 1;
        }
    }

    if (analyzer->word_count < MAX_WORDS) {
        strncpy(analyzer->words[analyzer->word_count].word, temp, MAX_WORD_LEN - 1);
        analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = len;
        analyzer->words[analyzer->word_count].is_capitalized = capitalized;
        analyzer->word_count++;
        return 1;
    }
    return 0;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) {
        return;
    }

    size_t text_len = strlen(text);
    if (text_len == 0 || text_len >= MAX_TEXT_LEN) {
        return;
    }

    strncpy(analyzer->processed_text, text, MAX_TEXT_LEN - 1);
    analyzer->processed_text[MAX_TEXT_LEN - 1] = '\0';

    int in_word = 0;
    int word_start = 0;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;

    for (size_t i = 0; i <= text_len; i++) {
        char c = text[i];
        if (is_word_char(c) && !in_word) {
            in_word = 1;
            word_start = i;
            word_len = 0;
        }

        if (in_word) {
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = c;
            }
        }

        if ((!is_word_char(c) || i == text_len) && in_word) {
            current_word[word_len] = '\0';
            int capitalized = isupper((unsigned char)current_word[0]) ? 1 : 0;
            if (add_word(analyzer, current_word, word_len, capitalized)) {
                analyzer->total_words++;
                analyzer->total_chars += word_len;
            }
            in_word = 0;
            word_len = 0;
        }
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Total characters in words: %d\n", analyzer->total_chars);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("\nWord frequency:\n");

    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].count > 0) {
            printf("'%s': %d occurrences", analyzer->words[i].word, analyzer->words[i].count);
            if (analyzer->words[i].is_capitalized) {
                printf(" (capitalized)");
            }
            printf("\n");
        }
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);

    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, MAX_TEXT_LEN, stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }

    size_t len = strlen(input_text);
    if (len > 0 &&