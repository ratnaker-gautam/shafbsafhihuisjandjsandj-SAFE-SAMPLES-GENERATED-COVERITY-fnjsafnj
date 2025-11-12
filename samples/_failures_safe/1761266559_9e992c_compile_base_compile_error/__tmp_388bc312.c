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
    int total_chars;
    int total_words;
    int unique_words;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    if (!analyzer) return;
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

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    if (!analyzer || !word) return -1;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int length, int capitalized) {
    if (!analyzer || !word || analyzer->word_count >= MAX_WORDS || length <= 0 || length >= MAX_WORD_LEN) {
        return -1;
    }
    
    int index = find_word_index(analyzer, word);
    if (index >= 0) {
        analyzer->words[index].count++;
        return index;
    }
    
    if (analyzer->word_count < MAX_WORDS) {
        strncpy(analyzer->words[analyzer->word_count].word, word, MAX_WORD_LEN - 1);
        analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = length;
        analyzer->words[analyzer->word_count].is_capitalized = capitalized;
        analyzer->word_count++;
        analyzer->unique_words++;
        return analyzer->word_count - 1;
    }
    
    return -1;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (!analyzer || !text) return;
    
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int in_word = 0;
    int word_start_cap = 0;
    
    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        
        if (is_word_char(c)) {
            if (!in_word) {
                in_word = 1;
                buf_index = 0;
                word_start_cap = isupper((unsigned char)c) ? 1 : 0;
            }
            
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = tolower((unsigned char)c);
            }
        } else {
            if (in_word && buf_index > 0) {
                buffer[buf_index] = '\0';
                add_word(analyzer, buffer, buf_index, word_start_cap);
                analyzer->total_words++;
                analyzer->total_chars += buf_index;
                in_word = 0;
                buf_index = 0;
            }
        }
    }
    
    if (in_word && buf_index > 0) {
        buffer[buf_index] = '\0';
        add_word(analyzer, buffer, buf_index, word_start_cap);
        analyzer->total_words++;
        analyzer->total_chars += buf_index;
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    if (!analyzer) return;
    
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Total characters in words: %d\n", analyzer->total_chars);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("\nWord frequency (appearing more than once):\n");
    
    int found_frequent = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].count > 1) {
            printf("'%s': %d times\n", analyzer->words[i].word, analyzer->words[i].count);
            found_frequent = 1;
        }
    }
    
    if (!found_frequent) {
        printf("No words appear more than once.\n");
    }
    
    printf("\nCapitalized words:\n");
    int found_caps = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].is_capitalized) {
            printf("'%s'\n", analyzer->words[i].word);
            found_caps = 1;
        }
    }
    
    if