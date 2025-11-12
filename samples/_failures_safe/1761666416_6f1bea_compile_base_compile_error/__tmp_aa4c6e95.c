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
    int total_characters;
    int total_words;
    int unique_words;
    int longest_word_length;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_characters = 0;
    analyzer->total_words = 0;
    analyzer->unique_words = 0;
    analyzer->longest_word_length = 0;
    memset(analyzer->words, 0, sizeof(analyzer->words));
}

int is_word_character(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
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
    if (analyzer->word_count >= MAX_WORDS) return;
    
    char normalized[MAX_WORD_LEN];
    strncpy(normalized, word, MAX_WORD_LEN - 1);
    normalized[MAX_WORD_LEN - 1] = '\0';
    to_lower_case(normalized);
    
    int index = find_word_index(analyzer, normalized);
    
    if (index == -1) {
        struct WordStats *ws = &analyzer->words[analyzer->word_count];
        strncpy(ws->word, normalized, MAX_WORD_LEN - 1);
        ws->word[MAX_WORD_LEN - 1] = '\0';
        ws->count = 1;
        ws->length = strlen(normalized);
        ws->is_capitalized = is_capitalized;
        analyzer->word_count++;
        analyzer->unique_words++;
    } else {
        analyzer->words[index].count++;
        if (is_capitalized) {
            analyzer->words[index].is_capitalized = 1;
        }
    }
    
    int len = strlen(word);
    if (len > analyzer->longest_word_length) {
        analyzer->longest_word_length = len;
    }
}

int process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (!text) return 0;
    
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int in_word = 0;
    int word_start_capitalized = 0;
    
    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        analyzer->total_characters++;
        
        if (is_word_character(c)) {
            if (!in_word) {
                word_start_capitalized = isupper((unsigned char)c);
                in_word = 1;
                buf_index = 0;
            }
            
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = c;
            }
        } else {
            if (in_word) {
                buffer[buf_index] = '\0';
                if (buf_index > 0) {
                    add_word(analyzer, buffer, word_start_capitalized);
                    analyzer->total_words++;
                }
                in_word = 0;
                buf_index = 0;
            }
        }
    }
    
    if (in_word && buf_index > 0) {
        buffer[buf_index] = '\0';
        add_word(analyzer, buffer, word_start_capitalized);
        analyzer->total_words++;
    }
    
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total characters: %d\n", analyzer->total_characters);
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("\nWord frequency (appearing more than once):\n");
    
    int printed = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].count > 1) {
            printf("'%s': %d times\n", analyzer->words[i].word, analyzer->words[i].count);
            printed = 1;
        }
    }
    
    if (!printed) {
        printf("(No words appear more than once)\n");
    }
    
    printf("\nCapitalized words:\n");
    printed = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].is_capitalized) {
            printf("'%s'\n", analyzer->words[i].word);
            printed = 1