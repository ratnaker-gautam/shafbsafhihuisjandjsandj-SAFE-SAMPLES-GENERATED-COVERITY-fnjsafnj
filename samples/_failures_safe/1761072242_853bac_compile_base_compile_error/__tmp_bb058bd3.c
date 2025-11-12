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

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
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
    
    int index = find_word_index(analyzer, word);
    if (index >= 0) {
        analyzer->words[index].count++;
        return;
    }
    
    if (analyzer->word_count >= MAX_WORDS) return;
    
    struct WordStats *ws = &analyzer->words[analyzer->word_count];
    strncpy(ws->word, word, MAX_WORD_LEN - 1);
    ws->word[MAX_WORD_LEN - 1] = '\0';
    ws->count = 1;
    ws->length = length;
    ws->is_capitalized = capitalized;
    
    analyzer->word_count++;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    char buffer[MAX_TEXT_LEN];
    strncpy(buffer, text, MAX_TEXT_LEN - 1);
    buffer[MAX_TEXT_LEN - 1] = '\0';
    
    char word[MAX_WORD_LEN];
    int word_len = 0;
    int in_word = 0;
    
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (is_word_char(buffer[i])) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < MAX_WORD_LEN - 1) {
                word[word_len++] = buffer[i];
            }
        } else {
            if (in_word) {
                word[word_len] = '\0';
                if (word_len > 0) {
                    int capitalized = isupper((unsigned char)word[0]);
                    add_word(analyzer, word, word_len, capitalized);
                    analyzer->total_words++;
                    
                    if (word_len > analyzer->longest_word_length) {
                        analyzer->longest_word_length = word_len;
                    }
                    if (word_len < analyzer->shortest_word_length) {
                        analyzer->shortest_word_length = word_len;
                    }
                }
                in_word = 0;
            }
        }
    }
    
    if (in_word && word_len > 0) {
        word[word_len] = '\0';
        int capitalized = isupper((unsigned char)word[0]);
        add_word(analyzer, word, word_len, capitalized);
        analyzer->total_words++;
        
        if (word_len > analyzer->longest_word_length) {
            analyzer->longest_word_length = word_len;
        }
        if (word_len < analyzer->shortest_word_length) {
            analyzer->shortest_word_length = word_len;
        }
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("'%s': %d occurrences", analyzer->words[i].word, analyzer->words[i].count);
        if (analyzer->words[i].is_capitalized) {
            printf(" (capitalized)");
        }
        printf("\n");
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {