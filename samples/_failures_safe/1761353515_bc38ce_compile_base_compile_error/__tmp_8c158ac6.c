//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define MAX_TEXT_LEN 1000

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
    char processed_text[MAX_TEXT_LEN];
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_characters = 0;
    analyzer->total_words = 0;
    memset(analyzer->processed_text, 0, MAX_TEXT_LEN);
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(analyzer->words[i].word, 0, MAX_WORD_LEN);
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

int is_valid_char(char c) {
    return isalpha(c) || isspace(c) || c == '.' || c == ',' || c == '!' || c == '?';
}

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return 0;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

int add_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= MAX_WORDS) return 0;
    if (word == NULL || strlen(word) == 0) return 0;
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            analyzer->words[i].count++;
            return 1;
        }
    }
    
    if (!safe_strncpy(analyzer->words[analyzer->word_count].word, word, MAX_WORD_LEN)) {
        return 0;
    }
    
    analyzer->words[analyzer->word_count].length = strlen(word);
    analyzer->words[analyzer->word_count].is_capitalized = isupper(word[0]) ? 1 : 0;
    analyzer->words[analyzer->word_count].count = 1;
    analyzer->word_count++;
    return 1;
}

void process_text(struct TextAnalyzer *analyzer, const char *input) {
    if (input == NULL) return;
    
    char buffer[MAX_TEXT_LEN];
    if (!safe_strncpy(buffer, input, MAX_TEXT_LEN)) return;
    
    char *token = strtok(buffer, " \t\n\r.,!?");
    while (token != NULL && analyzer->total_words < MAX_WORDS) {
        if (strlen(token) > 0 && strlen(token) < MAX_WORD_LEN) {
            if (add_word(analyzer, token)) {
                analyzer->total_words++;
                analyzer->total_characters += strlen(token);
            }
        }
        token = strtok(NULL, " \t\n\r.,!?");
    }
    
    safe_strncpy(analyzer->processed_text, input, MAX_TEXT_LEN);
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Total characters (words only): %d\n", analyzer->total_characters);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("'%s': %d occurrences (length: %d, %s)\n",
               analyzer->words[i].word,
               analyzer->words[i].count,
               analyzer->words[i].length,
               analyzer->words[i].is_capitalized ? "capitalized" : "lowercase");
    }
}

int get_user_input(char *buffer, size_t size) {
    if (buffer == NULL || size == 0) return 0;
    
    printf("Enter text to analyze (max %zu characters): ", size - 1);
    
    if (fgets(buffer, size, stdin) == NULL) return 0;
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    for (size_t i = 0; i < len && buffer[i] != '\0'; i++) {
        if (!is_valid_char(buffer[i])) {
            printf("Error: Invalid character detected. Only letters, spaces and basic punctuation allowed.\n");
            return 0;
        }
    }
    
    return 1;
}

int main(void) {
    struct TextAnalyzer analyzer;
    char input_buffer[MAX_TEXT_LEN];
    
    init_analyzer(&analyzer);
    
    printf("Text Analyzer - String Processing Demo\n");
    printf("======================================\n");
    
    if (!get_user_input(input