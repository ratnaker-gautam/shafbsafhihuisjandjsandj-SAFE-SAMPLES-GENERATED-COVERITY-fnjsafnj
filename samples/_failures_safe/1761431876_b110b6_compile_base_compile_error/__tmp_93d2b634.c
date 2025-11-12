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

void clean_word(char *word) {
    int len = strlen(word);
    while (len > 0 && !isalpha((unsigned char)word[len - 1])) {
        word[--len] = '\0';
    }
    while (*word && !isalpha((unsigned char)*word)) {
        memmove(word, word + 1, strlen(word));
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

void add_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= MAX_WORDS) return;
    
    char clean[MAX_WORD_LEN];
    strncpy(clean, word, MAX_WORD_LEN - 1);
    clean[MAX_WORD_LEN - 1] = '\0';
    clean_word(clean);
    
    if (strlen(clean) == 0) return;
    
    int index = find_word_index(analyzer, clean);
    if (index >= 0) {
        analyzer->words[index].count++;
    } else {
        int new_index = analyzer->word_count++;
        strncpy(analyzer->words[new_index].word, clean, MAX_WORD_LEN - 1);
        analyzer->words[new_index].word[MAX_WORD_LEN - 1] = '\0';
        analyzer->words[new_index].count = 1;
        analyzer->words[new_index].length = strlen(clean);
        analyzer->words[new_index].is_capitalized = isupper((unsigned char)clean[0]) ? 1 : 0;
        
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
    char buffer[1024];
    strncpy(buffer, text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char *token = strtok(buffer, " \t\n\r.,;:!?\"");
    while (token != NULL) {
        if (strlen(token) > 0 && strlen(token) < MAX_WORD_LEN) {
            add_word(analyzer, token);
        }
        token = strtok(NULL, " \t\n\r.,;:!?\"");
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord frequency (top 10):\n");
    
    for (int i = 0; i < (analyzer->word_count < 10 ? analyzer->word_count : 10); i++) {
        for (int j = i + 1; j < analyzer->word_count; j++) {
            if (analyzer->words[j].count > analyzer->words[i].count) {
                struct WordStats temp = analyzer->words[i];
                analyzer->words[i] = analyzer->words[j];
                analyzer->words[j] = temp;
            }
        }
    }
    
    for (int i = 0; i < (analyzer->word_count < 10 ? analyzer->word_count : 10); i++) {
        printf("%d. '%s' - %d occurrences\n", i + 1, analyzer->words[i].word, analyzer->words[i].count);
    }
    
    int capitalized_count = 0;
    for (int i = 0; i < analyzer->word_count; i++) {