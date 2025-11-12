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
    int longest_word_length;
    int shortest_word_length;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;
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
        char c = word[i];
        if (isalpha((unsigned char)c)) {
            normalized[norm_len++] = tolower((unsigned char)c);
        } else if (c == '\'' || c == '-') {
            normalized[norm_len++] = c;
        }
    }
    normalized[norm_len] = '\0';

    if (norm_len == 0) {
        return;
    }

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
    if (text == NULL) {
        return;
    }

    int len = strlen(text);
    if (len == 0) {
        return;
    }

    int word_start = -1;
    for (int i = 0; i <= len; i++) {
        char c = text[i];
        if (is_word_char(c) && word_start == -1) {
            word_start = i;
        } else if ((!is_word_char(c) || c == '\0') && word_start != -1) {
            int word_length = i - word_start;
            if (word_length > 0 && word_length < MAX_WORD_LEN) {
                char word[MAX_WORD_LEN];
                strncpy(word, &text[word_start], word_length);
                word[word_length] = '\0';
                add_word(analyzer, word, word_length);
                analyzer->total_words++;
                
                if (word_length > analyzer->longest_word_length) {
                    analyzer->longest_word_length = word_length;
                }
                if (word_length < analyzer->shortest_word_length) {
                    analyzer->shortest_word_length = word_length;
                }
            }
            word_start = -1;
        }
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    if (analyzer->total_words == 0) {
        printf("No words found in text.\n");
        return;
    }

    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord frequency:\n");

    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].count > 0) {
            printf("%s: %d", analyzer->words[i].word, analyzer->words[i].count);
            if (analyzer->words[i].is_capitalized) {
                printf(" (capitalized)");
            }
            printf("\n");
        }
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);

    char input_buffer[4096];
    printf("Enter text to analyze (max 4095 characters):\n");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin