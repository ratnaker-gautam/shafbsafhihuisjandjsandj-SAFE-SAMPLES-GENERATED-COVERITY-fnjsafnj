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
    int first_position;
    int last_position;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    int unique_words;
    char text[MAX_TEXT_LEN];
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->unique_words = 0;
    memset(analyzer->text, 0, MAX_TEXT_LEN);
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(analyzer->words[i].word, 0, MAX_WORD_LEN);
        analyzer->words[i].count = 0;
        analyzer->words[i].first_position = -1;
        analyzer->words[i].last_position = -1;
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n' || c == '\0';
}

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return 0;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        if (!is_valid_char(src[i])) return 0;
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

int find_word(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lowercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int process_text(struct TextAnalyzer *analyzer, const char *input_text) {
    if (input_text == NULL || strlen(input_text) >= MAX_TEXT_LEN - 1) {
        return 0;
    }
    
    if (!safe_strncpy(analyzer->text, input_text, MAX_TEXT_LEN)) {
        return 0;
    }
    
    char text_copy[MAX_TEXT_LEN];
    if (!safe_strncpy(text_copy, input_text, MAX_TEXT_LEN)) {
        return 0;
    }
    
    to_lowercase(text_copy);
    
    char *token = strtok(text_copy, " \t\n");
    int position = 0;
    
    while (token != NULL && analyzer->word_count < MAX_WORDS) {
        if (strlen(token) > 0 && strlen(token) < MAX_WORD_LEN) {
            int word_index = find_word(analyzer, token);
            
            if (word_index == -1) {
                if (analyzer->word_count < MAX_WORDS) {
                    struct WordStats *ws = &analyzer->words[analyzer->word_count];
                    if (safe_strncpy(ws->word, token, MAX_WORD_LEN)) {
                        ws->count = 1;
                        ws->first_position = position;
                        ws->last_position = position;
                        analyzer->word_count++;
                        analyzer->unique_words++;
                    }
                }
            } else {
                analyzer->words[word_index].count++;
                analyzer->words[word_index].last_position = position;
            }
            
            analyzer->total_words++;
            position++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    return 1;
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("\nWord Statistics:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        struct WordStats *ws = &analyzer->words[i];
        printf("Word: '%-15s' Count: %3d  First: %3d  Last: %3d\n", 
               ws->word, ws->count, ws->first_position, ws->last_position);
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, MAX_TEXT_LEN, stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (!process_text(&analyzer,