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

void process_text(struct TextAnalyzer *analyzer, const char *input) {
    if (input == NULL || strlen(input) >= MAX_TEXT_LEN) return;
    
    safe_strncpy(analyzer->processed_text, input, MAX_TEXT_LEN);
    analyzer->total_chars = strlen(analyzer->processed_text);
    
    char buffer[MAX_TEXT_LEN];
    safe_strncpy(buffer, input, MAX_TEXT_LEN);
    
    char *token = strtok(buffer, " \t\n.,!?");
    while (token != NULL && analyzer->word_count < MAX_WORDS) {
        int found = -1;
        for (int i = 0; i < analyzer->word_count; i++) {
            if (strcmp(analyzer->words[i].word, token) == 0) {
                found = i;
                break;
            }
        }
        
        if (found >= 0) {
            analyzer->words[found].count++;
        } else {
            struct WordStats *ws = &analyzer->words[analyzer->word_count];
            safe_strncpy(ws->word, token, MAX_WORD_LEN);
            ws->length = strlen(token);
            ws->count = 1;
            ws->is_capitalized = isupper(token[0]) ? 1 : 0;
            analyzer->word_count++;
        }
        
        analyzer->total_words++;
        token = strtok(NULL, " \t\n.,!?");
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Text Analysis Results:\n");
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("\nWord Statistics:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *ws = &analyzer->words[i];
        printf("Word: %-15s Count: %-3d Length: %-2d %s\n", 
               ws->word, ws->count, ws->length, 
               ws->is_capitalized ? "(Capitalized)" : "");
    }
}

int get_user_input(char *buffer, int max_len) {
    if (buffer == NULL || max_len <= 0) return 0;
    
    printf("Enter text to analyze (max %d chars): ", max_len - 1);
    
    if (fgets(buffer, max_len, stdin) == NULL) return 0;
    
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
    
    if (!get_user_input(input_buffer, MAX_TEXT_LEN)) {
        printf("Failed to get valid input.\n");
        return 1;
    }
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    process_text(&analyzer, input_buffer);
    print_analysis(&analyzer);
    
    return 0;
}