//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: string_processing
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
    int total_chars;
    int total_words;
    int unique_words;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
    analyzer->total_words = 0;
    analyzer->unique_words = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

void normalize_word(char *dest, const char *src, size_t max_len) {
    size_t i = 0;
    size_t j = 0;
    
    while (src[i] != '\0' && j < max_len - 1) {
        if (isalpha((unsigned char)src[i])) {
            dest[j] = tolower((unsigned char)src[i]);
            j++;
        } else if (src[i] == '\'' || src[i] == '-') {
            dest[j] = src[i];
            j++;
        }
        i++;
    }
    dest[j] = '\0';
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextAnalyzer *analyzer, const char *raw_word, int is_capitalized) {
    if (strlen(raw_word) == 0) return;
    
    char normalized[MAX_WORD_LEN];
    normalize_word(normalized, raw_word, MAX_WORD_LEN);
    
    if (strlen(normalized) == 0) return;
    
    int index = find_word_index(analyzer, normalized);
    
    if (index >= 0) {
        analyzer->words[index].count++;
        if (is_capitalized) {
            analyzer->words[index].is_capitalized = 1;
        }
    } else {
        if (analyzer->word_count < MAX_WORDS) {
            strncpy(analyzer->words[analyzer->word_count].word, normalized, MAX_WORD_LEN - 1);
            analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
            analyzer->words[analyzer->word_count].count = 1;
            analyzer->words[analyzer->word_count].length = strlen(normalized);
            analyzer->words[analyzer->word_count].is_capitalized = is_capitalized;
            analyzer->word_count++;
            analyzer->unique_words++;
        }
    }
    analyzer->total_words++;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    char buffer[MAX_TEXT_LEN];
    strncpy(buffer, text, MAX_TEXT_LEN - 1);
    buffer[MAX_TEXT_LEN - 1] = '\0';
    
    analyzer->total_chars = strlen(buffer);
    
    int i = 0;
    char current_word[MAX_WORD_LEN];
    int word_pos = 0;
    int in_word = 0;
    
    while (buffer[i] != '\0' && word_pos < MAX_WORD_LEN - 1) {
        if (is_word_char(buffer[i])) {
            if (!in_word) {
                in_word = 1;
                word_pos = 0;
            }
            current_word[word_pos] = buffer[i];
            word_pos++;
        } else {
            if (in_word) {
                current_word[word_pos] = '\0';
                int is_cap = isupper((unsigned char)current_word[0]);
                add_word(analyzer, current_word, is_cap);
                in_word = 0;
                word_pos = 0;
            }
        }
        i++;
    }
    
    if (in_word && word_pos > 0) {
        current_word[word_pos] = '\0';
        int is_cap = isupper((unsigned char)current_word[0]);
        add_word(analyzer, current_word, is_cap);
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("\nWord Frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("%-20s: count=%d, length=%d", 
               analyzer->words[i].word, 
               analyzer->words[i].