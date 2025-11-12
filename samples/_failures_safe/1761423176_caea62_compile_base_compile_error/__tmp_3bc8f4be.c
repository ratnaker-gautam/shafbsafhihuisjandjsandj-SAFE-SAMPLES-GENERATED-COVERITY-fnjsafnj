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
    if (!analyzer) return;
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

void process_word(struct TextAnalyzer *analyzer, const char *word, int len) {
    if (len <= 0 || len >= MAX_WORD_LEN) return;
    if (!analyzer || !word) return;
    
    char normalized[MAX_WORD_LEN];
    int normalized_len = 0;
    int capitalized = isupper((unsigned char)word[0]);
    
    for (int i = 0; i < len && normalized_len < MAX_WORD_LEN - 1; i++) {
        normalized[normalized_len++] = tolower((unsigned char)word[i]);
    }
    normalized[normalized_len] = '\0';
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, normalized) == 0) {
            analyzer->words[i].count++;
            if (capitalized) {
                analyzer->words[i].is_capitalized = 1;
            }
            return;
        }
    }
    
    if (analyzer->word_count < MAX_WORDS) {
        strncpy(analyzer->words[analyzer->word_count].word, normalized, MAX_WORD_LEN - 1);
        analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = normalized_len;
        analyzer->words[analyzer->word_count].is_capitalized = capitalized;
        
        if (normalized_len > analyzer->longest_word_length) {
            analyzer->longest_word_length = normalized_len;
        }
        if (normalized_len < analyzer->shortest_word_length) {
            analyzer->shortest_word_length = normalized_len;
        }
        
        analyzer->word_count++;
    }
    
    analyzer->total_words++;
}

void analyze_text(struct TextAnalyzer *analyzer, const char *text) {
    if (!analyzer || !text) return;
    
    int len = strlen(text);
    if (len <= 0) return;
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    
    for (int i = 0; i <= len; i++) {
        char c = text[i];
        
        if (is_word_char(c) && buf_pos < MAX_WORD_LEN - 1) {
            buffer[buf_pos++] = c;
        } else {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                process_word(analyzer, buffer, buf_pos);
                buf_pos = 0;
            }
        }
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    if (!analyzer) return;
    
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].count > 0) {
            printf("%s: %d occurrence(s)", analyzer->words[i].word, analyzer->words[i].count);
            if (analyzer->words[i].is_capitalized) {
                printf(" (capitalized)");
            }
            printf("\n");
        }
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_buffer[4096];
    printf("Enter text to analyze (max 4095 characters):\n");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin)) {
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        if (strlen(input_buffer) > 0) {
            analyze_text(&analyzer, input_buffer);
            print_analysis(&analyzer);
        } else {
            printf("