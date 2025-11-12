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

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

void normalize_word(char *word) {
    int len = strlen(word);
    while (len > 0 && !isalpha((unsigned char)word[len - 1])) {
        word[--len] = '\0';
    }
    
    if (len > 0) {
        for (int i = 0; i < len; i++) {
            word[i] = tolower((unsigned char)word[i]);
        }
    }
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextAnalyzer *analyzer, const char *word, int is_capitalized) {
    if (analyzer->word_count >= MAX_WORDS || strlen(word) == 0) {
        return;
    }
    
    char normalized[MAX_WORD_LEN];
    strncpy(normalized, word, MAX_WORD_LEN - 1);
    normalized[MAX_WORD_LEN - 1] = '\0';
    normalize_word(normalized);
    
    if (strlen(normalized) == 0) {
        return;
    }
    
    int index = find_word_index(analyzer, normalized);
    if (index >= 0) {
        analyzer->words[index].count++;
        if (is_capitalized) {
            analyzer->words[index].is_capitalized = 1;
        }
    } else {
        int new_index = analyzer->word_count;
        strncpy(analyzer->words[new_index].word, normalized, MAX_WORD_LEN - 1);
        analyzer->words[new_index].word[MAX_WORD_LEN - 1] = '\0';
        analyzer->words[new_index].count = 1;
        analyzer->words[new_index].length = strlen(normalized);
        analyzer->words[new_index].is_capitalized = is_capitalized;
        analyzer->word_count++;
        
        if (analyzer->words[new_index].length > analyzer->longest_word_length) {
            analyzer->longest_word_length = analyzer->words[new_index].length;
        }
        if (analyzer->words[new_index].length < analyzer->shortest_word_length) {
            analyzer->shortest_word_length = analyzer->words[new_index].length;
        }
    }
    analyzer->total_words++;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) {
        return;
    }
    
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int in_word = 0;
    int word_start_capitalized = 0;
    
    for (size_t i = 0; text[i] != '\0' && buf_index < MAX_WORD_LEN - 1; i++) {
        if (is_word_char(text[i])) {
            buffer[buf_index++] = text[i];
            if (!in_word && isupper((unsigned char)text[i])) {
                word_start_capitalized = 1;
            }
            in_word = 1;
        } else if (in_word) {
            buffer[buf_index] = '\0';
            add_word(analyzer, buffer, word_start_capitalized);
            buf_index = 0;
            in_word = 0;
            word_start_capitalized = 0;
        }
    }
    
    if (in_word && buf_index > 0) {
        buffer[buf_index] = '\0';
        add_word(analyzer, buffer, word_start_capitalized);
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words processed: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord frequency (appearing more than once):\n");
    
    int printed = 0;
    for (int i = 0; i