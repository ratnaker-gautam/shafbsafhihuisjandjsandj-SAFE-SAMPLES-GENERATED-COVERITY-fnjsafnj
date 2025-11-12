//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define MAX_TEXT_LEN 1000

struct WordInfo {
    char word[MAX_WORD_LEN];
    int count;
    int first_position;
};

struct TextAnalyzer {
    struct WordInfo words[MAX_WORDS];
    int word_count;
    char original_text[MAX_TEXT_LEN];
    int total_words;
};

void initialize_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->original_text[0] = '\0';
    analyzer->total_words = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].count = 0;
        analyzer->words[i].first_position = -1;
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n';
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

int process_text(struct TextAnalyzer *analyzer, const char *input_text) {
    if (analyzer == NULL || input_text == NULL) return 0;
    
    size_t input_len = strlen(input_text);
    if (input_len == 0 || input_len >= MAX_TEXT_LEN) return 0;
    
    for (size_t i = 0; i < input_len; i++) {
        if (!is_valid_char(input_text[i])) return 0;
    }
    
    if (!safe_strncpy(analyzer->original_text, input_text, MAX_TEXT_LEN)) return 0;
    
    char text_copy[MAX_TEXT_LEN];
    if (!safe_strncpy(text_copy, input_text, MAX_TEXT_LEN)) return 0;
    
    char *token = strtok(text_copy, " \t\n");
    int position = 0;
    
    while (token != NULL && analyzer->word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            int found = 0;
            for (int i = 0; i < analyzer->word_count; i++) {
                if (strcmp(analyzer->words[i].word, token) == 0) {
                    analyzer->words[i].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                if (!safe_strncpy(analyzer->words[analyzer->word_count].word, token, MAX_WORD_LEN)) {
                    return 0;
                }
                analyzer->words[analyzer->word_count].count = 1;
                analyzer->words[analyzer->word_count].first_position = position;
                analyzer->word_count++;
            }
            analyzer->total_words++;
            position++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Original text: %s\n", analyzer->original_text);
    printf("Total words processed: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("\nWord analysis:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("Word: %-15s Count: %-3d First position: %d\n",
               analyzer->words[i].word,
               analyzer->words[i].count,
               analyzer->words[i].first_position);
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    initialize_analyzer(&analyzer);
    
    char input_buffer[MAX_TEXT_LEN];
    
    printf("Enter text to analyze (max %d characters): ", MAX_TEXT_LEN - 1);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (!process_text(&analyzer, input_buffer)) {
        printf("Error processing text\n");
        return 1;
    }
    
    print_analysis(&analyzer);
    
    return 0;
}