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
    int unique_words;
    int longest_word_length;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->unique_words = 0;
    analyzer->longest_word_length = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
        memset(analyzer->words[i].word, 0, MAX_WORD_LEN);
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

void to_lower_case(char *str) {
    if (str == NULL) return;
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer == NULL || word == NULL) return -1;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int is_capitalized) {
    if (analyzer == NULL || word == NULL) return 0;
    if (analyzer->word_count >= MAX_WORDS) return 0;
    
    char normalized[MAX_WORD_LEN];
    strncpy(normalized, word, MAX_WORD_LEN - 1);
    normalized[MAX_WORD_LEN - 1] = '\0';
    to_lower_case(normalized);
    
    int index = find_word_index(analyzer, normalized);
    if (index != -1) {
        analyzer->words[index].count++;
        if (is_capitalized) {
            analyzer->words[index].is_capitalized = 1;
        }
        return 1;
    }
    
    if (strlen(normalized) >= MAX_WORD_LEN) return 0;
    
    strcpy(analyzer->words[analyzer->word_count].word, normalized);
    analyzer->words[analyzer->word_count].count = 1;
    analyzer->words[analyzer->word_count].length = strlen(normalized);
    analyzer->words[analyzer->word_count].is_capitalized = is_capitalized;
    
    if (analyzer->words[analyzer->word_count].length > analyzer->longest_word_length) {
        analyzer->longest_word_length = analyzer->words[analyzer->word_count].length;
    }
    
    analyzer->word_count++;
    analyzer->unique_words++;
    return 1;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (analyzer == NULL || text == NULL) return;
    
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int in_word = 0;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (is_word_char(text[i])) {
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = text[i];
            }
            in_word = 1;
        } else {
            if (in_word && buf_index > 0) {
                buffer[buf_index] = '\0';
                int is_cap = isupper((unsigned char)buffer[0]) ? 1 : 0;
                add_word(analyzer, buffer, is_cap);
                analyzer->total_words++;
                buf_index = 0;
            }
            in_word = 0;
        }
    }
    
    if (in_word && buf_index > 0) {
        buffer[buf_index] = '\0';
        int is_cap = isupper((unsigned char)buffer[0]) ? 1 : 0;
        add_word(analyzer, buffer, is_cap);
        analyzer->total_words++;
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("\nWord frequency (appearing 2+ times):\n");
    
    int printed = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].count >= 2) {
            printf("'%s': %d times", analyzer->words[i].word, analyzer->words[i].count);
            if (analyzer