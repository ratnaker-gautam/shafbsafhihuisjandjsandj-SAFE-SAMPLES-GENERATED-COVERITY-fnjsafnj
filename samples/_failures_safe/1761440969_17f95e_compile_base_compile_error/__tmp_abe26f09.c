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
        memset(analyzer->words[i].word, 0, MAX_WORD_LEN);
    }
}

int is_word_character(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

void normalize_word(char *dest, const char *src, size_t len) {
    if (len >= MAX_WORD_LEN) len = MAX_WORD_LEN - 1;
    for (size_t i = 0; i < len; i++) {
        dest[i] = tolower((unsigned char)src[i]);
    }
    dest[len] = '\0';
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextAnalyzer *analyzer, const char *word, int length, int capitalized) {
    if (length <= 0 || length >= MAX_WORD_LEN) return;
    
    char normalized[MAX_WORD_LEN];
    normalize_word(normalized, word, length);
    
    int index = find_word_index(analyzer, normalized);
    if (index >= 0) {
        analyzer->words[index].count++;
        if (capitalized && !analyzer->words[index].is_capitalized) {
            analyzer->words[index].is_capitalized = 1;
        }
    } else if (analyzer->word_count < MAX_WORDS) {
        struct WordStats *ws = &analyzer->words[analyzer->word_count];
        strncpy(ws->word, normalized, MAX_WORD_LEN - 1);
        ws->word[MAX_WORD_LEN - 1] = '\0';
        ws->count = 1;
        ws->length = length;
        ws->is_capitalized = capitalized;
        analyzer->word_count++;
    }
    
    analyzer->total_words++;
    if (length > analyzer->longest_word_length) {
        analyzer->longest_word_length = length;
    }
    if (length < analyzer->shortest_word_length) {
        analyzer->shortest_word_length = length;
    }
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (!text) return;
    
    size_t len = strlen(text);
    if (len == 0) return;
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    int word_start_capitalized = 0;
    
    for (size_t i = 0; i < len; i++) {
        char c = text[i];
        
        if (is_word_character(c)) {
            if (!in_word) {
                in_word = 1;
                buf_pos = 0;
                word_start_capitalized = isupper((unsigned char)c);
            }
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = c;
            }
        } else {
            if (in_word && buf_pos > 0) {
                buffer[buf_pos] = '\0';
                add_word(analyzer, buffer, buf_pos, word_start_capitalized);
                in_word = 0;
            }
        }
    }
    
    if (in_word && buf_pos > 0) {
        buffer[buf_pos] = '\0';
        add_word(analyzer, buffer, buf_pos, word_start_capitalized);
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord frequency (appearing more than once):\n");
    
    int found_frequent = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].count > 1) {
            printf("'%s': %d times", analyzer->words[i].word, analyzer->words[i].count);
            if (analyzer->words[i].is_capitalized) {
                printf(" (capitalized)");
            }
            printf("\