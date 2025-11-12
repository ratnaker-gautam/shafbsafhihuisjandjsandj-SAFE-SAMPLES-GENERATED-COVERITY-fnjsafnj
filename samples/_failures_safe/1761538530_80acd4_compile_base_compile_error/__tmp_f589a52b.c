//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000
#define MAX_LINE_LEN 256

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
    int unique_words;
    int longest_word_length;
    int shortest_word_length;
};

struct TextProcessor {
    char input_text[MAX_LINE_LEN * 10];
    char processed_text[MAX_LINE_LEN * 10];
    int text_length;
    int line_count;
};

void initialize_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->unique_words = 0;
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

void initialize_processor(struct TextProcessor *processor) {
    processor->input_text[0] = '\0';
    processor->processed_text[0] = '\0';
    processor->text_length = 0;
    processor->line_count = 0;
}

int safe_strlen(const char *str, int max_len) {
    int len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c) || ispunct((unsigned char)c);
}

int sanitize_input(char *input, int max_len) {
    if (input == NULL) return 0;
    
    int len = safe_strlen(input, max_len);
    for (int i = 0; i < len; i++) {
        if (!is_valid_char(input[i])) {
            return 0;
        }
    }
    return 1;
}

void process_text(struct TextProcessor *processor) {
    if (processor == NULL || processor->input_text[0] == '\0') return;
    
    int input_len = safe_strlen(processor->input_text, sizeof(processor->input_text));
    int processed_idx = 0;
    processor->line_count = 0;
    
    for (int i = 0; i < input_len && processed_idx < (int)sizeof(processor->processed_text) - 1; i++) {
        char c = processor->input_text[i];
        
        if (c == '\n') {
            processor->line_count++;
        }
        
        if (isalnum((unsigned char)c) || isspace((unsigned char)c) || ispunct((unsigned char)c)) {
            processor->processed_text[processed_idx++] = c;
        }
    }
    processor->processed_text[processed_idx] = '\0';
    processor->text_length = processed_idx;
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void analyze_words(struct TextAnalyzer *analyzer, struct TextProcessor *processor) {
    if (analyzer == NULL || processor == NULL) return;
    
    char text_copy[MAX_LINE_LEN * 10];
    int text_len = safe_strlen(processor->processed_text, sizeof(text_copy));
    if (text_len >= (int)sizeof(text_copy)) return;
    
    strncpy(text_copy, processor->processed_text, sizeof(text_copy) - 1);
    text_copy[sizeof(text_copy) - 1] = '\0';
    
    char *token = strtok(text_copy, " \t\n\r.,;:!?\"'()[]{}");
    while (token != NULL && analyzer->word_count < MAX_WORDS) {
        int token_len = safe_strlen(token, MAX_WORD_LEN);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            int word_idx = find_word_index(analyzer, token);
            
            if (word_idx == -1) {
                strncpy(analyzer->words[analyzer->word_count].word, token, MAX_WORD_LEN - 1);
                analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
                analyzer->words[analyzer->word_count].count = 1;
                analyzer->words[analyzer->word_count].length = token_len;
                analyzer->words[analyzer->word_count].is_capitalized = isupper((unsigned char)token[0]) ? 1 : 0;
                analyzer->word_count++;
                analyzer->unique_words++;
            } else {
                analyzer->words[word_idx].count++;
            }
            
            analyzer->total_words++;
            
            if (token_len > analyzer->longest_word_length) {
                analyzer->longest_word_length = token_len;
            }
            if (token_len < analyzer->shortest_word