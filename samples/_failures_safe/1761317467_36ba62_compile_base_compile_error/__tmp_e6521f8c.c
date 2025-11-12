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
    int total_chars;
    int total_words;
    char processed_text[MAX_TEXT_LEN];
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
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
    return isalpha(c) || c == ' ' || c == '\0';
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

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int process_text(struct TextAnalyzer *analyzer, const char *input) {
    if (analyzer == NULL || input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len >= MAX_TEXT_LEN - 1) return 0;
    
    for (size_t i = 0; i < input_len; i++) {
        if (!is_valid_char(input[i])) return 0;
    }
    
    if (!safe_strncpy(analyzer->processed_text, input, MAX_TEXT_LEN)) return 0;
    
    char temp_text[MAX_TEXT_LEN];
    if (!safe_strncpy(temp_text, input, MAX_TEXT_LEN)) return 0;
    
    char *token = strtok(temp_text, " ");
    while (token != NULL && analyzer->word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            int word_idx = find_word_index(analyzer, token);
            if (word_idx == -1) {
                if (!safe_strncpy(analyzer->words[analyzer->word_count].word, token, MAX_WORD_LEN)) {
                    return 0;
                }
                analyzer->words[analyzer->word_count].count = 1;
                analyzer->words[analyzer->word_count].length = (int)token_len;
                analyzer->words[analyzer->word_count].is_capitalized = isupper(token[0]) ? 1 : 0;
                analyzer->word_count++;
            } else {
                analyzer->words[word_idx].count++;
            }
            analyzer->total_words++;
            analyzer->total_chars += (int)token_len;
        }
        token = strtok(NULL, " ");
    }
    
    return 1;
}

void print_analysis(struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Text Analysis Results:\n");
    printf("Processed text: %s\n", analyzer->processed_text);
    printf("Total words: %d\n", analyzer->total_words);
    printf("Total characters (words only): %d\n", analyzer->total_chars);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("\nWord Statistics:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("Word: %-15s Count: %-3d Length: %-2d Capitalized: %s\n",
               analyzer->words[i].word,
               analyzer->words[i].count,
               analyzer->words[i].length,
               analyzer->words[i].is_capitalized ? "Yes" : "No");
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (letters and spaces only): ");
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("Empty input provided.\n");
        return 1;
    }
    
    if (!process_text