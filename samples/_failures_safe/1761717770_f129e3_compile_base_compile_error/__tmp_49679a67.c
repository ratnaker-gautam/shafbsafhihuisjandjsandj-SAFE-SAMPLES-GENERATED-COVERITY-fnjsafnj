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
    while (len > 0 && !isalpha((unsigned char)word[0])) {
        memmove(word, word + 1, len);
        len--;
    }
}

int add_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= MAX_WORDS) return 0;
    
    char clean[MAX_WORD_LEN];
    strncpy(clean, word, MAX_WORD_LEN - 1);
    clean[MAX_WORD_LEN - 1] = '\0';
    clean_word(clean);
    
    int len = strlen(clean);
    if (len == 0) return 0;
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, clean) == 0) {
            analyzer->words[i].count++;
            analyzer->total_words++;
            return 1;
        }
    }
    
    if (analyzer->word_count < MAX_WORDS) {
        strncpy(analyzer->words[analyzer->word_count].word, clean, MAX_WORD_LEN - 1);
        analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = len;
        analyzer->words[analyzer->word_count].is_capitalized = isupper((unsigned char)clean[0]);
        
        if (len > analyzer->longest_word_length) {
            analyzer->longest_word_length = len;
        }
        if (len < analyzer->shortest_word_length) {
            analyzer->shortest_word_length = len;
        }
        
        analyzer->word_count++;
        analyzer->total_words++;
        return 1;
    }
    
    return 0;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    char buffer[1024];
    strncpy(buffer, text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char *token = strtok(buffer, " \t\n\r.,;:!?\"()[]{}");
    while (token != NULL) {
        if (strlen(token) > 0 && strlen(token) < MAX_WORD_LEN) {
            add_word(analyzer, token);
        }
        token = strtok(NULL, " \t\n\r.,;:!?\"()[]{}");
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total unique words: %d\n", analyzer->word_count);
    printf("Total word occurrences: %d\n", analyzer->total_words);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("%-20s: %d times\n", analyzer->words[i].word, analyzer->words[i].count);
    }
    
    int capitalized_count = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].is_capitalized) {
            capitalized_count++;
        }
    }
    printf("\nCapitalized words: %d\n", capitalized_count);
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input[2048];
    printf("Enter text to analyze (max 2047 characters):\n");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1]