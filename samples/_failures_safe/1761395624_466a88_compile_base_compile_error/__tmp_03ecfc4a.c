//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000

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
    int total_chars;
    int longest_word_length;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->total_chars = 0;
    analyzer->longest_word_length = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
        analyzer->words[i].word[0] = '\0';
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

void add_word(struct TextAnalyzer *analyzer, const char *word, int length) {
    if (length <= 0 || length >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
        return;
    }

    char normalized[MAX_WORD_LEN];
    int norm_len = 0;
    int capitalized = isupper((unsigned char)word[0]);

    for (int i = 0; i < length && norm_len < MAX_WORD_LEN - 1; i++) {
        normalized[norm_len++] = tolower((unsigned char)word[i]);
    }
    normalized[norm_len] = '\0';

    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, normalized) == 0) {
            analyzer->words[i].count++;
            return;
        }
    }

    if (analyzer->word_count < MAX_WORDS) {
        strncpy(analyzer->words[analyzer->word_count].word, normalized, MAX_WORD_LEN - 1);
        analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = norm_len;
        analyzer->words[analyzer->word_count].is_capitalized = capitalized;
        analyzer->word_count++;
    }
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;

    int len = strlen(text);
    if (len <= 0) return;

    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;

    for (int i = 0; i < len; i++) {
        if (is_word_char(text[i])) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = text[i];
            }
            in_word = 1;
        } else {
            if (in_word && buf_pos > 0) {
                buffer[buf_pos] = '\0';
                add_word(analyzer, buffer, buf_pos);
                analyzer->total_words++;
                analyzer->total_chars += buf_pos;
                if (buf_pos > analyzer->longest_word_length) {
                    analyzer->longest_word_length = buf_pos;
                }
                buf_pos = 0;
            }
            in_word = 0;
        }
    }

    if (in_word && buf_pos > 0) {
        buffer[buf_pos] = '\0';
        add_word(analyzer, buffer, buf_pos);
        analyzer->total_words++;
        analyzer->total_chars += buf_pos;
        if (buf_pos > analyzer->longest_word_length) {
            analyzer->longest_word_length = buf_pos;
        }
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Total characters in words: %d\n", analyzer->total_chars);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("\nWord frequency:\n");

    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].count > 0) {
            printf("%s: %d\n", analyzer->words[i].word, analyzer->words[i].count);
        }
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);

    char input[4096];
    printf("Enter text to analyze (max 4095 characters):\n");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }

    if (strlen(input) == 0) {
        printf("No input provided.\n");